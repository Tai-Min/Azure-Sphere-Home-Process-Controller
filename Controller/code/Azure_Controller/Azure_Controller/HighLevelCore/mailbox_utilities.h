#pragma once
#include "network_utilities.h"
#include "config_structs.h"

/**
* @brief Initialize communication with real time capable core.
*
* @return 0 on success, -1 on failure.
*/
int mailboxInit();

/**
* @brief Stop communication with real time capable core.
*/
void mailboxStop();

/**
* @brief Load MQTT config from EEPROM via rt core.
*
* @return addr Config from EEPROM.
*/
struct MQTTConfig mqttConfigLoad();

/**
* @brief Send MQTT config to real time sapable core so it can be saved in EEPROM.
*
* @param conf Config to be sent.
*/
void mqttConfigSave(struct MQTTConfig conf);

/**
* @brief Send IP address to real time sapable core so it can display it on 7 segment display.
*
* @param addr Address to be sent.
*/
void ipAddressKeep(struct IPAddress addr);

/**
* @brief Tell rt core to set network error led.
*/
void networkErrorSet();

/**
* @brief Tell rt core to clear network error led.
*/
void networkErrorClear();

/**
* @brief Tell rt core to set MQTT error led.
*/
void mqttErrorSet();

/**
* @brief Tell rt core to clear MQTT error led.
*/
void mqttErrorClear();

/**
* @brief Request setpoint and process value from rt core.
* @param sp Pointer to setpoint double.
* @param pv Pointer to process value double.
*/
void getSpPv(double* sp, double* pv);

/**
* @brief Set setpoint in rt core.
*
* @param sp Setpoint to be set.
*/
void setSp(double sp);

/**
* @brief Load Control config from EEPROM via rt core.
*
* @param iconf Pointer to input configuration structure to which config should be loaded.
* @param oconf Pointer to output configuration structure to which config should be loaded.
* @param cconf Pointer to controller configuration structure to which config should be loaded.
*/
void controlConfigLoad(struct InputPeriphConfig* iconf, struct OutputPeriphConfig* oconf, struct ControllerConfig* cconf);

/**
* @brief Send control config to real time sapable core so it can be updated in rt core and saved in EEPROM.
* @param iconf Input config to save.
* @param oconf Output config to save.
* @param cconf Controller config to save.
*/
void controlConfigSave(struct InputPeriphConfig iconf, struct OutputPeriphConfig oconf, struct ControllerConfig cconf);