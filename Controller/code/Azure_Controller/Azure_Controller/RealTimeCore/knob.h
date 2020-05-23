#pragma once

#include <stdint.h>
#include <stdbool.h>

/*
* @brief Reacts to knob's movement and sets knob's diode in case of movement then sets setpoint's diode after short timeout.
*
* @param pParams unused.
*/
void KNOB_knobTask(void* pParams);