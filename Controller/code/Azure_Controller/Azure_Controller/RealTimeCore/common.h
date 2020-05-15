#pragma once

#include "os_hal_gpio.h"

double map(double x, double in_min, double in_max, double out_min, double out_max);
long long millis();
os_hal_gpio_data inverseLogic(os_hal_gpio_data logicState);