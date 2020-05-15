#pragma once

#include <stdbool.h>
#include <stdint.h>

struct IPAddress {
	uint8_t octets[4];
};

enum CurrentDisplayState {
	SETPOINT_VALUE,
	KNOB_VALUE,
	IP_ADDRESS
};

void GLOBAL_setIPAddress(struct IPAddress addr);
struct IPAddress GLOBAL_getIPAddress();

void GLOBAL_setDisplayState(enum CurrentDisplayState state);
enum CurrentDisplayState GLOBAL_getDisplayState();

void GLOBAL_setPeriphErrorFlag(bool state);
bool GLOBAL_getPeriphErrorFlag();

void GLOBAL_setNetworkErrorFlag(bool state);
bool GLOBAL_getNetworkErrorFlag();

void GLOBAL_setMQTTErrorFlag(bool state);
bool GLOBAL_getMQTTErrorFlag();

void GLOBAL_setSetpointValue(double val);
double GLOBAL_getSetpointValue();

void GLOBAL_setProcessValue(double val);
double GLOBAL_getProcessValue();

void GLOBAL_setControlValue(double val);
double GLOBAL_getControlValue();

void GLOBAL_setKnobValueInInputPeriphRange(double val);
double GLOBAL_getKnobValueInInputPeriphRange();