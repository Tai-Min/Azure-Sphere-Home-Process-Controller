#pragma once

#include <stdint.h>

/*
* @brief Init pseudo SPI GPIO
*/
void PERIPH_initPeriphCommunication();

/*
* @brief Check whether communication failed with periph for n times in a row.
*
* @return True if communication failed.
*/
bool PERIPH_isPeriphConnectionError();

/*
* @brief Get data from input periph board (left side of the controller looking from front).
*
* @param input Value where received input should be written to.
* @param inputIndex Index of the input inside input periph board, unused in this project.
* @return True if communication succeeded.
*/
bool PERIPH_readPeriphInput(uint16_t* input, uint8_t inputIndex);

/*
* @brief Write data to output periph board (right side of the controller looking from front).
*
* @param output Value to write.
* @param inputIndex Index of the output inside output periph board, unused in this project.
* @return True if communication succeeded.
*/
bool PERIPH_writePeriphOutput(uint16_t output, uint8_t outputIndex);