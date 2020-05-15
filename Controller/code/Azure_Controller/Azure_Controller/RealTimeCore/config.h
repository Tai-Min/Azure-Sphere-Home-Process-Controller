#pragma once

#include <stdint.h>
#include <stdbool.h>

enum ControllerType {
	NONE = 0,
	TWO_STATE,
	PID
};

struct InputPeriphConfig {
	double inputMinValue;
	double inputMaxValue;
};

struct OutputPeriphConfig {
	double outputMinValue;
	double outputMaxValue;
};

struct TwoStateConfig {
	double offValue;
	double onValue;
	double bottomSwitchBoundary;
	double topSwitchBoundary;
};

struct PIDConfig {
	bool autotune;
	float kp;
	float ki;
	float kd;
	double saturarionUpper;
	double saturationLower;
	double deadzoneValue;
};

union Controllers {
	struct TwoStateConfig twoStateConfig;
	struct PIDConfig PIDConfig;
};

struct ControllerConfig {
	enum ControllerType controllerType;
	union Controllers controllerConfig;
};

void initConfig();

void setInputPeriphConfig(uint8_t index, struct InputPeriphConfig inputConf);
struct InputPeriphConfig getInputPeriphConfig();

void setOutputPeriphConfig(uint8_t index, struct OutputPeriphConfig outputConf);
struct OutputPeriphConfig getOutputPeriphConfig();

struct ControllerConfig getSelectedControllerConfig();

void setUsedController(struct ControllerConfig conf);