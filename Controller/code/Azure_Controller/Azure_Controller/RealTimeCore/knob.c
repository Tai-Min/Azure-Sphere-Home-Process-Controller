#include <math.h>
#include "FreeRTOS.h"

#include "os_hal_adc.h"

#include "knob.h"
#include "common.h"
#include "global_state.h"

#define KNOB_MAX_DIFFERENCE 250

static const adc_channel adcChannel = ADC_CHANNEL_1;

static uint32_t currentKnobValue = 0;
static uint32_t previousKnobValue = 0;
static bool knobValueChangedflag = false;
static long long knobDisplayTimestamp = 0;

/* external functions */
void KNOB_knobTask(void* pParams);

/* helpers */
static uint16_t getKnobValue();
int16_t getKnobValueInInputPeriphRange();

static bool knobValueChanged();
static void dimissKnobValueChange();

static int getKnobCurrentValue();
static uint16_t inline getKnobValueDifference();
static void checkKnobForChange();
static bool knobTimeout();

/* declarations */
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

/**************************************************************/

uint16_t getKnobValue() {
	return currentKnobValue;
}

int16_t getKnobValueInInputPeriphRange() {
	return map(getKnobValue(), KNOB_MIN, KNOB_MAX, 50, 121);
}

bool knobValueChanged() {
	if (knobValueChangedflag) {
		knobValueChangedflag = false;
		return true;
	}
	return false;
}

void dimissKnobValueChange() {
	knobValueChanged();
}

static int getKnobCurrentValue() {
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