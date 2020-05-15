#include <stdlib.h>
#include "FreeRTOS.h"
#include "printf.h"
#include "config.h"
#include "eeprom.h"

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

/* external functions */
void initConfig();

void setInputPeriphConfig(uint8_t index, struct InputPeriphConfig inputConf);
struct InputPeriphConfig getInputPeriphConfig();

void setOutputPeriphConfig(uint8_t index, struct OutputPeriphConfig outputConf);
struct OutputPeriphConfig getOutputPeriphConfig();

struct ControllerConfig getSelectedControllerConfig();

void setUsedController(struct ControllerConfig conf);

/* helpers */
static void useNoController();
static void useTwoStateController(struct TwoStateConfig controlConf);
static void usePIDController(struct PIDConfig controlConf);

/* definitions */
void initConfig() {
	initEEPROMCommunication();

	if (!isEEPROMInitialized()) {
		writeInputConfigToEEPROM(inputConfig);
		writeOutputConfigToEEPROM(outputConfig);
		writeControllerConfigToEEPROM(controllerConfig);
		setEEPROMInitializedBit();
	}
	
	inputConfig = loadInputConfigFromEEPROM();
	outputConfig = loadOutputConfigFromEEPROM();
	controllerConfig = loadControllerConfigFromEEPROM();
}

void setInputPeriphConfig(uint8_t index, struct InputPeriphConfig inputConf) {
	inputConfig = inputConf;
	writeInputConfigToEEPROM(inputConf);
}

struct InputPeriphConfig getInputPeriphConfig() {
	return inputConfig;
}

void setOutputPeriphConfig(uint8_t index, struct OutputPeriphConfig outputConf) {
	outputConfig = outputConf;
	writeOutputConfigToEEPROM(outputConf);
}

struct OutputPeriphConfig getOutputPeriphConfig() {
	return outputConfig;
}

struct ControllerConfig getSelectedControllerConfig() {
	return controllerConfig;
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
}

/**************************************************************/
static void useNoController() {
	controllerConfig.controllerType = NONE;
	writeControllerConfigToEEPROM(controllerConfig);
}

static void useTwoStateController(struct TwoStateConfig controlConf) {
	controllerConfig.controllerType = TWO_STATE;
	controllerConfig.controllerConfig.twoStateConfig = controlConf;
	writeControllerConfigToEEPROM( controllerConfig);
}

static void usePIDController(struct PIDConfig controlConf) {
	controllerConfig.controllerType = PID;
	controllerConfig.controllerConfig.PIDConfig = controlConf;
	writeControllerConfigToEEPROM(controllerConfig);
}