#pragma once

#include <stdbool.h>
#include <stdint.h>

/*
* @brief Contains four bytes of IPv4 address.
*/
struct IPAddress {
	uint8_t octets[4];
};

/*
* @brief Describes, which diode out of three is currently set (SP, KNOB or IP diodes)
*/
enum CurrentDisplayState {
	SETPOINT_VALUE,
	KNOB_VALUE,
	IP_ADDRESS
};

/*
* @brief Set GLOBAL IP Address.
*
* @param addr Address structure to be set.
*/
void GLOBAL_setIPAddress(struct IPAddress addr);

/*
* @brief get GLOBAL IP Address.
*
* @return GLOBAL IP Address.
*/
struct IPAddress GLOBAL_getIPAddress();

/*
* @brief Set GLOBAL display state.
*
* @param state Display's state.
*/
void GLOBAL_setDisplayState(enum CurrentDisplayState state);

/*
* @brief get GLOBAL display state.
*
* @return GLOBAL display state.
*/
enum CurrentDisplayState GLOBAL_getDisplayState();

/*
* @brief Set GLOBAL periph error flag.
*
* @param state State of periph error flag.
*/
void GLOBAL_setPeriphErrorFlag(bool state);

/*
* @brief get GLOBAL periph error flag.
*
* @return GLOBAL periph error flag.
*/
bool GLOBAL_getPeriphErrorFlag();

/*
* @brief Set GLOBAL network error flag.
*
* @param state State of network error flag.
*/
void GLOBAL_setNetworkErrorFlag(bool state);

/*
* @brief get GLOBAL network error flag.
*
* @return GLOBAL network error flag.
*/
bool GLOBAL_getNetworkErrorFlag();

/*
* @brief Set GLOBAL MQTT error flag.
*
* @param state State of MQTT error flag.
*/
void GLOBAL_setMQTTErrorFlag(bool state);

/*
* @brief get GLOBAL MQTT error flag.
*
* @return GLOBAL MQTT error flag.
*/
bool GLOBAL_getMQTTErrorFlag();

/*
* @brief Set GLOBAL setpoint value.
*
* @param val setpoint value.
*/
void GLOBAL_setSetpointValue(double val);

/*
* @brief get GLOBAL setpoint value.
*
* @return GLOBAL setpoint value.
*/
double GLOBAL_getSetpointValue();

/*
* @brief Set GLOBAL process value.
*
* @param val process value.
*/
void GLOBAL_setProcessValue(double val);

/*
* @brief get GLOBAL process value.
*
* @return GLOBAL process value.
*/
double GLOBAL_getProcessValue();

/*
* @brief Set GLOBAL control value.
*
* @param val control value.
*/
void GLOBAL_setControlValue(double val);

/*
* @brief get GLOBAL control value.
*
* @return GLOBAL control value.
*/
double GLOBAL_getControlValue();

/*
* @brief Set GLOBAL knob value.
*
* @param val knob value.
*/
void GLOBAL_setKnobValueInInputPeriphRange(double val);

/*
* @brief Set GLOBAL knob value.
*
* @param val knob value.
*/
double GLOBAL_getKnobValueInInputPeriphRange();