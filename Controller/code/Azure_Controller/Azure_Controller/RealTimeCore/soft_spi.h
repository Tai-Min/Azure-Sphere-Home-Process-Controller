#pragma once

#include <stdint.h>

/* Poor man's software SPI */
/* As hardware SPI has conflict with board's wifi for some reason */

/*
* @brief Init software SPI.
*/
void SPIInit();

/*
* @brief Perform simple transfer on software SPI.
*
* @param num Number of bytes to transfer.
* @param rx Receive buffer.
* @param tx Trasnfer buffer.
* @param cs Chip select 0 or 1.
*/
void SPITransfer(int num, uint8_t* rx, uint8_t* tx, uint8_t cs);