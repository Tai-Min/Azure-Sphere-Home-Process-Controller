#include "network_utilities.h"
#include "applibs_versions.h"
#include <applibs/wificonfig.h>

#include <errno.h>
#include <ifaddrs.h>
#include <applibs/log.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

/* external functions */
bool isNetworkConnected();
struct IPAddress getIPAddress();
const char* getSavedNetworkInterfaceName();
void updateUserDefinedNetwork(char* ssid, char* psk);

/* helpers */
/**
* @brief Translate given IPv4 string into IPAddress struct.
*
* @param strip IP address as string.
* @return IP address as IPAddress struct.
*/
struct IPAddress StringToIPAddress(char* strip);

/* declarations external functions */
bool isNetworkConnected() {
	WifiConfig_ConnectedNetwork net;
	int res = WifiConfig_GetCurrentNetwork(&net);
	if (res == -1 && errno == ENODATA)
		return false;
	return true;
}

struct IPAddress getIPAddress() {
    struct IPAddress ip = {
        .octets = {0,0,0,0}
    };

    struct ifaddrs* ifaddr, * ifa;
    int family, s;
    char host[20];

    if (getifaddrs(&ifaddr) == -1)
    {
        Log_Debug("Failure to obtain IP address\n");
        return ip;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, 20, NULL, 0, 5);
        if (strcmp(ifa->ifa_name, "wlan0") == 0 && (ifa->ifa_addr->sa_family == AF_INET)) {
            if (s != 0) {
                Log_Debug("Failure to obtain IP address\n");
                freeifaddrs(ifaddr);
                return ip;
            }
            
            ip = StringToIPAddress(host);
        }
    }
    freeifaddrs(ifaddr);
    return ip;
}

const char* getSavedNetworkInterfaceName() {
    static char buf[WIFICONFIG_SSID_MAX_LENGTH];
    int cnt = WifiConfig_GetStoredNetworkCount();
    WifiConfig_StoredNetwork*nets = malloc(sizeof(WifiConfig_StoredNetwork)*cnt);
    int res = WifiConfig_GetStoredNetworks(nets, cnt);
    Log_Debug("%d\n", res);
    if (res < 2) {
        free(nets);
        return "";
    }
    memcpy(buf, nets[1].ssid, nets[1].ssidLength);
    buf[nets[1].ssidLength] = '\0';

    free(nets);
    return buf;
}

void updateUserDefinedNetwork(char* ssid, char* psk) {
    WifiConfig_SetSSID(1, ssid, strlen(ssid));
    WifiConfig_SetPSK(1, psk, strlen(psk));
    WifiConfig_SetPSK(0, psk, strlen(psk));
    WifiConfig_PersistConfig();
}

/* declarations helpers */

struct IPAddress StringToIPAddress(char *strip) {
    struct IPAddress ip = {
        .octets = {0,0,0,0}
    };

    char* ipsegment = strtok(strip, ".");
    int cntr = 0;
    while (ipsegment != NULL) {
        ip.octets[cntr] = atoi(ipsegment);
        cntr++;
        ipsegment = strtok(NULL, ".");
    }

    return ip;
}