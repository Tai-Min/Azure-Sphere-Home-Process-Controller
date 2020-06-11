#pragma once

#include <stdbool.h>
#include <stdint.h>

struct IPAddress {
	uint8_t octets[4];
};

/**
* @brief Check whether there is established network connection.
*
* @return True if board is connected to a network.
*/
bool isNetworkConnected();

/**
* @brief Get IP address of the board received from wlan0 interface.
*
* @return IP of the board or 0.0.0.0 if failure.
*/
struct IPAddress getIPAddress();

/**
* @brief Returns name of user added network interface.
* 
* @return Name of added network interface or empty string if there is no such interface.
*/
const char* getSavedNetworkInterfaceName();

/**
* @brief Update user defined network with given credentials.
* 
* @param ssid SSID of user defined network.
* @param psk PSK of user defined network.
*/
void updateUserDefinedNetwork(char* ssid, char* psk);