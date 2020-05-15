#include "common.h"
#include "FreeRTOS.h"
#include "task.h"

double map(double x, double in_min, double in_max, double out_min, double out_max) {
	if (in_max - in_min == 0)
		return 0;
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long long millis() {
	return xTaskGetTickCount();
}

os_hal_gpio_data inverseLogic(os_hal_gpio_data logicState) {
	return !logicState;
}