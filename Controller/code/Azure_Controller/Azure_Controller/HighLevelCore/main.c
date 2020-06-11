#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#include "applibs_versions.h"
#include <applibs/log.h>
#include <applibs/networking.h>

#include "eventloop_timer_utilities.h"

#include "common.h"
#include "mqtt_utilities.h"
#include "http_utilities.h"
#include "mailbox_utilities.h"
#include "network_utilities.h"
#include "config_structs.h"
#include "website.h"
#include "cloud.h"


#define MQTT_PUBLISH_PERIOD 10
#define CLOUD_PUBLISH_PERIOD 60 

typedef enum {
    ExitCode_Success = 0,
    ExitCode_TermHandler_SigTerm = 1,
    ExitCode_TimerHandler_Consume = 2,
    ExitCode_Init_EventLoop = 3,
    ExitCode_Init_Timer = 4,
    ExitCode_Main_EventLoopFail = 5,
    ExitCode_Init_Mailbox = 6,
    ExitCode_Init_HTTP =7
} ExitCode;

static EventLoop* eventLoop = NULL;
static volatile sig_atomic_t exitCode = ExitCode_Success;

int main(int argc, char* argv[]);

/**
* @brief Signal handler for termination requests.
* 
* @param signalNumber Unused.
*/
static void TerminationHandler(int signalNumber);

/**
* @brief Init event loop, timers, mailbox and http server.
*
* @return ExitCode_Success on success, other ExitCode on failure.
*/
static ExitCode InitHandlers(void);

/**
* @brief Close event loop, dispose timers, close MQTT, HTTP server and mailbox.
*/
static void CloseHandlers(void);

/* cloud */
/**
* @brief Periodically called to send diagnostic info to Azure cloud services.
*
* @param timer Pointer to timer to consume and restart.
*/
static void cloudPublishTimerEventHandler(EventLoopTimer* timer);
static EventLoopTimer* publishCloudMsgTimer = NULL;
static int telemetryCount = 0;
static struct timespec azureTelemetryPeriod;

/* website */
/**
* @brief Callback received from HTTP server when GET method has been received.
*
* @param siteLocation String that contains site location. i.e /config.
* @param argKeys Keys from key-value pair passed to GET method after ? symbol.
* @param argValues Values from key-value pair passed to GET method after ? symbol.
* @return Pointer to internal buffer that holds website data to be sent to client.
*/
static const char* HTTP_GET_MethodCallback(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]);

/**
* @brief Parse params received from GET request
*
* @param argKeys Keys from key-value pair passed to GET method after ? symbol.
* @param argValues Values from key-value pair passed to GET method after ? symbol.
*/
static void parseGETParams(char argKeys[20][31], char argValues[20][31]);

/* mqtt */
/**
* @brief Periodically called to send setpoint and process value to MQTT broker.
*
* @param timer Pointer to timer to consume and restart.
*/
static void mqttPublishTimerEventHandler(EventLoopTimer* timer);

/**
* @brief Called when device receives data from subscribed setpoint topic.
* 
* @param topic Name of topic from which message has come.
* @param msg Message.
*/
static void mqttSubscribeCallback(char* topic, char* msg);
static EventLoopTimer* publishMQTTMsgTimer = NULL;
static struct MQTTConfig mqttConf = {
    .processValueTopic = "\0",
    .setpointTopic = "\0",
    .brokerIPAddress = "\0"
};
static bool reloadMQTT = false;

int main(int argc, char*argv[])
{
    if (argc > 1) {
        setCloudScopeId(argv[1]);
    }
    else {
        Log_Debug("Cloud scope is required to run this application.\n");
        return -1;
    }

    waitMs(2000);// wait a bit for real time core to set up stuff

    signal(SIGPIPE, SIG_IGN);

    exitCode = InitHandlers();

    while (exitCode == ExitCode_Success) {
        EventLoop_Run_Result result = EventLoop_Run(eventLoop, -1, true);

        // errno in case of breakpoint
        if (result == EventLoop_Run_Failed && errno != EINTR) {
            exitCode = ExitCode_Main_EventLoopFail;
        }
    }

    CloseHandlers();
    Log_Debug("Application exiting\n");
    return exitCode;
}

