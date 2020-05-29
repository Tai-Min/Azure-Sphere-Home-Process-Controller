#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "printf.h"
#include "config.h"
#include "eeprom.h"
#include "global_state.h"

static struct ControllerConfig controllerConfig = {
	.controllerType = NONE
};

static struct InputPeriphConfig inputConfig = {
	.inputMinValue = 0,
	.inputMaxValue = 100
};

static struct OutputPeriphConfig outputConfig = {
	.outputMinValue = 0,
	.outputMaxValue = 100
};

static bool initFlag = false;

/* external functions */
void initConfig();
bool isConfigInit();

void setMQTTConfig(struct MQTTConfig conf);
struct MQTTConfig getMQTTConfig();

void setInputPeriphConfig(struct InputPeriphConfig inputConf);
struct InputPeriphConfig getInputPeriphConfig();

void setOutputPeriphConfig(struct OutputPeriphConfig outputConf);
struct OutputPeriphConfig getOutputPeriphConfig();

struct ControllerConfig getSelectedControllerConfig();

void setUsedController(struct ControllerConfig conf);

/* helpers */
/*
* @brief Set controller's config to none.
*/
static void useNoController();

/*
* @brief Set controller's config to two state.
*/
static void useTwoStateController(struct TwoStateConfig controlConf);

/*
* @brief Set controller's config to pid.
*/
static void usePIDController(struct PIDConfig controlConf);

/* definitions external functions */
void initConfig() {
	initEEPROMCommunication();

	if (!isEEPROMInitialized()) {
		struct MQTTConfig c = {
			.processValueTopic = "PV\0",
			.setpointTopic = "SP\0",
			.brokerIPAddress = "255.255.255.255\0"
		};
		writeInputConfigToEEPROM(inputConfig);
		writeOutputConfigToEEPROM(outputConfig);
		writeControllerConfigToEEPROM(controllerConfig);
		writeMQTTConfigToEEPROM(c);
		setEEPROMInitializedBit();
	}
	
	inputConfig = loadInputConfigFromEEPROM();
	outputConfig = loadOutputConfigFromEEPROM();
	controllerConfig = loadControllerConfigFromEEPROM();
	GLOBAL_setSetpointValue(inputConfig.inputMinValue);

	initFlag = true;
}

bool isConfigInit() {
	return initFlag;
}

void setMQTTConfig(struct MQTTConfig conf) {
	writeMQTTConfigToEEPROM(conf);
}

struct MQTTConfig getMQTTConfig() {
	struct MQTTConfig conf = loadMQTTConfigFromEEPROM();
	return conf;
}

void setInputPeriphConfig(struct InputPeriphConfig inputConf) {
	inputConfig = inputConf;
	double sp = GLOBAL_getSetpointValue();
	if (sp > inputConf.inputMaxValue)
		sp = inputConf.inputMaxValue;
	if (sp < inputConf.inputMinValue)
		sp = inputConf.inputMinValue;

	GLOBAL_setSetpointValue(sp);

	writeInputConfigToEEPROM(inputConf);
}

struct InputPeriphConfig getInputPeriphConfig() {
	return inputConfig;
}

void setOutputPeriphConfig(struct OutputPeriphConfig outputConf) {
	outputConfig = outputConf;
	writeOutputConfigToEEPROM(outputConf);
}

struct OutputPeriphConfig getOutputPeriphConfig() {
	return outputConfig;
}

void setUsedController(struct ControllerConfig conf) {
	switch (conf.controllerType) {
	case NONE:
		useNoController();
		break;
	case TWO_STATE:
		useTwoStateController(conf.controllerConfig.twoStateConfig);
		break;
	case PID:
		usePIDController(conf.controllerConfig.PIDConfig);
		break;
	}
	writeControllerConfigToEEPROM(controllerConfig);
}

struct ControllerConfig getSelectedControllerConfig() {
	return controllerConfig;
}

/* definitions helpers */
static void useNoController() {
	controllerConfig.controllerType = NONE;
	
}

static void useTwoStateController(struct TwoStateConfig controlConf) {
	controllerConfig.controllerType = TWO_STATE;
	controllerConfig.controllerConfig.twoStateConfig = controlConf;
}

static void usePIDController(struct PIDConfig controlConf) {
	controllerConfig.controllerType = PID;
	controllerConfig.controllerConfig.PIDConfig = controlConf;
}