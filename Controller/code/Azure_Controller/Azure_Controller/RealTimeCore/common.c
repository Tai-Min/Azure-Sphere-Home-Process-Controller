#include "common.h"
#include "FreeRTOS.h"
#include "task.h"

/* external functions */
double map(double x, double in_min, double in_max, double out_min, double out_max);
long long millis();
os_hal_gpio_data inverseLogic(os_hal_gpio_data logicState);

/* definitions external functions */
double map(double x, double in_min, double in_max, double out_min, double out_max) {
	if (in_max - in_min == 0)
		return 0;
	double val = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if (val > out_max)
		val = out_max;
	if (val < out_min)
		val = out_min;

	return val;
}

long long millis() {
	return xTaskGetTickCount();
}

os_hal_gpio_data inverseLogic(os_hal_gpio_data logicState) {
	return !logicState;
}