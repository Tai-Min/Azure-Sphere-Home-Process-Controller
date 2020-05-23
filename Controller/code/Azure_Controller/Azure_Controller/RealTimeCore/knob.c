#include <math.h>
#include <stdlib.h>
#include "FreeRTOS.h"

#include "os_hal_adc.h"
#include "printf.h"

#include "knob.h"
#include "common.h"
#include "config.h"
#include "global_state.h"

#define KNOB_MIN 0
#define KNOB_MAX (4095 - 50) //offset due to potentiometer's real range

#define KNOB_MAX_DIFFERENCE 150

static const adc_channel adcChannel = ADC_CHANNEL_1;

static uint32_t currentKnobValue = 0;
static uint32_t previousKnobValue = 0;
static bool knobValueChangedflag = false;
static long long knobDisplayTimestamp = 0;

/* external functions */
void KNOB_knobTask(void* pParams);

/* helpers */
/*
* @brief Returns knob's value from previous reading.
* 
* @return Knob's value from previous reading.
*/
static uint16_t getKnobValue();

/*
* @brief Get knob's value mapped to input peripheral boundaries.
*
* @return Knob's value mapped to input peripheral boundaries.
*/
static int16_t getKnobValueInInputPeriphRange();

/*
* @brief Returns true if knob's value has changed by KNOB_MAX_DIFFERENCE margin.
*
* @return True if knob's value has changed by KNOB_MAX_DIFFERENCE margin.
*/
static bool knobValueChanged();

/*
* @brief Ignore if knob value has changed. Useful to ignore change when IP address is currently displayed.
*/
static void dimissKnobValueChange();

/*
* @brief Read knob's ADC channel.
*/
static void getKnobCurrentValue();

/*
* @brief Get how much knob's value has changed between readings.
*
* @return The difference.
*/
static uint16_t inline getKnobValueDifference();

/*
* @brief Check if knob's value has changed by KNOB_MAX_DIFFERENCE and set adequate flag.
*/
static void checkKnobForChange();

/*
* @brief Returns true if some time has passed without change of knob's value.
*/
static bool knobTimeout();

/* definitions external functions */
void KNOB_knobTask(void* pParams) {
	getKnobCurrentValue(adcChannel, &currentKnobValue);
	previousKnobValue = currentKnobValue;
	vTaskDelay(pdMS_TO_TICKS(50));

	while (1) {
		getKnobCurrentValue();

		checkKnobForChange();

		if (knobValueChanged() && GLOBAL_getDisplayState() != IP_ADDRESS) {
			GLOBAL_setDisplayState(KNOB_VALUE);
			knobDisplayTimestamp = millis();
		}

		if (GLOBAL_getDisplayState() == IP_ADDRESS) {
			dimissKnobValueChange();
		}

		if (GLOBAL_getDisplayState() == KNOB_VALUE && knobTimeout()){
			GLOBAL_setDisplayState(SETPOINT_VALUE);
		}

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

/* definitions helpers */
static uint16_t getKnobValue() {
	return currentKnobValue;
}

static int16_t getKnobValueInInputPeriphRange() {
	return map(getKnobValue(), KNOB_MIN, KNOB_MAX, getInputPeriphConfig().inputMinValue, getInputPeriphConfig().inputMaxValue);
}

static bool knobValueChanged() {
	if (knobValueChangedflag) {
		knobValueChangedflag = false;
		return true;
	}
	return false;
}

static void dimissKnobValueChange() {
	knobValueChanged();
}

static void getKnobCurrentValue() {
	u16 ch_bit_map = (0x01 << adcChannel);

	mtk_os_hal_adc_ctlr_init(ADC_PMODE_ONE_TIME, ADC_FIFO_DIRECT, ch_bit_map);

	mtk_os_hal_adc_start_ch(ch_bit_map);

	mtk_os_hal_adc_one_shot_get_data(adcChannel, (u32*)&currentKnobValue);

	vTaskDelay(10);
	mtk_os_hal_adc_ctlr_deinit();

	GLOBAL_setKnobValueInInputPeriphRange(getKnobValueInInputPeriphRange());
}

static uint16_t inline getKnobValueDifference() {
	return abs((int16_t)(currentKnobValue - previousKnobValue));
}

static void checkKnobForChange() {
	if (getKnobValueDifference() > KNOB_MAX_DIFFERENCE) {
		knobValueChangedflag = true;
		previousKnobValue = currentKnobValue;
	}
}

static bool knobTimeout() {
	return (millis() - knobDisplayTimestamp > 5000);
}