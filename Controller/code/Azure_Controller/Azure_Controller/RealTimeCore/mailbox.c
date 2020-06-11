#include <stdio.h>
#include <string.h>
#include "printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "global_state.h"
#include "intercomm/logical-intercore.h"
#include "intercomm/intercore.h"

#include "config.h"

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

static const ComponentId hlAppId = { .data1 = 0x2af2765c,
										.data2 = 0x0d65,
										.data3 = 0x4e68,
										.data4 = {0xaa, 0x92, 0x03, 0x36, 0xd4, 0x16, 0xe3, 0x35} };
static IntercoreComm icc;

static const char* ok = 'OK\n';

/* external functions */
void MAILBOX_mailboxTask(void* pParam);

/* helpers */
/**
* @brief Called when data is received from high level core, processes the data and responds if necessary.
*/
void mailboxCallback();

/**
* @brief Responds with MQTT config.
*/
void respondWithMQTTConfig();

/**
* @brief Saves received MQTT config to EEPROM.
*
* @param rxData MQTT config in bytes.
*/
void saveMQTTConfig(char* rxData);

/**
* @brief Saves received IP Address to GLOBAL structure.
*
* @param rxData IP address in bytes.
*/
void keepIPAddress(char* rxData);

/**
* @brief Responds with GLOBAL setpoint and process value.
*/
void sendSpPv();

/**
* @brief Sets GLOBAL setpoint with value received from high level core.
*
* @param rxData setpoint in bytes.
*/
void setSp(char* rxData);

/**
* @brief Responds with control config (inputs, outputs, controller).
*/
void loadControlConfig();

/**
* @brief Saves received control config (inputs, outputs, controller) to EEPROM.
*
* @param rxData control config in bytes.
*/
void saveControlConfig(char* rxData);

/* definitions external functions */
void MAILBOX_mailboxTask(void* pParam) {

	NVIC_Register(11, MT3620_HandleMailboxIrq11);
	SetupIntercoreComm(&icc, mailboxCallback);

	while (1) {
		InvokeDeferredProcs();
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

/* definitions helpers */
void mailboxCallback() {
	ComponentId sender;
	uint8_t rxData[1024];
	size_t rxDataSize = sizeof(rxData);
	IntercoreRecv(&icc, &sender, rxData, &rxDataSize);

	if (!rxDataSize) {
		printf("Received empty callback\n");
		return;
	}

	printf("Received ");
	switch (rxData[COMMAND_BYTE]) {
	case MQTT_CONFIG_LOAD:
		printf("MQTT_CONFIG_LOAD\n");
		respondWithMQTTConfig();
		break;
	case MQTT_CONFIG_SAVE:
		printf("MQTT_CONFIG_SAVE\n");
		saveMQTTConfig(rxData + 1);
		break;
	case IP_ADDRESS_KEEP:
		printf("IP_ADDRESS_KEEP\n");
		keepIPAddress(rxData + 1);
		break;
	case NETWORK_ERROR_SET:
		printf("NETWORK_ERROR_SET\n");
		GLOBAL_setNetworkErrorFlag(true);
		IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
		break;
	case NETWORK_ERROR_CLEAR:
		printf("NETWOR_ERROR_CLEAR\n");
		GLOBAL_setNetworkErrorFlag(false);
		IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
		break;
	case MQTT_ERROR_SET:
		printf("MQTT_ERROR_SET\n");
		GLOBAL_setMQTTErrorFlag(true);
		IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
		break;
	case MQTT_ERROR_CLEAR:
		printf("MQTT_ERROR_CLEAR\n");
		GLOBAL_setMQTTErrorFlag(false);
		IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
		break;
	case SP_PV_GET:
		printf("SP_PV_GET\n");
		sendSpPv();
		break;
	case SP_SET:
		printf("SP_SET\n");
		setSp(rxData + 1);
		break;
	case CONTROL_CONFIG_LOAD:
		printf("CONTROL_CONFIG_LOAD\n");
		loadControlConfig();
		break;
	case CONTROL_CONFIG_SAVE:
		printf("CONTROL_CONFIG_SAVE\n");
		saveControlConfig(rxData + 1);
		break;
	default:
		printf("Received unknown callback\n");
		break;
	}
}

void respondWithMQTTConfig() {
	struct MQTTConfig conf = getMQTTConfig();

	IntercoreSend(&icc, &hlAppId, (uint8_t*)&conf, sizeof(struct MQTTConfig));
}

void saveMQTTConfig(char* rxData) {
	struct MQTTConfig conf;
	memcpy((uint8_t*)&conf, rxData, sizeof(struct MQTTConfig));
	printf("Received MQTT config:\n");
	printf("Broker ip: %s\n", conf.brokerIPAddress);
	printf("Setpoint topic: %s\n", conf.setpointTopic);
	printf("Process value topic: %s\n", conf.processValueTopic);

	setMQTTConfig(conf);

	IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char)*3);
}

