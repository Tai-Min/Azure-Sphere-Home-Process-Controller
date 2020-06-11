#pragma once

/**
* @brief MQTT config structure.
*/
struct MQTTConfig {
	char setpointTopic[11];
	char processValueTopic[11];
	char brokerIPAddress[16];
};

/**
* @brief Available controllers.
*/
enum ControllerType {
	NONE = 0,
	TWO_STATE,
	PID
};

/**
* @brief Config structure of input peripheral.
*/
struct InputPeriphConfig {
	double inputMinValue;
	double inputMaxValue;
};

/**
* @brief Config structure of output peripheral.
*/
struct OutputPeriphConfig {
	double outputMinValue;
	double outputMaxValue;
};

/**
* @brief Config structure of two state controller.
*/
struct TwoStateConfig {
	double offValue;
	double onValue;
	double bottomSwitchBoundary;
	double topSwitchBoundary;
};

/**
* @brief Config structure of PID controller.
*/
struct PIDConfig {
	double kp;
	double ki;
	double kd;
	double saturarionUpper;
	double saturationLower;
};

/**
* @brief Union that holds config of every available controller.
*/
union ControllerConfigUnion {
	struct TwoStateConfig twoStateConfig;
	struct PIDConfig PIDConfig;
};

/**
* @brief Universal controller config structure.
*/
struct ControllerConfig {
	enum ControllerType controllerType;
	union ControllerConfigUnion controllerConfig;
};