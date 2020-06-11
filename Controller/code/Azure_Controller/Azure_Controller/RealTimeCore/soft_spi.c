#include "soft_spi.h"
#include "os_hal_gpio.h"

static const os_hal_gpio_pin misoGpio = OS_HAL_GPIO_33;
static const os_hal_gpio_pin mosiGpio = OS_HAL_GPIO_32;
static const os_hal_gpio_pin sckGpio = OS_HAL_GPIO_31;
static const os_hal_gpio_pin cs0Gpio = OS_HAL_GPIO_34;
static const os_hal_gpio_pin cs1Gpio = OS_HAL_GPIO_35;

/* external functions */
void SPIInit();
void SPITransfer(int num, uint8_t* rx, uint8_t* tx, uint8_t cs);

/* helpers */
/**
* @brief Transfer one byte through software SPI
*
* @param toSlave Byte that will be transferred to slave device.
* @return Value received from slave device.
*/
uint8_t transferByte(uint8_t toSlave);

/* definitions external functions */
void SPIInit() {
	mtk_os_hal_gpio_request(misoGpio);
	mtk_os_hal_gpio_request(mosiGpio);
	mtk_os_hal_gpio_request(sckGpio);
	mtk_os_hal_gpio_request(cs0Gpio);
	mtk_os_hal_gpio_request(cs1Gpio);

	mtk_os_hal_gpio_set_direction(misoGpio, OS_HAL_GPIO_DIR_INPUT);
	mtk_os_hal_gpio_set_direction(mosiGpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(sckGpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(cs0Gpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(cs1Gpio, OS_HAL_GPIO_DIR_OUTPUT);

	mtk_os_hal_gpio_set_output(mosiGpio, 0);
	mtk_os_hal_gpio_set_output(sckGpio, 0);
	mtk_os_hal_gpio_set_output(cs0Gpio, 1);
	mtk_os_hal_gpio_set_output(cs1Gpio, 1);
}

void SPITransfer(int num, uint8_t* rx, uint8_t* tx, uint8_t cs) {
	if (cs == 0) mtk_os_hal_gpio_set_output(cs0Gpio, OS_HAL_GPIO_DATA_LOW);
	else mtk_os_hal_gpio_set_output(cs1Gpio, OS_HAL_GPIO_DATA_LOW);

	for (uint8_t i = 0; i < num; i++) {
		rx[i] = transferByte(tx[i]);
	}

	if (cs == 0) mtk_os_hal_gpio_set_output(cs0Gpio, OS_HAL_GPIO_DATA_HIGH);
	else mtk_os_hal_gpio_set_output(cs1Gpio, OS_HAL_GPIO_DATA_HIGH);
}

/* definitions helpers */

uint8_t transferByte(uint8_t toSlave) {
	uint8_t fromSlave = 0;

	for (int8_t i = 7; i >= 0; i--) {
		if (toSlave & (1 << i)) mtk_os_hal_gpio_set_output(mosiGpio, OS_HAL_GPIO_DATA_HIGH);
		else mtk_os_hal_gpio_set_output(mosiGpio, OS_HAL_GPIO_DATA_LOW);

		mtk_os_hal_gpio_set_output(sckGpio, OS_HAL_GPIO_DATA_HIGH);

		os_hal_gpio_data in;
		mtk_os_hal_gpio_get_input(misoGpio,&in);
		fromSlave |= (in << i);

		mtk_os_hal_gpio_set_output(sckGpio, OS_HAL_GPIO_DATA_LOW);
	}

	return fromSlave;
}
