#include "FreeRTOS.h"
#include "task.h"

#include "os_hal_gpio.h"

#include "buttons.h"
#include "common.h"
#include "global_state.h"

static const os_hal_gpio_pin spButtonGpio = OS_HAL_GPIO_0;
static const os_hal_gpio_pin ipButtonGpio = OS_HAL_GPIO_2;

void buttonTask(void* pParams);

void buttonTask(void* pParams) {
	mtk_os_hal_gpio_request(spButtonGpio);
	mtk_os_hal_gpio_request(ipButtonGpio);
	mtk_os_hal_gpio_set_direction(spButtonGpio, OS_HAL_GPIO_DIR_INPUT);
	mtk_os_hal_gpio_set_direction(ipButtonGpio, OS_HAL_GPIO_DIR_INPUT);

	while (1) {
		os_hal_gpio_data spButtonState = 0;

		mtk_os_hal_gpio_get_input(spButtonGpio, &spButtonState);
		spButtonState = inverseLogic(spButtonState);

		if (spButtonState && GLOBAL_getDisplayState() == KNOB_VALUE) {
			GLOBAL_setSetpointValue(GLOBAL_getKnobValueInInputPeriphRange());
			GLOBAL_setDisplayState(SETPOINT_VALUE);
		}

		os_hal_gpio_data ipButtonState = 0;

		mtk_os_hal_gpio_get_input(ipButtonGpio, &ipButtonState);
		ipButtonState = inverseLogic(ipButtonState);

		if (ipButtonState) {
			GLOBAL_setDisplayState(IP_ADDRESS);
		}

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}
