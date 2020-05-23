#include "FreeRTOS.h"
#include "task.h"

#include "os_hal_i2c.h"

#include "config.h"
#include "eeprom.h"

#define USAGE_BIT 0

#define CONTROLLER_CONF_FIRST_BYTE (USAGE_BIT + 1)
#define CONTROLLER_CONF_LAST_BYTE (sizeof(struct ControllerConfig) + CONTROLLER_CONF_FIRST_BYTE - 1)

#define INPUT_CONF_FIRST_BYTE (CONTROLLER_CONF_LAST_BYTE + 1)
#define INPUT_CONF_LAST_BYTE (sizeof(struct InputPeriphConfig) + INPUT_CONF_FIRST_BYTE - 1)

#define OUTPUT_CONF_FIRST_BYTE (INPUT_CONF_LAST_BYTE + 1)
#define OUTPUT_CONF_LAST_BYTE (sizeof(struct OutputPeriphConfig) + OUTPUT_CONF_FIRST_BYTE - 1)

#define MQTT_CONF_FIRST_BYTE (OUTPUT_CONF_LAST_BYTE + 1)
#define MQTT_CONF_LAST_BYTE (sizeof(struct MQTTConfig) + MQTT_CONF_FIRST_BYTE - 1)

#define EEPROM_CONTROL_CODE 0b01010000

static i2c_num i2cNum = OS_HAL_I2C_ISU2;

static uint8_t * commandBuf;
static int8_t * rxBuf;

/* external functions */
void initEEPROMCommunication();
bool isEEPROMInitialized();
struct InputPeriphConfig loadInputConfigFromEEPROM();
struct OutputPeriphConfig loadOutputConfigFromEEPROM();
struct ControllerConfig loadControllerConfigFromEEPROM();
struct MQTTConfig loadMQTTConfigFromEEPROM();

void setEEPROMInitializedBit();
void writeInputConfigToEEPROM(struct InputPeriphConfig inputConf);
void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf);
void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig);
void writeMQTTConfigToEEPROM(struct MQTTConfig mqttConf);

/* definitions external functions */
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

	return (res == 1);
}

struct InputPeriphConfig loadInputConfigFromEEPROM() {
	struct InputPeriphConfig inputConf;
	uint8_t addr = INPUT_CONF_FIRST_BYTE;
	rxBuf = pvPortMalloc(sizeof(struct InputPeriphConfig));
	
	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, rxBuf, 1, sizeof(struct InputPeriphConfig));
	memcpy((uint8_t *)&inputConf, rxBuf, sizeof(struct InputPeriphConfig));

	printf("Input loaded as:\n");
	printf("Min val: %f\n", inputConf.inputMinValue);
	printf("Max val: %f\n", inputConf.inputMaxValue);

	vPortFree(rxBuf);

	return inputConf;
}

struct OutputPeriphConfig loadOutputConfigFromEEPROM() {
	struct OutputPeriphConfig outputConf;
	uint8_t addr = OUTPUT_CONF_FIRST_BYTE;
	rxBuf = pvPortMalloc(sizeof(struct OutputPeriphConfig));

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, rxBuf, 1, sizeof(struct OutputPeriphConfig));
	memcpy((uint8_t*)&outputConf, rxBuf, sizeof(struct OutputPeriphConfig));

	printf("Output loaded as:\n");
	printf("Min val: %f\n", outputConf.outputMinValue);
	printf("Max val: %f\n", outputConf.outputMaxValue);

	vPortFree(rxBuf);

	return outputConf;
}