void keepIPAddress(char* rxData) {
	struct IPAddress ip;
	memcpy(ip.octets, rxData, sizeof(struct IPAddress));

	printf("Ip address: %d-%d-%d-%d\n", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3] );

	GLOBAL_setIPAddress(ip);

	IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
}

void sendSpPv() {
	double sp = GLOBAL_getSetpointValue();
	double pv = GLOBAL_getProcessValue();

	char txData[sizeof(sp) * 2];
	memcpy(txData, (uint8_t*)&sp, sizeof(sp));
	memcpy(txData + sizeof(sp), (uint8_t*)&pv, sizeof(sp));

	IntercoreSend(&icc, &hlAppId, txData, sizeof(txData));
}

void setSp(char* rxData) {
	double sp;
	memcpy((uint8_t*)&sp, rxData, sizeof(double));

	printf("Setpoint: %f\n", sp);

	GLOBAL_setSetpointValue(sp);

	IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
}

void loadControlConfig() {
	
	struct InputPeriphConfig iconf = getInputPeriphConfig();
	struct OutputPeriphConfig oconf = getOutputPeriphConfig();
	struct ControllerConfig cconf = getSelectedControllerConfig();
	uint8_t *txData = pvPortMalloc(sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig) + sizeof(struct ControllerConfig));
	memcpy(txData, (uint8_t*)&iconf, sizeof(struct InputPeriphConfig));
	memcpy(txData + sizeof(struct InputPeriphConfig), (uint8_t*)&oconf, sizeof(struct OutputPeriphConfig));
	memcpy(txData + sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig), (uint8_t*)&cconf, sizeof(struct ControllerConfig));

	IntercoreSend(&icc, &hlAppId, txData, sizeof(struct InputPeriphConfig) + sizeof(struct OutputPeriphConfig) + sizeof(struct ControllerConfig));

	vPortFree(txData);
}

void saveControlConfig(char* rxData) {
	struct InputPeriphConfig iconf;
	struct OutputPeriphConfig oconf;
	struct ControllerConfig cconf;

	memcpy((uint8_t*)&iconf, rxData, sizeof(struct InputPeriphConfig));
	rxData += sizeof(struct InputPeriphConfig);
	memcpy((uint8_t*)&oconf, rxData, sizeof(struct OutputPeriphConfig));
	rxData += sizeof(struct OutputPeriphConfig);
	memcpy((uint8_t*)&cconf, rxData, sizeof(struct ControllerConfig));

	printf("Received input config:\n");
	printf("Input min: %f\n", iconf.inputMinValue);
	printf("Input max: %f\n", iconf.inputMaxValue);

	printf("Received output config:\n");
	printf("Output min: %f\n", oconf.outputMinValue);
	printf("Output max: %f\n", oconf.outputMaxValue);

	printf("Received controller config:\n");
	switch (cconf.controllerType) {
	case NONE:
		printf("Controller: NONE\n");
		break;
	case TWO_STATE:
		printf("Controller: TWO STATE\n");
		printf("Off value: %f\n", cconf.controllerConfig.twoStateConfig.offValue);
		printf("On value: %f\n", cconf.controllerConfig.twoStateConfig.onValue);
		printf("Bottom switch boundary: %f\n", cconf.controllerConfig.twoStateConfig.bottomSwitchBoundary);
		printf("Top switch boundary: %f\n", cconf.controllerConfig.twoStateConfig.topSwitchBoundary);
		break;
	case PID:
		printf("Controller: PID\n");
		printf("Kp value: %f\n", cconf.controllerConfig.PIDConfig.kp);
		printf("Ki value: %f\n", cconf.controllerConfig.PIDConfig.ki);
		printf("Kd value: %f\n", cconf.controllerConfig.PIDConfig.kd);
		printf("Lower saturation: %f\n", cconf.controllerConfig.PIDConfig.saturationLower);
		printf("Upper saturation: %f\n", cconf.controllerConfig.PIDConfig.saturarionUpper);
		break;
	}

	setInputPeriphConfig(iconf);
	setOutputPeriphConfig(oconf);
	setUsedController(cconf);

	IntercoreSend(&icc, &hlAppId, (uint8_t*)&ok, sizeof(char) * 3);
}