#include <math.h>

#include "FreeRTOS.h"
#include "os_hal_gpio.h"

#include "seg.h"
#include "global_state.h"

#define COMMAND_1 0b01000000
#define COMMAND_2 0b11000000
#define COMMAND_3 0b10001000

#define EMPTY_INDEX 10
#define MINUS_INDEX 11

static const uint8_t digits[] = {
	0b00111111, //0
	0b00110000, //1
	0b01011011, //2
	0b01111001, //3
	0b01110100, //4
	0b01101101, //5
	0b01101111, //6
	0b00111000, //7
	0b01111111, //8
	0b01111101, //9
	0b00000000, //empty
	0b01000000  //-
};

static const os_hal_gpio_pin clockGpio = OS_HAL_GPIO_28;
static const os_hal_gpio_pin dataGpio = OS_HAL_GPIO_26;
static int16_t displayedValue = 0;

/* external functions */
void SEG_displayTask(void* pParams);

/* helpers */
/**
* @brief Set value between -999 and 999 to be displayed on 7 segment display.
* 
* @param Value to be displayed.
*/
static void setDisplayedValue(int16_t _value);

/**
* @brief Display whole IP Address received from GLOBAL structure on display.
*/
static void displayIPAddress();

/**
* @brief Returns last digit from given value. i.e returns 9 from 129.
*
* @param val Value from which return last digit.
* @return Last digit from given value.
*/
static uint8_t getLastDigitFromValue(int16_t val);

/**
* @brief Remove last digit from given value. i.e returns 12 from 129.
*
* @param val Value from which remove digit.
* @return Value with last digit removed.
*/
static int16_t inline removeLastDigitFromValue(int16_t val);

/**
* @brief Returns how much digits has displayedValue.
*
* @return Number of digits in displayedValue.
*/
static uint8_t inline howManyDigitsInDisplayedValue();

/**
* @brief Do start GPIO sequence.
*/
static void startWriteDataToDisplay();

/**
* @brief Do stop GPIO sequence.
*/
static void stopWriteDataToDisplay();

/**
* @brief Do write GPIO sequence.
*
* @param val Bit to write.
*/
static void writeBitToDisplay(bool val);

/**
* @brief Write eight bits to display's memory.
*
* @param Value to write.
*/
static void writeByteToDisplay(int16_t _value);

/**
* @brief Write to display content of displayValue without sign.
*/
static void writeDisplayedValueNoSign();

/**
* @brief Write to display sign of displayValue - if displayValue is negative empty otherwise.
*/
static void writeSignOfDisplayedValue();

/**
* @brief Write emties after displayValue, used to clear previous displayed content.
*/
static void writeEmptiesAfterSign();

/**
* @brief Perform all the stuff necessary to write a value to 7 segment display.
*/
static void writeValueToDisplay();

/* definitions external functions */
void SEG_displayTask(void* pParams) {
	mtk_os_hal_gpio_request(dataGpio);
	mtk_os_hal_gpio_request(clockGpio);
	mtk_os_hal_gpio_set_direction(dataGpio, OS_HAL_GPIO_DIR_OUTPUT);
	mtk_os_hal_gpio_set_direction(clockGpio, OS_HAL_GPIO_DIR_OUTPUT);

	mtk_os_hal_gpio_set_output(clockGpio, 1);
	mtk_os_hal_gpio_set_output(dataGpio, 1);

	while (1) {
		if (GLOBAL_getDisplayState() == SETPOINT_VALUE) {
			setDisplayedValue(GLOBAL_getSetpointValue());
		}
		else if (GLOBAL_getDisplayState() == KNOB_VALUE) {
			setDisplayedValue(GLOBAL_getKnobValueInInputPeriphRange());
		}
		else if (GLOBAL_getDisplayState() == IP_ADDRESS) {
			displayIPAddress();
		}

		writeValueToDisplay();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

/* definitions helpers */

static void setDisplayedValue(int16_t _value) {
	if (_value > 999)
		displayedValue = 999;
	if (_value < -999)
		displayedValue = -999;
	else
		displayedValue = _value;
}

static void displayIPAddress() {
	for (int i = 0; i < 4; i++) {
		setDisplayedValue(GLOBAL_getIPAddress().octets[i]);
		writeValueToDisplay();
		vTaskDelay(pdMS_TO_TICKS(1500));
	}

	GLOBAL_setDisplayState(SETPOINT_VALUE);
}

static uint8_t inline getLastDigitFromValue(int16_t val) {
	return val % 10;
}

static int16_t inline removeLastDigitFromValue(int16_t val) {
	return (int16_t)(val / (double)10);
}

static uint8_t howManyDigitsInDisplayedValue() {
	if (abs(displayedValue) < 10)
		return 1;
	if (abs(displayedValue) < 100)
		return 2;
	return 3;
}

static void startWriteDataToDisplay() {
	//pull dataGpio down when clock is up to start
	mtk_os_hal_gpio_set_output(clockGpio, 1);
	mtk_os_hal_gpio_set_output(dataGpio, 0);
}

static void stopWriteDataToDisplay() {
	//pull dataGpio up when clock is up to stop
	mtk_os_hal_gpio_set_output(clockGpio, 1);
	mtk_os_hal_gpio_set_output(dataGpio, 1);
}

static void writeBitToDisplay(bool val) {
	//change dataGpio when clock is down
	mtk_os_hal_gpio_set_output(clockGpio, 0);
	mtk_os_hal_gpio_set_output(dataGpio, val);
	mtk_os_hal_gpio_set_output(clockGpio, 1);
}

static void writeByteToDisplay(int16_t _value) {
	for (uint8_t i = 0; i < 8; i++) {
		writeBitToDisplay(_value & (1 << i));
	}

	mtk_os_hal_gpio_set_output(clockGpio, 0);//set clock down so dataGpio can be changed to 0 for ack
	mtk_os_hal_gpio_set_output(dataGpio, 0);
	mtk_os_hal_gpio_set_output(clockGpio, 1);
	mtk_os_hal_gpio_set_output(clockGpio, 0);//set clock down again to release ack
}

static void writeDisplayedValueNoSign() {
	int16_t tempVal = abs(displayedValue);
	for (uint8_t i = 0; i < howManyDigitsInDisplayedValue(); i++) {
		uint8_t digit = getLastDigitFromValue(tempVal);
		tempVal = removeLastDigitFromValue(tempVal);
		writeByteToDisplay(digits[digit]);
	}
}

static void writeSignOfDisplayedValue() {
	if (displayedValue < 0)
		writeByteToDisplay(digits[MINUS_INDEX]);
	else
		writeByteToDisplay(digits[EMPTY_INDEX]);
}

static void writeEmptiesAfterSign() {
	for (uint8_t i = 0; i < 4 - (howManyDigitsInDisplayedValue() + 1); i++) {
		writeByteToDisplay(digits[EMPTY_INDEX]);
	}
}

static void writeValueToDisplay() {
	startWriteDataToDisplay();
	writeByteToDisplay(COMMAND_1);
	stopWriteDataToDisplay();

	startWriteDataToDisplay();
	writeByteToDisplay(COMMAND_2);
	writeDisplayedValueNoSign();
	writeSignOfDisplayedValue();
	writeEmptiesAfterSign();
	stopWriteDataToDisplay();

	startWriteDataToDisplay();
	writeByteToDisplay(COMMAND_3);
	stopWriteDataToDisplay();
}