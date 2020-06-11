#include "mailbox_utilities.h"
#include <stdlib.h>
#include <applibs/application.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <applibs/log.h>
#include <string.h>
#include "eventloop_timer_utilities.h"
#include <pthread.h>
#include "common.h"
#define COMMAND_BYTE 0

#define MQTT_CONFIG_LOAD 0
#define MQTT_CONFIG_SAVE 1
#define IP_ADDRESS_KEEP 2
#define NETWORK_ERROR_SET 3
#define NETWORK_ERROR_CLEAR 4
#define MQTT_ERROR_SET 5
#define MQTT_ERROR_CLEAR 6
#define SP_PV_GET 7
#define SP_SET 8
#define CONTROL_CONFIG_LOAD 9
#define CONTROL_CONFIG_SAVE 10

static const char rtAppComponentId[] = "4bb8a11f-05e7-4d44-9776-10fec85f7a5f";
static uint8_t rxBuf[1024];
static uint8_t txBuf[1024];
static int sockFd = -1;
static pthread_mutex_t	mailboxMtx = PTHREAD_MUTEX_INITIALIZER;

/* external functions */
int mailboxInit();
void mailboxStop();

struct MQTTConfig mqttConfigLoad();
void mqttConfigSave(struct MQTTConfig conf);
void ipAddressKeep(struct IPAddress addr);
void networkErrorSet();
void networkErrorClear();
void mqttErrorSet();
void mqttErrorClear();
void getSpPv(double* sp, double* pv);
void setSp(double sp);
void controlConfigLoad(struct InputPeriphConfig* iconf, struct OutputPeriphConfig* oconf, struct ControllerConfig* cconf);
void controlConfigSave(struct InputPeriphConfig iconf, struct OutputPeriphConfig oconf, struct ControllerConfig cconf);

/* helper functions */
/**
* @brief Communicate with rt core
*
* @param size Number of bytes in txBuf to be sent.
*/
void performTransaction(int size);

/* declarations */
int mailboxInit() {
    // Open a connection to the RTApp.
    sockFd = Application_Connect(rtAppComponentId);
    if (sockFd == -1) {
        Log_Debug("ERROR: Unable to create socket: %d (%s)\n", errno, strerror(errno));
        return -1;
    }

    // Set timeout, to handle case where real-time capable application does not respond.
    static const struct timeval recvTimeout = { .tv_sec = 2, .tv_usec = 0 };
    int result = setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &recvTimeout, sizeof(recvTimeout));
    if (result == -1) {
        Log_Debug("ERROR: Unable to set socket timeout: %d (%s)\n", errno, strerror(errno));
        return -1;
    }

    return 0;
}

void mailboxStop() {
    if (sockFd >= 0) close(sockFd);
    sockFd = -1;
}

struct MQTTConfig mqttConfigLoad() {
    struct MQTTConfig conf;
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = MQTT_CONFIG_LOAD;

    performTransaction(1);

    memcpy((uint8_t*)&conf, rxBuf, sizeof(struct MQTTConfig));

    pthread_mutex_unlock(&mailboxMtx);
    return conf;
}

void mqttConfigSave(struct MQTTConfig conf) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = MQTT_CONFIG_SAVE;
    memcpy(txBuf + 1, (uint8_t*)&conf, sizeof(struct MQTTConfig));

    performTransaction(1 + sizeof(struct MQTTConfig));

    pthread_mutex_unlock(&mailboxMtx);
}

void ipAddressKeep(struct IPAddress addr) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = IP_ADDRESS_KEEP;
    memcpy(txBuf + 1, (uint8_t*)&addr, sizeof(struct IPAddress));

    performTransaction(1 + sizeof(struct IPAddress));

    pthread_mutex_unlock(&mailboxMtx);
}

void networkErrorSet() {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = NETWORK_ERROR_SET;

    performTransaction(1);

    pthread_mutex_unlock(&mailboxMtx);
}

void networkErrorClear() {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = NETWORK_ERROR_CLEAR;

    performTransaction(1);

    pthread_mutex_unlock(&mailboxMtx);
}

void mqttErrorSet() {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = MQTT_ERROR_SET;

    performTransaction(1);

    pthread_mutex_unlock(&mailboxMtx);
}

void mqttErrorClear() {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = MQTT_ERROR_CLEAR;

    performTransaction(1);

    pthread_mutex_unlock(&mailboxMtx);
}

void getSpPv(double *sp, double *pv) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = SP_PV_GET;

    performTransaction(1);

    memcpy((uint8_t*)sp, rxBuf, sizeof(double));
    memcpy((uint8_t*)pv, rxBuf + sizeof(double), sizeof(double));

    pthread_mutex_unlock(&mailboxMtx);
}

void setSp(double sp) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = SP_SET;
    memcpy(txBuf + 1, (uint8_t*)&sp, sizeof(double));

    performTransaction(1 + sizeof(double));

    pthread_mutex_unlock(&mailboxMtx);
}

void controlConfigLoad(struct InputPeriphConfig *iconf, struct OutputPeriphConfig *oconf, struct ControllerConfig *cconf) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = CONTROL_CONFIG_LOAD;

    performTransaction(1);

    memcpy((uint8_t*)iconf, rxBuf, sizeof(struct InputPeriphConfig));
    memcpy((uint8_t*)oconf, rxBuf + sizeof(struct InputPeriphConfig), sizeof(struct OutputPeriphConfig));
    memcpy((uint8_t*)cconf, rxBuf + sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig), sizeof(struct ControllerConfig));

    pthread_mutex_unlock(&mailboxMtx);
}

void controlConfigSave(struct InputPeriphConfig iconf, struct OutputPeriphConfig oconf, struct ControllerConfig cconf) {
    pthread_mutex_lock(&mailboxMtx);

    txBuf[0] = CONTROL_CONFIG_SAVE;

    memcpy(txBuf + 1, (uint8_t*)&iconf,  sizeof(struct InputPeriphConfig));
    memcpy(txBuf + 1 + sizeof(struct InputPeriphConfig), (uint8_t*)&oconf, sizeof(struct OutputPeriphConfig));
    memcpy(txBuf + 1 + sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig), (uint8_t*)&cconf, sizeof(struct ControllerConfig));

    performTransaction(1 + sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig) + sizeof(struct ControllerConfig));

    pthread_mutex_unlock(&mailboxMtx);
}

/* declarations helpers */
void performTransaction(int size) {
    int bytesReceived = 0;
    while (bytesReceived <= 0) {
        send(sockFd, txBuf, size, 0);
        bytesReceived = recv(sockFd, rxBuf, sizeof(rxBuf), 0);
    }
    //Log_Debug("Received %d bytes\n", bytesReceived);
}