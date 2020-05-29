#include <stdlib.h>

#include "FreeRTOS.h"

#include "os_hal_gpio.h"

#include "control.h"
#include "periph.h"
#include "common.h"
#include "config.h"
#include "global_state.h"

#define CONTROL_LOOP_SAMPLING_TIME 50

/*
* @brief Internal state of two state controller.
*/
struct TwoStateInternalState {
	bool isOn;
};

/*
* @brief Internal state of pid controller.
*/
struct PIDInternalState {
	double integratorValue;
	double previousError;
};

/*
* @brief Union that holds internal state of every available controller.
*/
static union ControllerInternalState {
	struct TwoStateInternalState twoState;
	struct PIDInternalState pid;
};

static union ControllerInternalState controller;

/* external functions */
void CONTROL_controlTask(void* pParams);

/* helpers */
/*
* @brief Reads value from input peripheral board, maps it to boundary defined in input periph config and writes it to GLOBAL structure as process value.
*/
static uint8_t readProcessValue();

/*
* @brief Writes control value from GLOBAL structure mapped from output periph's config to uint16_t range into output peripheral board. 
*/
static uint8_t writeControlValue();

/*
* @brief Classic two state control with hysteresis and writes result to GLOBAL structure as control value.
*/
static void performTwoStateControl();

/*
* @brief PID control with integral anti-windup. Writes result into GLOBAL structure as control value.
*/
static void performPIDControl();

/*
* @brief Perform selected control algorithm.
*/
static void performControlAlgorithm();

/* definitions external functions */
void CONTROL_controlTask(void* pParams) {
	// just in case as mailbox needs access to config too
	if(!isConfigInit())
		initConfig();

	PERIPH_initPeriphCommunication();

	while (1) {
		unsigned long long start = millis();
		readProcessValue();

		performControlAlgorithm();

		writeControlValue();

		int passed = millis() - start;
		// just in case if reading/writting takes too long for some reason
		if (passed <= CONTROL_LOOP_SAMPLING_TIME) {
			vTaskDelay(pdMS_TO_TICKS(CONTROL_LOOP_SAMPLING_TIME - passed));
		}
	}
}

/* definitions helpers */
static uint8_t readProcessValue() {
	bool res = false;
	uint16_t value;

	// do until read succeeded or periph failure 
	while (!res && !PERIPH_isPeriphConnectionError()) {
		res = PERIPH_readPeriphInput(&value, 0);
		//vTaskDelay(pdMS_TO_TICKS(1));// needed sometimes for "SPI" peripheral
	}
	GLOBAL_setProcessValue(map(value, 0, 65535, getInputPeriphConfig().inputMinValue, getInputPeriphConfig().inputMaxValue));
	printf("GLOBAL PV: %f\n", GLOBAL_getProcessValue());
	return 0;
}

static uint8_t writeControlValue() {
	double value = map(GLOBAL_getControlValue(), getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMaxValue, 0, 65535);
	double minValue = map(getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMinValue, getOutputPeriphConfig().outputMaxValue, 0, 65535);

	bool res = false;

	// until succeeds
	while (!res) {
		// in case of failure try to write minimum output's value into board
		if (PERIPH_isPeriphConnectionError()) {
			res = true;// pseudo success
			PERIPH_writePeriphOutput(minValue, 0);
			GLOBAL_setPeriphErrorFlag(true);
		}
		else {
			res = PERIPH_writePeriphOutput(value, 0);
		}
		//vTaskDelay(pdMS_TO_TICKS(1));
	}

	return 0;
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
	static long long previousTime = 0;

	long long passed = millis() - previousTime;
	if (passed == 0) {
		return;
	}

	struct PIDConfig conf = getSelectedControllerConfig().controllerConfig.PIDConfig;
	double error = GLOBAL_getSetpointValue() - GLOBAL_getProcessValue();
	controller.pid.integratorValue = controller.pid.integratorValue + error * (double)(passed / (double)1000);
	double derivative = (error - controller.pid.previousError) / (double)(passed / (double)1000);
	double controlVal = conf.kp * error + conf.ki * controller.pid.integratorValue + conf.kd * derivative;

	// anti windup
	if (controlVal > conf.saturarionUpper) {
		if(error > 0 && controlVal > 0 || error < 0 && controlVal < 0)
			controller.pid.integratorValue = controller.pid.integratorValue - error * (double)(passed / (double)1000);

		controlVal = conf.saturarionUpper;
	}
	else if (controlVal < conf.saturationLower) {
		if (error > 0 && controlVal > 0 || error < 0 && controlVal < 0)
			controller.pid.integratorValue = controller.pid.integratorValue - error * (double)(passed / (double)1000);

		controlVal = conf.saturationLower;
	}

	GLOBAL_setControlValue(controlVal);

	controller.pid.previousError = error;
	previousTime = previousTime + passed;
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

