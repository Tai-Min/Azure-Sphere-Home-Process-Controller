#pragma once

#include "os_hal_gpio.h"

/*
* @brief Map x described by in_min, in_max space into space described by out_min, out_max.
*
* @param x value to be mapped.
* @param in_min minimum value that x can achieve.
* @param in_max maximum value that x can achieve.
* @param out_min minimum value that output should achieve.
* @param out_max maximum value that output should achieve.
* @return Mapped value.
*/
double map(double x, double in_min, double in_max, double out_min, double out_max);

/*
* @brief Return time in millisecond since scheduler has started.
*
* @return Time in millisecond since scheduler has started.
*/
long long millis();

/*
* @brief Invert logical state of gpio.
*
* @param logicState State of GPIO to be inverted.
* @return Logic state inverted.
*/
os_hal_gpio_data inverseLogic(os_hal_gpio_data logicState);