struct ControllerConfig loadControllerConfigFromEEPROM() {
	struct ControllerConfig ctrlConf;
	uint8_t addr = CONTROLLER_CONF_FIRST_BYTE;
	rxBuf = pvPortMalloc(sizeof(struct ControllerConfig));

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, rxBuf, 1, sizeof(struct ControllerConfig));
	memcpy((uint8_t*)&ctrlConf, rxBuf, sizeof(struct ControllerConfig));

	if (ctrlConf.controllerType == NONE) {
		printf("Controller: NONE\n");
	}
	else if (ctrlConf.controllerType == TWO_STATE) {
		printf("Controller: TWO STATE\n");
		printf("Off value: %f\n", ctrlConf.controllerConfig.twoStateConfig.offValue);
		printf("On value: %f\n", ctrlConf.controllerConfig.twoStateConfig.onValue);
		printf("Bottom switch boundary: %f\n", ctrlConf.controllerConfig.twoStateConfig.bottomSwitchBoundary);
		printf("Top switch boundary: %f\n", ctrlConf.controllerConfig.twoStateConfig.topSwitchBoundary);
	}
	else if (ctrlConf.controllerType == PID) {
		printf("Controller: PID\n");
		printf("Kp value: %f\n", ctrlConf.controllerConfig.PIDConfig.kp);
		printf("Ki value: %f\n", ctrlConf.controllerConfig.PIDConfig.ki);
		printf("Kd value: %f\n", ctrlConf.controllerConfig.PIDConfig.kd);
		printf("Lower saturation: %f\n", ctrlConf.controllerConfig.PIDConfig.saturationLower);
		printf("Upper saturation: %f\n", ctrlConf.controllerConfig.PIDConfig.saturarionUpper);
	}
	else {
		printf("Controller loaded as Invalid\n");
	}

	vPortFree(rxBuf);

	return ctrlConf;
}

struct MQTTConfig loadMQTTConfigFromEEPROM() {
	struct MQTTConfig mqttConf;
	uint8_t addr = MQTT_CONF_FIRST_BYTE;
	rxBuf = pvPortMalloc(sizeof(struct MQTTConfig));

	mtk_os_hal_i2c_write_read(i2cNum, EEPROM_CONTROL_CODE, &addr, rxBuf, 1, sizeof(struct MQTTConfig));
	memcpy((uint8_t*)&mqttConf, rxBuf, sizeof(struct MQTTConfig));

	printf("MQTT config loaded as:\n");
	printf("Broker IP: %s\n", mqttConf.brokerIPAddress);
	printf("Setpoint topic: %s\n", mqttConf.setpointTopic);
	printf("Process value topic: %s\n", mqttConf.processValueTopic);

	vPortFree(rxBuf);

	return mqttConf;
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
	rxBuf = pvPortMalloc(2);

	for (uint8_t i = 0; i < 1 + sizeof(struct InputPeriphConfig); i++) {
		rxBuf[0] = INPUT_CONF_FIRST_BYTE + i;
		memcpy(rxBuf+1, (uint8_t*)&inputConf +i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, rxBuf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	vPortFree(rxBuf);

	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeOutputConfigToEEPROM(struct OutputPeriphConfig outputConf) {
	rxBuf = pvPortMalloc(2);

	for (uint8_t i = 0; i < 1 + sizeof(struct OutputPeriphConfig); i++) {
		rxBuf[0] = OUTPUT_CONF_FIRST_BYTE + i;
		memcpy(rxBuf + 1, (uint8_t*)&outputConf + i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, rxBuf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	vPortFree(rxBuf);

	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeControllerConfigToEEPROM(struct ControllerConfig controllerConfig) {
	rxBuf = pvPortMalloc(2);

	for (uint8_t i = 0; i < 1 + sizeof(struct ControllerConfig); i++) {
		rxBuf[0] = CONTROLLER_CONF_FIRST_BYTE + i;
		memcpy(rxBuf + 1, (uint8_t*)&controllerConfig + i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, rxBuf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	vPortFree(rxBuf);

	vTaskDelay(pdMS_TO_TICKS(10));
}

void writeMQTTConfigToEEPROM(struct MQTTConfig mqttConf) {
	rxBuf = pvPortMalloc(2);

	for (uint8_t i = 0; i < 1 + sizeof(struct MQTTConfig); i++) {
		rxBuf[0] = MQTT_CONF_FIRST_BYTE + i;
		memcpy(rxBuf + 1, (uint8_t*)&mqttConf + i, 1);
		mtk_os_hal_i2c_write(i2cNum, EEPROM_CONTROL_CODE, rxBuf, 2);
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	vPortFree(rxBuf);

	vTaskDelay(pdMS_TO_TICKS(10));
}
