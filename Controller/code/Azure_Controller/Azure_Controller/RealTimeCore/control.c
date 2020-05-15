#include <stdlib.h>

#include "FreeRTOS.h"

#include "os_hal_gpio.h"

#include "control.h"
#include "periph.h"
#include "common.h"
#include "config.h"
#include "global_state.h"

#define CONTROL_LOOP_SAMPLING_TIME 50

struct TwoState {
	bool isOn;
};

struct PID {
	double integratorValue;
	double previousError;
};

union Controller {
	struct TwoState twoState;
	struct PID pid;
};

static union Controller controller;

/* external functions */
void CONTROL_controlTask();

/* helpers */
static uint8_t readProcessValue();
static uint8_t writeControlValue();
static void performTwoStateControl();
static void performPIDControl();
static void performControlAlgorithm();

/* declarations */
void CONTROL_controlTask() {
	printf("INPUT struct size %d\n", sizeof(struct InputPeriphConfig));
	printf("OUTPUT struct size %d\n", sizeof(struct OutputPeriphConfig));
	printf("CONTROLLER union size %d\n", sizeof(struct ControllerConfig));

	initConfig();
	vTaskDelay(pdMS_TO_TICKS(10));
	PERIPH_initPeriphCommunication();

	while (1) {
		long long start = millis();
		uint8_t transactionTime = readProcessValue();
		performControlAlgorithm();
		transactionTime += writeControlValue();
		long long passed = millis() - start + transactionTime;
		if (passed <= CONTROL_LOOP_SAMPLING_TIME) {
			vTaskDelay(pdMS_TO_TICKS(CONTROL_LOOP_SAMPLING_TIME - passed));
		}
	}
}

/**************************************************************/

static uint8_t readProcessValue() {
	uint8_t looped = 0;
	bool res = false;
	uint16_t value;
	while (!res && !PERIPH_isPeriphConnectionError()) {
		res = PERIPH_readPeriphInput(&value, 0);
		vTaskDelay(pdMS_TO_TICKS(3));
		looped += 3;
	}
	GLOBAL_setProcessValue(map(value, 0, sizeof(uint16_t)-1, getInputPeriphConfig().inputMinValue, getInputPeriphConfig().inputMaxValue));
	return looped;
}

static uint8_t writeControlValue() {
	double value = map(GLOBAL_getControlValue(), getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMaxValue, 0, sizeof(uint16_t) - 1);
	double minValue = map(getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMaxValue, 0, sizeof(uint16_t) - 1);

	uint8_t looped = 0;
	bool res = false;
	while (!res) {
		if (PERIPH_isPeriphConnectionError()) {
			res = true;
			PERIPH_writePeriphOutput(minValue, 0);
			GLOBAL_setPeriphErrorFlag(true);
		}
		else {
			res = PERIPH_writePeriphOutput(value, 0);
		}
		vTaskDelay(pdMS_TO_TICKS(3));
		looped += 3;
	}
	return looped;
}

static void performTwoStateControl() {
	double error = GLOBAL_getSetpointValue() - GLOBAL_getProcessValue();
	if (error >= getSelectedControllerConfig().controllerConfig.twoStateConfig.topSwitchBoundary)
		controller.twoState.isOn = true;
	else if (error <= getSelectedControllerConfig().controllerConfig.twoStateConfig.bottomSwitchBoundary)
		controller.twoState.isOn = false;

	if (controller.twoState.isOn)
		GLOBAL_setControlValue(getSelectedControllerConfig().controllerConfig.twoStateConfig.onValue);
	else
		GLOBAL_setControlValue(getSelectedControllerConfig().controllerConfig.twoStateConfig.offValue);
}

static void performPIDControl() {
	struct PIDConfig conf = getSelectedControllerConfig().controllerConfig.PIDConfig;
	double error = GLOBAL_getSetpointValue() - GLOBAL_getProcessValue();
	controller.pid.integratorValue = controller.pid.integratorValue + error * (double)(CONTROL_LOOP_SAMPLING_TIME / (double)1000);
	double derivative = (error - controller.pid.previousError) / (double)(CONTROL_LOOP_SAMPLING_TIME / (double)1000);
	double controlVal = conf.kp * error + conf.ki * controller.pid.integratorValue + conf.kd * derivative;

	if (controlVal > conf.saturarionUpper) {
		if(error > 0 && controlVal > 0 || error < 0 && controlVal < 0)
			controller.pid.integratorValue = controller.pid.integratorValue - error * (double)(CONTROL_LOOP_SAMPLING_TIME / (double)1000);

		controlVal = conf.saturarionUpper;
	}
	else if (controlVal < conf.saturationLower) {
		if (error > 0 && controlVal > 0 || error < 0 && controlVal < 0)
			controller.pid.integratorValue = controller.pid.integratorValue - error * (double)(CONTROL_LOOP_SAMPLING_TIME / (double)1000);

		controlVal = conf.saturationLower;
	}

	GLOBAL_setControlValue(controlVal);

	controller.pid.previousError = error;
}

static void performControlAlgorithm() {
	switch (getSelectedControllerConfig().controllerType) {
	case NONE:
		break;
	case TWO_STATE:
		performTwoStateControl();
		break;
	case PID:
		performPIDControl();
		break;
	}
}