static void TerminationHandler(int signalNumber)
{
    exitCode = ExitCode_TermHandler_SigTerm;
}

static ExitCode InitHandlers(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = TerminationHandler;
    sigaction(SIGTERM, &action, NULL);

    eventLoop = EventLoop_Create();
    if (eventLoop == NULL) {
        Log_Debug("Could not create event loop.\n");
        return ExitCode_Init_EventLoop;
    }

    // Callback for MQTT subscribe
    MQTTRegisterSubscribeCallback(mqttSubscribeCallback);

    // Init config http server
    if (httpServerInit() == -1) {
        return ExitCode_Init_HTTP;
    }
    registerGETCallback(HTTP_GET_MethodCallback);

    // Init communication with RT Core
    if (mailboxInit() == -1) {
        return ExitCode_Init_Mailbox;
    }

    // Register a ten second timer to publish setpoint and pv to mqtt broker
    static const struct timespec sendPeriodMQTT = { .tv_sec = MQTT_PUBLISH_PERIOD, .tv_nsec = 0 };
    publishMQTTMsgTimer = CreateEventLoopPeriodicTimer(eventLoop, &mqttPublishTimerEventHandler, &sendPeriodMQTT);
    if (publishMQTTMsgTimer == NULL) {
        return ExitCode_Init_Timer;
    }

    // Register timer to send data to cloud
    azureTelemetryPeriod.tv_sec = getCloudPollPeriodSeconds();
    publishCloudMsgTimer = CreateEventLoopPeriodicTimer(eventLoop, &cloudPublishTimerEventHandler, &azureTelemetryPeriod);
    if (publishCloudMsgTimer == NULL) {
        return ExitCode_Init_Timer;
    }

    return ExitCode_Success;
}

static void CloseHandlers(void)
{
    DisposeEventLoopTimer(publishMQTTMsgTimer);
    DisposeEventLoopTimer(publishCloudMsgTimer);

    EventLoop_Close(eventLoop);

    MQTTStop();
    MQTTKillSubthread();
    httpServerStop();
    httpServerKillSubthread();
    mailboxStop();
}

static void cloudPublishTimerEventHandler(EventLoopTimer* timer) {
    if (ConsumeEventLoopTimerEvent(timer) != 0) {
        exitCode = ExitCode_TimerHandler_Consume;
        return;
    }

    bool isNetworkReady = false;
    if (Networking_IsNetworkingReady(&isNetworkReady) != -1) {
        if (isNetworkReady && !isCloudAuthenticated()) {
            SetupAzureClient();
        }
    }

    if (isCloudAuthenticated()) {
        telemetryCount++;
        if (telemetryCount == CLOUD_PUBLISH_PERIOD) {
            telemetryCount = 0;
            char telemetryBuf[128];
            double sp, pv;
            getSpPv(&sp, &pv);
            snprintf(telemetryBuf, 128, "{\"Setpoint\": \"%f\", \"ProcessValue\": \"%f\"}", sp, pv);
            sendTelemetryToCloud(telemetryBuf);
        }
        doCloudLLWork();
    }
    else {
    }

    // update telemetry period
    azureTelemetryPeriod.tv_sec = getCloudPollPeriodSeconds();
    SetEventLoopTimerPeriod(timer, &azureTelemetryPeriod);
}

