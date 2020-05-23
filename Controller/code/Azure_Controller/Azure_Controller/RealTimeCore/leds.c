#include <math.h>

#include "FreeRTOS.h"

#include "os_hal_gpio.h"
#include<stdlib.h>
#include "leds.h"
#include "config.h"
#include <stdbool.h>
#include "global_state.h"

#define SETPOINT_BIT_POSITION 15
#define CLOUD_ERROR_BIT_POSITION 14
#define KNOB_BIT_POSITION 13
#define MQTT_ERROR_BIT_POSITION 10
#define IP_BIT_POSITION 11
#define NETWORK_ERROR_BIT_POSITION 12

#define LED_BAR_SIGNED_DIODE_COUNT 5
#define LED_BAR_POSITIVE_DIODES_OFFSET LED_BAR_SIGNED_DIODE_COUNT
#define LED_BAR_SIGNED_NUM_STATES (LED_BAR_SIGNED_DIODE_COUNT + 1) //no diodes lit is also a state

static const os_hal_gpio_pin dataGpio = OS_HAL_GPIO_1;
static const os_hal_gpio_pin clockGpio = OS_HAL_GPIO_17;
static const os_hal_gpio_pin latchGpio = OS_HAL_GPIO_16;

static uint16_t ledRegisterData = 0;

/* external functions */
void LED_ledTask(void* pParams);

/* helpers */
/*
* @brief Set one of three indicator leds: SP, KNOB, IP. Clears other leds.
*
* @param led Led to be set.
*/
static void setCurrentIndicatorLed(enum CurrentDisplayState led);

/*
* @brief Set or clear setpoint indicator led.
* 
* @param state Whether turn in on or off.
*/
static void inline setSetpointLed(bool state);

/*
* @brief Set or clear knob indicator led.
*
* @param state Whether turn in on or off.
*/
static void inline setKnobLed(bool state);

/*
* @brief Set or clear IP address indicator led.
*
* @param state Whether turn in on or off.
*/
static void inline setIPAddressLed(bool state);

/*
* @brief Set or clear periph error indicator led.
*
* @param state Whether turn in on or off.
*/
static void setPeriphErrorLed(bool state);

/*
* @brief Set or clear MQTT error indicator led.
*
* @param state Whether turn in on or off.
*/
static void setMQTTErrorLed(bool state);

/*
* @brief Set or clear network error indicator led.
*
* @param state Whether turn in on or off.
*/
static void setNetworkErrorLed(bool state);

/*
* @brief Update led bar depending on control's error.
*/
static void setLedBar();

/*
* @brief Set number of led's in led bar depending on percentage value.
*
* @param _barValue Value between -100 and 100 where -100 means that bottom half of led bar is lit and 100 means that top half is lit.
*/
static void setLedBarValueInPercent(int8_t _barValue);

/*
* @brief Translate given input into uint16_t value where first ten bits describe state of the led bar. 6 next bits are set to 0.
*
* @return Led bar's state as bits.
*/
static uint16_t ledBarValueToRegisterBits(int8_t _barValue);

/*
* @brief Sets index number of bits in given uint16_t value depending on given index. 
*
* Sets bits in positive half of led bar so it appears like: 0 - no leds lit, 5 - all five positive led bars lit.
*
* @param _ledBarBits uint16_t where bits should be set.
* @param num number of bits to set.
*/
static void inline setLedBarPositiveLightBit(uint16_t* _ledBarBits, uint8_t num);

/*
* @brief Sets index number of bits in given uint16_t value depending on given index.
*
* Sets bits in negative half of led bar so it appears like: 0 - no leds lit, 5 - all five negative led bars lit.
*
* @param _ledBarBits uint16_t where bits should be set.
* @param num number of bits to set.
*/
static void inline setLedBarNegativeLightBit(uint16_t* _ledBarBits, uint8_t num);

/*
* @brief Write uint16_t word to physical registers.
*/
static void writeLedDataToRegisters();

/*
* @brief Perform fast clock tick on clock GPIO.
*/
static void oneRegisterClockTick();

/*
* @brief Clear latch GPIO.
*/
static void inline startRegisterDataTransfer();

/*
* @brief Set latch GPIO.
*/
static void inline finishRegisterDataTransfer();

/*
* @brief Returns state of bit in ledRegisterData.
* 
* @param index Index of bit between 0 - 15.
* @return State of the bit.
*/
static bool inline getRegisterBitState(uint8_t index);

