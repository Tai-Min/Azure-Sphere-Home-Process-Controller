#pragma once

#include <stdbool.h>

/*
* @brief Set id scope of azure sphere cloud. Should be passed as argv to application.
*
* @param id Id to be set.
*/
void setCloudScopeId(char* id);

/*
* @brief Set up connection with Azure IoT Center.
*/
void SetupAzureClient();

/*
* @brief Check whether there is active authenticated connection with Azure IoT Central.
* 
* @return True if there is active connection.
*/
bool isCloudAuthenticated();

/*
* @brief Get poll period for cloud's timer. 
*
* If everything is working correctly, this function should return 1 to perform LL work every second.
* In case of problems with connection to the cloud, period is increased a bit up to some maximum period value.
*
* @return Period for Cloud's event timer in seconds.
*/
int getCloudPollPeriodSeconds();

/*
* @brief send JSON string to Azure IoT Central.
*
* @param jsonMessage JSON to be sent.
*/
void sendTelemetryToCloud(const char* jsonMessage);

/*
* @brief Handle all interactions with the cloud.
*/
void doCloudLLWork();