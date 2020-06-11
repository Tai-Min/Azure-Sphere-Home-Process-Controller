#include "FreeRTOS.h"
#include "task.h"
#include "os_hal_gpio.h"

#include "buttons.h"
#include "global_state.h"

static const os_hal_gpio_pin spButtonGpio = OS_HAL_GPIO_0;
static const os_hal_gpio_pin ipButtonGpio = OS_HAL_GPIO_2;

/* external functions */
void buttonTask(void* pParams);

/* helpers */
/**
* @brief Check SP button's state and on press, if knob led is set then change global setpoint to knob's value and set setpoint led.
*/
void processSetpointApplyButton();

/**
* @brief Check IP button's state and on press set ip led.
*/
void processIPButton();

/* definitions external functions */
void buttonTask(void* pParams) {
	mtk_os_hal_gpio_request(spButtonGpio);
	mtk_os_hal_gpio_request(ipButtonGpio);
	mtk_os_hal_gpio_set_direction(spButtonGpio, OS_HAL_GPIO_DIR_INPUT);
	mtk_os_hal_gpio_set_direction(ipButtonGpio, OS_HAL_GPIO_DIR_INPUT);

	while (1) {
		processSetpointApplyButton();
		processIPButton();
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

/* definitions helpers */
void processSetpointApplyButton() {
	os_hal_gpio_data spButtonState = 0;

	mtk_os_hal_gpio_get_input(spButtonGpio, &spButtonState);
	spButtonState = inverseLogic(spButtonState);

	if (spButtonState && GLOBAL_getDisplayState() == KNOB_VALUE) {
		GLOBAL_setSetpointValue(GLOBAL_getKnobValueInInputPeriphRange());
		GLOBAL_setDisplayState(SETPOINT_VALUE);
	}
}

void processIPButton() {
	os_hal_gpio_data ipButtonState = 0;

	mtk_os_hal_gpio_get_input(ipButtonGpio, &ipButtonState);
	ipButtonState = inverseLogic(ipButtonState);

	if (ipButtonState) {
		GLOBAL_setDisplayState(IP_ADDRESS);
	}
}