/* definitions external functions */
void LED_ledTask(void* pParams) {
	mtk_os_hal_gpio_request(dataGpio);
	mtk_os_hal_gpio_request(latchGpio);
	mtk_os_hal_gpio_request(clockGpio);
	mtk_os_hal_gpio_set_direction(dataGpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(latchGpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(clockGpio, OS_HAL_GPIO_DIR_OUTPUT);

	while (1) {
		setCurrentIndicatorLed(GLOBAL_getDisplayState());

		setPeriphErrorLed(GLOBAL_getPeriphErrorFlag());
		setNetworkErrorLed(GLOBAL_getNetworkErrorFlag());
		setMQTTErrorLed(GLOBAL_getMQTTErrorFlag());

		setLedBar();

		writeLedDataToRegisters();

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

/* definitions helpers */
static void setCurrentIndicatorLed(enum CurrentDisplayState led) {
	switch (led) {
	case SETPOINT_VALUE:
		setSetpointLed(1);
		setKnobLed(0);
		setIPAddressLed(0);
		break;
	case KNOB_VALUE:
		setSetpointLed(0);
		setKnobLed(1);
		setIPAddressLed(0);
		break;
	case IP_ADDRESS:
		setSetpointLed(0);
		setKnobLed(0);
		setIPAddressLed(1);
		break;
	}
}

static void inline setSetpointLed(bool state) {
	if (state)
		ledRegisterData |= 1 << SETPOINT_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << SETPOINT_BIT_POSITION);
}

static void inline setKnobLed(bool state) {
	if (state)
		ledRegisterData |= 1 << KNOB_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << KNOB_BIT_POSITION);
}

static void inline setIPAddressLed(bool state) {
	if (state)
		ledRegisterData |= 1 << IP_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << IP_BIT_POSITION);
}

static void setPeriphErrorLed(bool state) {
	if (state)
		ledRegisterData |= 1 << CLOUD_ERROR_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << CLOUD_ERROR_BIT_POSITION);
}

static void setMQTTErrorLed(bool state) {
	if (state)
		ledRegisterData |= 1 << MQTT_ERROR_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << MQTT_ERROR_BIT_POSITION);
}

static void setNetworkErrorLed(bool state) {
	if (state)
		ledRegisterData |= 1 << NETWORK_ERROR_BIT_POSITION;
	else
		ledRegisterData &= ~(1 << NETWORK_ERROR_BIT_POSITION);
}

static void setLedBar() {
	struct InputPeriphConfig p = getInputPeriphConfig();
	double err = GLOBAL_getSetpointValue(); //- GLOBAL_getProcessValue();
	double den = fabs(p.inputMaxValue - p.inputMinValue);

	if (den == 0) {
		err = 1;
		den = 1;
	}

	setLedBarValueInPercent((err / den) * 100);
}

static void setLedBarValueInPercent(int8_t _barValue) {
	_barValue /= (100/LED_BAR_SIGNED_DIODE_COUNT);
	if (_barValue > LED_BAR_SIGNED_DIODE_COUNT)
		_barValue = LED_BAR_SIGNED_DIODE_COUNT;
	else if (_barValue < -LED_BAR_SIGNED_DIODE_COUNT)
		_barValue = -LED_BAR_SIGNED_DIODE_COUNT;

	uint16_t ledBarBits = ledBarValueToRegisterBits(_barValue);

	ledRegisterData = (ledRegisterData & 0b1111110000000000) | ledBarBits;
}

static uint16_t ledBarValueToRegisterBits(int8_t _barValue) {
	uint16_t ledBarBits = 0;

	for (uint8_t i = 0; i < abs(_barValue); i++) {
		if (_barValue >= 0)
			setLedBarPositiveLightBit(&ledBarBits, i);
		else
			setLedBarNegativeLightBit(&ledBarBits, i);
	}

	return (_barValue >= 0 ? ledBarBits << LED_BAR_POSITIVE_DIODES_OFFSET : ledBarBits);
}

static void inline setLedBarPositiveLightBit(uint16_t* _ledBarBits, uint8_t num) {
	*_ledBarBits |= 1 << num;
}

static void inline setLedBarNegativeLightBit(uint16_t* _ledBarBits, uint8_t num) {
	*_ledBarBits |= (1 << (LED_BAR_SIGNED_DIODE_COUNT - 1 - num));
}

static void writeLedDataToRegisters() {
	startRegisterDataTransfer();
	for (int i = 15; i >= 0; i--) {
		mtk_os_hal_gpio_set_output(dataGpio, getRegisterBitState(i));
		oneRegisterClockTick();
	}
	finishRegisterDataTransfer();
}

static void oneRegisterClockTick() {
	mtk_os_hal_gpio_set_output(clockGpio, OS_HAL_GPIO_DATA_HIGH);
	mtk_os_hal_gpio_set_output(clockGpio, OS_HAL_GPIO_DATA_LOW);
}

static void inline startRegisterDataTransfer() {
	mtk_os_hal_gpio_set_output(latchGpio, OS_HAL_GPIO_DATA_LOW);
}

static void inline finishRegisterDataTransfer() {
	mtk_os_hal_gpio_set_output(latchGpio, OS_HAL_GPIO_DATA_HIGH);
}

static bool inline getRegisterBitState(uint8_t index) {
	return (bool)(ledRegisterData & (1 << index));
}