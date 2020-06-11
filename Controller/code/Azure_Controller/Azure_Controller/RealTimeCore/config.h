#pragma once

#include <stdint.h>
#include <stdbool.h>

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

/**
* @brief Init EEPROM and load config from it. If usage bit in EEPROM is not set then write default config and set this bit.
*/
void initConfig();

/**
* @brief Returns true if initConfig() has been called before.
*
* @return True if initConfig() has been called before.
*/
bool isConfigInit();

/**
* @brief Write MQTT config to EEPROM.
*
* @param conf Config to write.
*/

void setMQTTConfig(struct MQTTConfig conf);
/**
* @brief Read MQTT config from EEPROM.
*
* @return MQTT config stored in EEPROM.
*/
struct MQTTConfig getMQTTConfig();

/**
* @brief Write config of input peripheral to EEPROM.
*
* @param conf Config to write.
*/

void setInputPeriphConfig(struct InputPeriphConfig inputConf);
/**
* @brief Read config of input peripheral from EEPROM.
*
* @return Config of input peripheral stored in EEPROM.
*/
struct InputPeriphConfig getInputPeriphConfig();

/**
* @brief Write config of output peripheral to EEPROM.
*
* @param conf Config to write.
*/
void setOutputPeriphConfig(struct OutputPeriphConfig outputConf);

/**
* @brief Read config of output peripheral from EEPROM.
*
* @return Config of output peripheral stored in EEPROM.
*/
struct OutputPeriphConfig getOutputPeriphConfig();

/**
* @brief Write config of selected controller to EEPROM.
*
* @param conf Config to write.
*/
void setUsedController(struct ControllerConfig conf);

/**
* @brief Read controller's along with it's type from EEPROM.
*
* @return Config of the controller stored in EEPROM.
*/
struct ControllerConfig getSelectedControllerConfig();
