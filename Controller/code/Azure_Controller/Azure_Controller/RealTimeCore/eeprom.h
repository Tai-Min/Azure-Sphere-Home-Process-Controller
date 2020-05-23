#pragma once

/*
* @brief Init i2c periph.
*/
void initEEPROMCommunication();

/*
* @brief Returns true if EEPROM has been written to at least once in it's lifetime.
*
* @return True if EEPROM has been written to at least once in it's lifetime.
*/
bool isEEPROMInitialized();

/*
* @brief Load config of input peripheral board from EEPROM.
* 
* @return Loaded config.
*/
struct InputPeriphConfig loadInputConfigFromEEPROM();

/*
* @brief Load config of output peripheral board from EEPROM.
*
* @return Loaded config.
*/
struct OutputPeriphConfig loadOutputConfigFromEEPROM();

/*
* @brief Load config of the controller from EEPROM.
*
* @return Loaded config.
*/
struct ControllerConfig loadControllerConfigFromEEPROM();

/*
* @brief Load MQTT config from EEPROM.
*
* @return Loaded config.
*/
struct MQTTConfig loadMQTTConfigFromEEPROM();

/*
* @brief Sets USAGE_BIT inside EEPROM memory to 1.
*/
void setEEPROMInitializedBit();

/*
* @brief Write given input periph config to EEPROM.
*
* @param Config to be written.
*/
void writeInputConfigToEEPROM(struct InputPeriphConfig inputConf);

/*
* @brief Write given output periph config to EEPROM.
*
* @param Config to be written.
*/
void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf);

/*
* @brief Write given controller config to EEPROM.
*
* @param Config to be written.
*/
void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig);

/*
* @brief Write given MQTT config to EEPROM.
*
* @param Config to be written.
*/
void writeMQTTConfigToEEPROM(struct MQTTConfig config);
