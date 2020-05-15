# This code is based on a sample from Microsoft (see license below),
# with modifications made by MediaTek.
# Modified version of CMakeLists.txt from Microsoft Azure Sphere sample code:
# https://github.com/Azure/azure-sphere-samples/blob/master/Samples/HelloWorld/HelloWorld_RTApp_MT3620_BareMetal/CMakeLists.txt

#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

# Configurations
PROJECT(Azure_Sphere_RTcore_FreeRTOS_ADC C)
ADD_COMPILE_DEFINITIONS(OSAI_FREERTOS)
ADD_COMPILE_DEFINITIONS(OSAI_ENABLE_DMA)
ADD_LINK_OPTIONS(-specs=nano.specs -specs=nosys.specs)

SET(CMAKE_C_FLAGS  " ${CMAKE_C_FLAGS} -mfloat-abi=hard -mfpu=fpv4-sp-d16")

# Executable
ADD_EXECUTABLE(${PROJECT_NAME}
               main.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_i2c.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_spim.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_gpio.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_uart.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_dma.c
               ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/src/os_hal_adc.c "leds.c" "knob.c" "seg.c" "control.c" "common.c" "periph.c" "eeprom.c" "buttons.c" "config.c" "global_state.c")

# Include Folders
INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
                    ./)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
                           ./thirdparty/mt3620_m4_software/MT3620_M4_Sample_Code/FreeRTOS/OS_HAL/inc
                           ./)

# Libraries
set(OSAI_FREERTOS 1)
ADD_SUBDIRECTORY(./thirdparty/mt3620_m4_software/MT3620_M4_Driver ./lib/MT3620_M4_Driver)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} MT3620_M4_Driver)

# Linker, Image
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker.ld)
INCLUDE("${AZURE_SPHERE_MAKE_IMAGE_FILE}")
