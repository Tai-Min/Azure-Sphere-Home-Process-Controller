#include "cloud.h"

#include "applibs_versions.h"
#include <applibs/log.h>
#include <applibs/networking.h>

#include <azureiot/iothub_client_options.h>
#include <azureiot/azure_sphere_provisioning.h>
#include "parson.h"

// Azure IoT defines.
static char* scopeId;
static IOTHUB_DEVICE_CLIENT_LL_HANDLE iothubClientHandle = NULL;
static const int keepalivePeriodSeconds = 20;
static bool iothubAuthenticated = false;

// Azure IoT poll periods
const int AzureIoTDefaultPollPeriodSeconds = 1;
static const int AzureIoTMinReconnectPeriodSeconds = 60;
static const int AzureIoTMaxReconnectPeriodSeconds = 10 * 60;
static int azureIoTPollPeriodSeconds = AzureIoTDefaultPollPeriodSeconds;

/* external functions */
void setCloudScopeId(char* id);
void SetupAzureClient();
bool isCloudAuthenticated();
int getCloudPollPeriodSeconds();
void sendTelemetryToCloud(const char* jsonMessage);
void doCloudLLWork();

/* declarations external functions */
void setCloudScopeId(char* id) {
    scopeId = id;
}

void SetupAzureClient()
{
    if (iothubClientHandle != NULL) {
        IoTHubDeviceClient_LL_Destroy(iothubClientHandle);
    }

    AZURE_SPHERE_PROV_RETURN_VALUE provResult =
        IoTHubDeviceClient_LL_CreateWithAzureSphereDeviceAuthProvisioning(scopeId, 10000, &iothubClientHandle);

    if (provResult.result != AZURE_SPHERE_PROV_RESULT_OK) {

        // If we fail to connect, reduce the polling frequency, starting at
        // AzureIoTMinReconnectPeriodSeconds and with a backoff up to
        // AzureIoTMaxReconnectPeriodSeconds
        if (azureIoTPollPeriodSeconds == AzureIoTDefaultPollPeriodSeconds) {
            azureIoTPollPeriodSeconds = AzureIoTMinReconnectPeriodSeconds;
        }
        else {
            azureIoTPollPeriodSeconds *= 2;
            if (azureIoTPollPeriodSeconds > AzureIoTMaxReconnectPeriodSeconds) {
                azureIoTPollPeriodSeconds = AzureIoTMaxReconnectPeriodSeconds;
            }
        }

        Log_Debug("ERROR: Failed to create IoTHub Handle - will retry in %i seconds.\n", azureIoTPollPeriodSeconds);
        return;
    }

    // Successfully connected, so make sure the polling frequency is back to the default
    azureIoTPollPeriodSeconds = AzureIoTDefaultPollPeriodSeconds;

    iothubAuthenticated = true;

    if (IoTHubDeviceClient_LL_SetOption(iothubClientHandle, OPTION_KEEP_ALIVE,
        &keepalivePeriodSeconds) != IOTHUB_CLIENT_OK) {
        Log_Debug("ERROR: Failure setting Azure IoT Hub client option \"%s\".\n",
            OPTION_KEEP_ALIVE);
        return;
    }
}

bool isCloudAuthenticated() {
    return iothubAuthenticated;
}

int getCloudPollPeriodSeconds() {
    return azureIoTPollPeriodSeconds;
}

void sendTelemetryToCloud(const char* jsonMessage)
{
    Log_Debug("Sending Azure IoT Hub telemetry: %s.\n", jsonMessage);

    bool isNetworkingReady = false;
    if ((Networking_IsNetworkingReady(&isNetworkingReady) == -1) || !isNetworkingReady) {
        Log_Debug("WARNING: Cannot send Azure IoT Hub telemetry because the network is not up.\n");
        return;
    }

    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromString(jsonMessage);

    if (messageHandle == 0) {
        Log_Debug("ERROR: unable to create a new IoTHubMessage.\n");
        return;
    }

    if (IoTHubDeviceClient_LL_SendEventAsync(iothubClientHandle, messageHandle, NULL, NULL) != IOTHUB_CLIENT_OK) {
        Log_Debug("ERROR: failure requesting IoTHubClient to send telemetry event.\n");
    }
    else {
        Log_Debug("INFO: IoTHubClient accepted the telemetry event for delivery.\n");
    }

    IoTHubMessage_Destroy(messageHandle);
}

void doCloudLLWork() {
    IoTHubDeviceClient_LL_DoWork(iothubClientHandle);
}