#pragma once

#include <stdbool.h>

/*
* @brief Initialize http server and create listening thread if necessary.
*
* @return 0 on success, -1 on failure.
*/
int httpServerInit();

/*
* @brief Closes active socket of http server, keeps subthread running.
*/
void httpServerStop();

void httpServerKillSubthread();

/*
* @brief Returns whether there is active socket listening for clients
*
* @return True if socket is open and listening for clients.
*/
bool httpIsServerUp();

/*
* @brief Register a function that should be called when server receives GET method.
*
* @param cb Callback function.
*/
void registerGETCallback(const char* (*cb)(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]));