static const char* HTTP_GET_MethodCallback(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]) {
    static char buf[4096];
    Log_Debug("GET callback\n");
    Log_Debug("%s\n", siteLocation);
    if (strcmp(siteLocation, "/config") == 0) {
        Log_Debug("config website\n");

        if (hasParams) {
            parseGETParams(argKeys, argValues);
        }

        double sp, pv;
        getSpPv(&sp, &pv);
        struct MQTTConfig mqttConf = mqttConfigLoad();
        struct InputPeriphConfig iconf;
        struct OutputPeriphConfig oconf;
        struct ControllerConfig cconf;
        controlConfigLoad(&iconf, &oconf, &cconf);
        const char* selectedNONE = cconf.controllerType == NONE ? "selected=\"selected\"" : "";
        const char* selectedPID = cconf.controllerType == PID ? "selected=\"selected\"" : "";
        const char* selectedTS = cconf.controllerType == TWO_STATE ? "selected=\"selected\"" : "";
        Log_Debug("IF name: %s\n", getSavedNetworkInterfaceName());
        snprintf(buf, 4096, page, sp,getSavedNetworkInterfaceName(),"111111",mqttConf.brokerIPAddress,mqttConf.setpointTopic,mqttConf.processValueTopic,
            iconf.inputMinValue,iconf.inputMaxValue,oconf.outputMinValue,oconf.outputMaxValue,selectedNONE, selectedTS,selectedPID, 
            cconf.controllerConfig.twoStateConfig.bottomSwitchBoundary, cconf.controllerConfig.twoStateConfig.topSwitchBoundary,
            cconf.controllerConfig.twoStateConfig.offValue, cconf.controllerConfig.twoStateConfig.onValue,
            cconf.controllerConfig.PIDConfig.kp, cconf.controllerConfig.PIDConfig.ki, cconf.controllerConfig.PIDConfig.kd,
            cconf.controllerConfig.PIDConfig.saturationLower, cconf.controllerConfig.PIDConfig.saturarionUpper);
    }
    else {
        Log_Debug("other website\n");
        memcpy(buf, nfound, strlen(nfound)+1);
        
    }
    return buf;
}


static void parseGETParams(char argKeys[20][31], char argValues[20][31]) {

    struct MQTTConfig mqttConf = mqttConfigLoad();
    struct InputPeriphConfig iconf;
    struct OutputPeriphConfig oconf;
    struct ControllerConfig cconf;
    char ssid[31] = "USER_NET\0";
    char psk[31] = "12345678\0";
    bool setWIFI = false;

    for (uint8_t i = 0; i < 20; i++) {
        if (strcmp(argKeys[i], "setpoint") == 0) {
            setSp(atof(argValues[i]));
        }
        else if (strcmp(argKeys[i], "ssid") == 0) {
            memcpy(ssid, argValues[i], strlen(argValues[i]) + 1);
            //Log_Debug("Received IF name: %s\n", ssid);
        }
        else if (strcmp(argKeys[i], "psk") == 0) {
            if (strcmp(argValues[i], "111111") != 0) {
                memcpy(psk, argValues[i], strlen(argValues[i]) + 1);
                setWIFI = true;
            }
        }
        else if (strcmp(argKeys[i], "brokerip") == 0) {
            memcpy(mqttConf.brokerIPAddress, argValues[i], strlen(argValues[i])+1);
        }
        else if (strcmp(argKeys[i], "sptopic") == 0) {
            memcpy(mqttConf.setpointTopic, argValues[i], strlen(argValues[i]) + 1);
        }
        else if (strcmp(argKeys[i], "pvtopic") == 0) {
            memcpy(mqttConf.processValueTopic, argValues[i], strlen(argValues[i]) + 1);
        }
        else if (strcmp(argKeys[i], "inputmin") == 0) {
            iconf.inputMinValue = atof(argValues[i]);
        }
        else if (strcmp(argKeys[i], "inputmax") == 0) {
            iconf.inputMaxValue = atof(argValues[i]);
        }
        else if (strcmp(argKeys[i], "outputmin") == 0) {
            oconf.outputMinValue = atof(argValues[i]);
        }
        else if (strcmp(argKeys[i], "outputmax") == 0) {
            oconf.outputMaxValue = atof(argValues[i]);
        }
        else if (strcmp(argKeys[i], "selectedcontroller") == 0) {
            if (strcmp(argValues[i], "None") == 0)
                cconf.controllerType = NONE;
            if (strcmp(argValues[i], "Two+State") == 0) {
                cconf.controllerType = TWO_STATE;
                for (uint8_t j = 0; j < 20; j++) {
                    if (strcmp(argKeys[j], "tstoggleoff") == 0) {
                        cconf.controllerConfig.twoStateConfig.bottomSwitchBoundary = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "tstoggleon") == 0) {
                        cconf.controllerConfig.twoStateConfig.topSwitchBoundary = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "tsoffoutput") == 0) {
                        cconf.controllerConfig.twoStateConfig.offValue = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "tsonoutput") == 0) {
                        cconf.controllerConfig.twoStateConfig.onValue = atof(argValues[j]);
                    }
                }
            }
            if (strcmp(argValues[i], "PID") == 0) {
                cconf.controllerType = PID;
                for (uint8_t j = 0; j < 20; j++) {
                    if (strcmp(argKeys[j], "pidkp") == 0) {
                        cconf.controllerConfig.PIDConfig.kp = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "pidki") == 0) {
                        cconf.controllerConfig.PIDConfig.ki = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "pidkd") == 0) {
                        cconf.controllerConfig.PIDConfig.kd = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "pidlowsat") == 0) {
                        cconf.controllerConfig.PIDConfig.saturationLower = atof(argValues[j]);
                    }
                    else if (strcmp(argKeys[j], "pidupsat") == 0) {
                        cconf.controllerConfig.PIDConfig.saturarionUpper = atof(argValues[j]);
                    }
                }
            }
        }
    }
    mqttConfigSave(mqttConf);
    reloadMQTT = true;
    controlConfigSave(iconf, oconf, cconf);
    if (setWIFI)
        updateUserDefinedNetwork(ssid, psk);
}

