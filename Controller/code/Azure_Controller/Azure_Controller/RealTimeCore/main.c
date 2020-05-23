#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "mt3620.h"
#include "FreeRTOS.h"
#include "task.h"
#include "printf.h"

#include "os_hal_gpio.h"
#include "os_hal_uart.h"

#include "leds.h"
#include "knob.h"
#include "seg.h"
#include "control.h"
#include "common.h"
#include "buttons.h"
#include "config.h"
#include "mailbox.h"

#define APP_STACK_SIZE_BYTES (1024 / 4)

static const uint8_t uart_port_num = OS_HAL_UART_PORT0;

/* handlers */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	printf("%s: %s\n", __func__, pcTaskName);
}

void vApplicationMallocFailedHook(void)
{
	printf("%s\n", __func__);
}

void _putchar(char character)
{
	mtk_os_hal_uart_put_char(uart_port_num, character);
	if (character == '\n')
		mtk_os_hal_uart_put_char(uart_port_num, '\r');
}

/* definitions */
_Noreturn void RTCoreMain(void);

/* declarations */
_Noreturn void RTCoreMain(void)
{
	NVIC_SetupVectorTable();

	mtk_os_hal_uart_ctlr_init(uart_port_num);

	xTaskCreate(KNOB_knobTask, "Knob Task", APP_STACK_SIZE_BYTES, NULL, 0, NULL);
	xTaskCreate(LED_ledTask, "LED Task", APP_STACK_SIZE_BYTES, NULL, 0, NULL);
	xTaskCreate(buttonTask, "Button Task", APP_STACK_SIZE_BYTES, NULL, 0, NULL);
	xTaskCreate(SEG_displayTask, "Display Task", APP_STACK_SIZE_BYTES, NULL, 0, NULL);
	xTaskCreate(CONTROL_controlTask, "Control Task", APP_STACK_SIZE_BYTES * 2, NULL, 7, NULL);
	xTaskCreate(MAILBOX_mailboxTask, "Mailbox Task", APP_STACK_SIZE_BYTES * 2, NULL, 5, NULL);

	vTaskStartScheduler();
	for (;;)
		__asm__("wfi");
}