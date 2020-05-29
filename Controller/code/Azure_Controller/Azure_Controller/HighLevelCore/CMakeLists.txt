#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.8)
project (Azure_Controller_HighLevelCore C)

# MQTT-C library
#add_library(mqttc STATIC 
#    thirdparty/MQTT-C/src/mqtt_pal.c
#    thirdparty/MQTT-C/src/mqtt.c
#)

#TARGET_INCLUDE_DIRECTORIES(./thirdparty/MQTT-C/inc)

#azsphere_configure_tools(TOOLS_REVISION "20.04")
#azsphere_configure_api(TARGET_API_SET "latest-lts")

# Create executable
add_compile_definitions(__unix__)
add_compile_definitions(__THROW)
add_executable (${PROJECT_NAME} 
                main.c 
                ./thirdparty/MQTT-C/src/mqtt.c
                ./thirdparty/MQTT-C/src/mqtt_pal.c
                "eventloop_timer_utilities.c" "mqtt_utilities.c" "common.c" "http_utilities.c" "mailbox_utilities.c" "network_utilities.c" "parson.c" "cloud.c")

target_include_directories(${PROJECT_NAME} PUBLIC ${AZURE_SPHERE_API_SET_DIR}/usr/include/azureiot)
target_compile_definitions(${PROJECT_NAME} PUBLIC AZURE_IOT_HUB_CONFIGURED)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
                           ./thirdparty/MQTT-C/include
                           ./)


target_link_libraries (${PROJECT_NAME} m azureiot applibs pthread gcc_s c)

# Add MakeImage post-build command
include (${AZURE_SPHERE_MAKE_IMAGE_FILE})
