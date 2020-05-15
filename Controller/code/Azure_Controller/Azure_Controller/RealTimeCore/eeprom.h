#pragma once

#include "config.h"

void initEEPROMCommunication();
bool isEEPROMInitialized();
struct InputPeriphConfig loadInputConfigFromEEPROM();
struct OutputPeriphConfig loadOutputConfigFromEEPROM();
struct ControllerConfig loadControllerConfigFromEEPROM();
void setEEPROMInitializedBit();
void writeInputConfigToEEPROM(struct InputPeriphConfig inputConf);
void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf);
void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig);
