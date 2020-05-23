#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <applibs/log.h>

#include "eventloop_timer_utilities.h"
#include "mqtt_utilities.h"
#include "http_utilities.h"
#include "mailbox_utilities.h"
#include "network_utilities.h"
#include "common.h"
#include "config_structs.h"
#include "website.h"

#define MQTT_PUBLISH_PERIOD 10
#define CLOUD_CHECK_PERIOD 10

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

int main(void);

/*
* @brief Signal handler for termination requests.
* 
* @param signalNumber Unused.
*/
static void TerminationHandler(int signalNumber);

/*
* @brief Init event loop, timers, mailbox and http server.
*
* @return ExitCode_Success on success, other ExitCode on failure.
*/
static ExitCode InitHandlers(void);

/*
* @brief Close event loop, dispose timers, close MQTT, HTTP server and mailbox.
*/
static void CloseHandlers(void);

/* cloud */
/*
* @brief Periodically called to send diagnostic info to Azure cloud services.
*
* @param timer Pointer to timer to consume and restart.
*/
static void cloudPublishTimerEventHandler(EventLoopTimer* timer);
static EventLoopTimer* publishCloudMsgTimer = NULL;

/* website */
/*
* @brief Callback received from HTTP server when GET method has been received.
*
* @param siteLocation String that contains site location. i.e /config.
* @param argKeys Keys from key-value pair passed to GET method after ? symbol.
* @param argValues Values from key-value pair passed to GET method after ? symbol.
* @return Pointer to internal buffer that holds website data to be sent to client.
*/
static const char* HTTP_GET_MethodCallback(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]);

/* mqtt */
/*
* @brief Periodically called to send setpoint and process value to MQTT broker.
*
* @param timer Pointer to timer to consume and restart.
*/
static void mqttPublishTimerEventHandler(EventLoopTimer* timer);
static EventLoopTimer* publishMQTTMsgTimer = NULL;
static struct MQTTConfig mqttConf = {
    .processValueTopic = "\0",
    .setpointTopic = "\0",
    .brokerIPAddress = "\0"
};

int main(void)
{
    waitMs(2000);

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

    // Register a ten second timer to send data to cloud
    static const struct timespec sendPeriodCloud = { .tv_sec = CLOUD_CHECK_PERIOD, .tv_nsec = 0 };
    publishCloudMsgTimer = CreateEventLoopPeriodicTimer(eventLoop, &cloudPublishTimerEventHandler, &sendPeriodCloud);
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

    Log_Debug("Cloud publish\n");
}

static const char* HTTP_GET_MethodCallback(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]) {
    static char buf[4096];
    Log_Debug("GET callback\n");
    Log_Debug("%s\n", siteLocation);
    if (strcmp(siteLocation, "/config") == 0) {
        Log_Debug("config website\n");

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
        snprintf(buf, 4096, page, sp,getSavedNetworkInterfaceName(),"",mqttConf.brokerIPAddress,mqttConf.setpointTopic,mqttConf.processValueTopic,
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
        return;
    }

    if (!MQTTIsActiveConnection()) {
        int res = MQTTInit("192.168.0.100", "1883", "SP");
        if (!res)
            return;
        waitMs(50);
    }

    double sp, pv;
    getSpPv(&sp, &pv);
    Log_Debug("%f, %f\n", sp, pv);
    if (MQTTPublish("PV", "Hello world!") != 0) {
        mqttErrorSet();
        MQTTStop();
    }
    else {
        mqttErrorClear();
    }
}