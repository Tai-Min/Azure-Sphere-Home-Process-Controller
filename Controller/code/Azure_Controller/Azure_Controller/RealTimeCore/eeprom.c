#include "FreeRTOS.h"
#include "task.h"

#include "os_hal_i2c.h"

#include "eeprom.h"

#define USAGE_BIT 0

#define CONTROLLER_CONF_SIZE_BYTES sizeof(struct ControllerConfig)
#define CONTROLLER_CONF_FIRST_BIT (USAGE_BIT + 1)
#define CONTROLLER_CONF_LAST_BIT (sizeof(struct ControllerConfig) + CONTROLLER_CONF_FIRST_BIT - 1)

#define INPUT_CONF_SIZE_BYTES sizeof(struct InputPeriphConfig)
#define INPUT_CONF_FIRST_BIT (CONTROLLER_CONF_LAST_BIT + 1)
#define INPUT_CONF_LAST_BIT (sizeof(struct InputPeriphConfig) + INPUT_CONF_FIRST_BIT - 1)

#define OUTPUT_CONF_SIZE_BYTES sizeof(struct OutputPeriphConfig)
#define OUTPUT_CONF_FIRST_BIT (INPUT_CONF_LAST_BIT + 1)
#define OUTPUT_CONF_LAST_BIT (sizeof(struct OutputPeriphConfig) - 1)

#define EEPROM_CONTROL_CODE 0b01010000

static union InputPeriphConfigBuffer {
	struct InputPeriphConfig config;
	uint8_t bytes[sizeof(struct InputPeriphConfig)];
};

static union OutputPeriphConfigBuffer {
	struct OutputPeriphConfig config;
	uint8_t bytes[sizeof(struct OutputPeriphConfig)];
};

static union ControllerConfigBuffer {
	struct ControllerConfig config;
	uint8_t bytes[sizeof(struct ControllerConfig)+1];
};

static union InputPeriphConfigBuffer inputConfBuf;

static union OutputPeriphConfigBuffer outputConfBuf;

static union ControllerConfigBuffer controllerConfBuf;

static i2c_num i2cNum = OS_HAL_I2C_ISU2;

static uint8_t * commandBuf;
static int8_t * buf;

/* external functions */
void initEEPROMCommunication();
bool isEEPROMInitialized();
enum ControllerType getControllerTypeFromEEPROM();
struct InputPeriphConfig loadInputConfigFromEEPROM();
struct OutputPeriphConfig loadOutputConfigFromEEPROM();
struct ControllerConfig loadControllerConfigFromEEPROM();
void setEEPROMInitializedBit();
void writeInputConfigToEEPROM(struct InputPeriphConfig inputConf);
void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf);
void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig);

/* declarations */
void initEEPROMCommunication() {
	mtk_os_hal_i2c_ctrl_init(i2cNum);
	mtk_os_hal_i2c_speed_init(i2cNum, I2C_SCL_400kHz);
	vTaskDelay(pdMS_TO_TICKS(10));
}

bool isEEPROMInitialized() {
	uint8_t addr = USAGE_BIT;
	uint8_t res;

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, &res, 1, 1);

	printf("Usage bit received as: %d\n", res);

	return res;
}

struct InputPeriphConfig loadInputConfigFromEEPROM() {
	uint8_t addr = INPUT_CONF_FIRST_BIT;
	buf = pvPortMalloc(INPUT_CONF_SIZE_BYTES);
	
	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, buf, 1, INPUT_CONF_SIZE_BYTES);
	memcpy(inputConfBuf.bytes, buf, INPUT_CONF_SIZE_BYTES);

	printf("Input loaded as:\n");
	printf("Min val: %f\n", inputConfBuf.config.inputMinValue);
	printf("Max val: %f\n", inputConfBuf.config.inputMaxValue);

	vPortFree(buf);

	return inputConfBuf.config;
}

struct OutputPeriphConfig loadOutputConfigFromEEPROM() {
	uint8_t addr = OUTPUT_CONF_FIRST_BIT;
	buf = pvPortMalloc(OUTPUT_CONF_SIZE_BYTES);

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, buf, 1, OUTPUT_CONF_SIZE_BYTES);
	memcpy(outputConfBuf.bytes, buf, OUTPUT_CONF_SIZE_BYTES);

	printf("Output loaded as:\n");
	printf("Min val: %f\n", outputConfBuf.config.outputMinValue);
	printf("Max val: %f\n", outputConfBuf.config.outputMaxValue);

	vPortFree(buf);

	return outputConfBuf.config;
}

struct ControllerConfig loadControllerConfigFromEEPROM() {
	uint8_t addr = CONTROLLER_CONF_FIRST_BIT;
	buf = pvPortMalloc(CONTROLLER_CONF_SIZE_BYTES);

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, buf, 1, CONTROLLER_CONF_SIZE_BYTES);
	memcpy(controllerConfBuf.bytes, buf, CONTROLLER_CONF_SIZE_BYTES);

	if (controllerConfBuf.config.controllerType == NONE) {
		printf("Controller loaded as NONE\n");
	}
	else if (controllerConfBuf.config.controllerType == TWO_STATE) {
		printf("Controller loaded as Two State\n");
	}
	else if (controllerConfBuf.config.controllerType == PID) {
		printf("Controller loaded as PID\n");
	}
	else {
		printf("Controller loaded as Invalid\n");
	}

	vPortFree(buf);

	return controllerConfBuf.config;
}

void setEEPROMInitializedBit() {
	commandBuf = pvPortMalloc(2);
	commandBuf[0] = USAGE_BIT;
	commandBuf[1] = 1;

	mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, commandBuf, 2);

	vPortFree(commandBuf);
	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeInputConfigToEEPROM(struct InputPeriphConfig inputConf) {
	inputConfBuf.config = inputConf;

	buf = pvPortMalloc(2);
	for (uint8_t i = 0; i < 1 + INPUT_CONF_SIZE_BYTES; i++) {
		buf[0] = INPUT_CONF_FIRST_BIT + i;
		memcpy(buf+1, inputConfBuf.bytes+i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, buf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vPortFree(buf);
	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf) {
	outputConfBuf.config = outputConf;

	buf = pvPortMalloc(2);
	for (uint8_t i = 0; i < 1 + OUTPUT_CONF_SIZE_BYTES; i++) {
		buf[0] = OUTPUT_CONF_FIRST_BIT + i;
		memcpy(buf + 1, outputConfBuf.bytes + i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, buf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vPortFree(buf);
	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig) {
	controllerConfBuf.config = controllerConfig;

	buf = pvPortMalloc(2);
	for (uint8_t i = 0; i < 1 + CONTROLLER_CONF_SIZE_BYTES; i++) {
		buf[0] = CONTROLLER_CONF_FIRST_BIT + i;
		memcpy(buf + 1, controllerConfBuf.bytes + i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, buf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vPortFree(buf);
	vTaskDelay(pdMS_TO_TICKS(10));
}
