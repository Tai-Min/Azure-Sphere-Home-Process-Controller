#include "global_state.h"

static struct GlobalState {
	struct IPAddress ipAddress;
	enum CurrentDisplayState displayState;
	bool isPeriphError;
	bool isNetworkError;
	bool isMQTTError;
	double setpointValue;
	double processValue;
	double controlValue;
	double knobValueInInputPeriphRange;
};

static struct GlobalState globalState = {
	.ipAddress = {0,0,0,0},
	.displayState = SETPOINT_VALUE,
	.isPeriphError = false,
	.isNetworkError = false,
	.isMQTTError = false,
	.setpointValue = 0,
	.processValue = 0,
	.controlValue = 0,
	.knobValueInInputPeriphRange = 0,
};

/* external functions */
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

/* definitions external functions */
void GLOBAL_setIPAddress(struct IPAddress addr) { globalState.ipAddress = addr; }
struct IPAddress GLOBAL_getIPAddress() { return globalState.ipAddress; };

void GLOBAL_setDisplayState(enum CurrentDisplayState state) { globalState.displayState = state; }
enum CurrentDisplayState GLOBAL_getDisplayState() { return globalState.displayState; };

void GLOBAL_setPeriphErrorFlag(bool state) { globalState.isPeriphError = state; }
bool GLOBAL_getPeriphErrorFlag() { return globalState.isPeriphError; }

void GLOBAL_setNetworkErrorFlag(bool state) { globalState.isNetworkError = state; }
bool GLOBAL_getNetworkErrorFlag() { return globalState.isNetworkError; }

void GLOBAL_setMQTTErrorFlag(bool state) { globalState.isMQTTError = state; }
bool GLOBAL_getMQTTErrorFlag() { return globalState.isMQTTError; }

void GLOBAL_setSetpointValue(double val) { globalState.setpointValue = val; }
double GLOBAL_getSetpointValue() { return globalState.setpointValue; }

void GLOBAL_setProcessValue(double val) { globalState.processValue = val; }
double GLOBAL_getProcessValue() { return  globalState.processValue; }

void GLOBAL_setControlValue(double val) { globalState.controlValue = val; }
double GLOBAL_getControlValue() { return globalState.controlValue; }

void GLOBAL_setKnobValueInInputPeriphRange(double val) { globalState.knobValueInInputPeriphRange = val; }
double GLOBAL_getKnobValueInInputPeriphRange() { return globalState.knobValueInInputPeriphRange; }