static void mqttPublishTimerEventHandler(EventLoopTimer* timer)
{
    if (ConsumeEventLoopTimerEvent(timer) != 0) {
        exitCode = ExitCode_TimerHandler_Consume;
        return;
    }

    if (!isNetworkConnected()) {
        mqttErrorSet();
        networkErrorSet();
        MQTTStop();
        return;
    }
    else {
        ipAddressKeep(getIPAddress());
        networkErrorClear();
    }

    if (!strcmp(mqttConf.brokerIPAddress, "")) {
        mqttConf = mqttConfigLoad();
        Log_Debug("MQTT config loaded as: SP topic: %s, PV topic: %s, Broker ip: %s\n", mqttConf.setpointTopic, mqttConf.processValueTopic, mqttConf.brokerIPAddress);
    }

    if (reloadMQTT) {
        MQTTStop();
        mqttConf = mqttConfigLoad();
        reloadMQTT = false;
        waitMs(50);
    }

    if (!MQTTIsActiveConnection()) {
        int res = MQTTInit(mqttConf.brokerIPAddress, "1883", mqttConf.setpointTopic);
        if (res) {
            mqttErrorSet();
            return;
        }
        mqttErrorClear();
        waitMs(50);
    }

    double sp, pv;
    getSpPv(&sp, &pv);
    char buf[100];
    snprintf(buf, 100, "%f", sp);
    if (MQTTPublish(mqttConf.setpointTopic, buf) != 0) {
        mqttErrorSet();
        MQTTStop();
    }

    snprintf(buf, 100, "%f", pv);
    if (MQTTPublish(mqttConf.processValueTopic, buf) != 0) {
        mqttErrorSet();
        MQTTStop();
    }
    else {
        mqttErrorClear();
    }
}

static void mqttSubscribeCallback(char* topic, char* msg) {
    if (strcmp(topic, mqttConf.setpointTopic) == 0) {
        setSp(atof(msg));
    }
}