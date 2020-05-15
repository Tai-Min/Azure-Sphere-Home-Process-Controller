#pragma once

#include <stdint.h>

void PERIPH_initPeriphCommunication();
bool PERIPH_isPeriphConnectionError();

bool PERIPH_readPeriphInput(uint16_t *input, uint8_t inputIndex);
bool PERIPH_writePeriphOutput(uint16_t output, uint8_t outputIndex);