#pragma once

#include <stdbool.h>

/**
* @brief Get number of milliseconds passed since start of the program.
*
* @return Number of milliseconds passed since start of the program.
*/
unsigned long millis();

/**
* @brief Stop current thread for given number of milliseconds.
*
* @param t How many milliseconds to wait.
*/
void waitMs(unsigned long t);