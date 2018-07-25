/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
***************************************************************************
*
* FNET Azure-IOT client example.
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_AZURE_CMD && FNET_CFG_AZURE

#include "fapp_prv.h"
#include "fapp_azure.h"

#include <stdio.h>
#include <stdlib.h>

#include "certs_azure.h"
#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_options.h"
#include "azure_c_shared_utility/xlogging.h"

/* Device connection string containing Hostname, Device Id & Device Key in the format:
   "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
#ifndef FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING
    #error FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING device connection string is not defined. "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"
#endif

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_azure_client_mqtt(void);
#ifndef NO_LOGGING
    static void _fapp_azure_log_function(LOG_CATEGORY log_category, const char *file, const char *func, int line, unsigned int options, const char *format, ...);
#endif
static void _fapp_azure_on_ctrlc(fnet_shell_desc_t desc, void *cookie);
static fnet_return_t _fapp_azure_client_send_message(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle);
static IOTHUBMESSAGE_DISPOSITION_RESULT _fapp_azure_receive_message_callback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback);

/************************************************************************
*     Definitions.
*************************************************************************/
static fnet_bool_t  _fapp_azure_is_running;
static fnet_bool_t  _fapp_azure_is_unauthenticated;
static fnet_index_t _fapp_azure_message_counter;
static char         _fapp_azure_message_buffer[512];

/************************************************************************
* Ctr+C termination handler.
************************************************************************/
static void _fapp_azure_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    /* Terminate Azure example. */
    _fapp_azure_is_running = FNET_FALSE;
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* Start Azure-IOT client.
************************************************************************/
void fapp_azure_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " Azure IoT Hub Telemetry example");
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Period (sec)", FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD );
    fnet_shell_println(desc, FAPP_TOCANCEL_STR);
    fnet_shell_println(desc, FAPP_DELIMITER_STR);

#ifndef NO_LOGGING
    xlogging_set_log_function(_fapp_azure_log_function); /* Register Azure log function (optional) */
#endif

    fnet_shell_block(desc, _fapp_azure_on_ctrlc, FNET_NULL); /* Block the shell input.*/

    fapp_azure_client_mqtt(); /* Start Azure client example (over MQTT) */

    fnet_shell_unblock(desc); /* Unblock shell, just in case.*/
}

/************************************************************************
* Azure log function.
************************************************************************/
#ifndef NO_LOGGING
static void _fapp_azure_log_function(LOG_CATEGORY log_category, const char *file, const char *func, int line, unsigned int options, const char *format, ...)
{
#if FAPP_CFG_AZURE_CMD_LOG
    time_t t;
    va_list args;
    va_start(args, format);

    t = time(NULL);

    switch (log_category)
    {
        case AZ_LOG_INFO:
            (void)fnet_printf("[LOG] Info: ");
            break;
        case AZ_LOG_ERROR:
            (void)fnet_printf("[LOG] Error: Time:%.24s File:%s Func:%s Line:%d ", ctime(&t), file, func, line);
            break;
        default:
            break;
    }

    (void)fnet_vprintf(format, args);
    va_end(args);

    (void)log_category;
    if (options & LOG_LINE)
    {
        (void)fnet_printf("\r\n");
    }
#else
    return;
#endif
}
#endif

/*******************************************************************************
 * Receive message callback
 ******************************************************************************/
static IOTHUBMESSAGE_DISPOSITION_RESULT _fapp_azure_receive_message_callback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback)
{
    int         *counter = (int *)userContextCallback;
    const char  *buffer;
    size_t      size;
    MAP_HANDLE  mapProperties;
    const char  *messageId;
    const char  *correlationId;
    const char  *userDefinedContentType;
    const char  *userDefinedContentEncoding;

    /* Message properties */
    if ((messageId = IoTHubMessage_GetMessageId(message)) == NULL)
    {
        messageId = "<null>";
    }

    if ((correlationId = IoTHubMessage_GetCorrelationId(message)) == NULL)
    {
        correlationId = "<null>";
    }

    if ((userDefinedContentType = IoTHubMessage_GetContentTypeSystemProperty(message)) == NULL)
    {
        userDefinedContentType = "<null>";
    }

    if ((userDefinedContentEncoding = IoTHubMessage_GetContentEncodingSystemProperty(message)) == NULL)
    {
        userDefinedContentEncoding = "<null>";
    }

    /* Message content */
    if (IoTHubMessage_GetByteArray(message, (const unsigned char **)&buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        fnet_printf("unable to retrieve the message data\r\n");
    }
    else
    {
        fnet_printf("[AZURE_APP] Received Message [%d]\r\n Message ID: %s\r\n Correlation ID: %s\r\n Content-Type: %s\r\n Content-Encoding: %s\r\n Data: <<<%.*s>>> & Size=%d\r\n",
                    *counter, messageId, correlationId, userDefinedContentType, userDefinedContentEncoding, size, buffer, size);
        /* If we receive the work 'quit' then we stop running */
        if (size == (strlen("quit") * sizeof(char)) && memcmp(buffer, "quit", size) == 0)
        {
            _fapp_azure_is_running = FNET_FALSE;
        }
    }

    /* Retrieve properties from the message */
    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char *const *keys;
        const char *const *values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index;

                fnet_printf(" Message Properties:\r\n");
                for (index = 0; index < propertyCount; index++)
                {
                    (void)fnet_printf("\tKey: %s Value: %s\r\n", keys[index], values[index]);
                }
                (void)fnet_printf("\r\n");
            }
        }
    }

    (*counter)++;
    return IOTHUBMESSAGE_ACCEPTED;
}

/************************************************************************
*  Send event callback
************************************************************************/
static void _fapp_azure_send_confirmation_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
    fnet_index_t messageTrackingId = (fnet_index_t)userContextCallback;

    fnet_char_t *result_str;

    /* Status of the event processing in the hub.*/
    switch(result)
    {
        case IOTHUB_CLIENT_CONFIRMATION_OK:
            result_str = "is received by IoT Hub";
            break;
        case IOTHUB_CLIENT_CONFIRMATION_BECAUSE_DESTROY:
            result_str = "is destroied";
            break;
        case IOTHUB_CLIENT_CONFIRMATION_MESSAGE_TIMEOUT:
            result_str = "has timeout";
            break;
        case IOTHUB_CLIENT_CONFIRMATION_ERROR:
            result_str = "has error";
            break;
        default:
            result_str = "is unknown";
            break;
    }

    fnet_println("[AZURE_APP] Message [%u] %s", messageTrackingId, result_str);
}

/************************************************************************
*  Connection status callback
************************************************************************/
static void _fapp_azure_client_connection_callback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void *userContextCallback)
{
    fnet_char_t *status_str;
    fnet_char_t *reason_str;

    switch(result)
    {
        case IOTHUB_CLIENT_CONNECTION_AUTHENTICATED:
            status_str = "authenticated";
            _fapp_azure_is_unauthenticated = FNET_TRUE;
            break;
        case IOTHUB_CLIENT_CONNECTION_UNAUTHENTICATED:
            status_str = "unauthenticated";
            _fapp_azure_is_unauthenticated = FNET_FALSE;
            break;
        default:
            status_str = "unknown"; /* Should never happen */
            break;
    }

    switch(reason)
    {
        case IOTHUB_CLIENT_CONNECTION_EXPIRED_SAS_TOKEN:
            reason_str = "SAS token is expired";
            break;
        case IOTHUB_CLIENT_CONNECTION_DEVICE_DISABLED:
            reason_str = "device is disabled";
            break;
        case IOTHUB_CLIENT_CONNECTION_BAD_CREDENTIAL:
            reason_str = "bad credential";
            break;
        case IOTHUB_CLIENT_CONNECTION_RETRY_EXPIRED:
            reason_str = "retry is expired";
            break;
        case IOTHUB_CLIENT_CONNECTION_NO_NETWORK:
            reason_str = "no network";
            break;
        case IOTHUB_CLIENT_CONNECTION_COMMUNICATION_ERROR:
            reason_str = "communication error";
            break;
        case IOTHUB_CLIENT_CONNECTION_OK:
            reason_str = "OK";
            break;
        default:
            reason_str = "unknown"; /* Should never happen */
            break;
    }

    fnet_println("[AZURE_APP] IoT Hub Connection is %s (%s)", status_str, reason_str);
}

/************************************************************************
*  Prepare and send telemetry message to Azure IoT hub
************************************************************************/
static fnet_return_t _fapp_azure_client_send_message(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle)
{
    /* Now that we are ready to receive commands, let's send some messages */
    fnet_return_t   result = FNET_ERR;
    double          temperature = 20.0;
    double          humidity = 60.0;

    IOTHUB_MESSAGE_HANDLE messageHandle;

    /* Prepare and send telemetry message to Azure IoT hub */
    temperature += (rand() % 10);
    humidity += (rand() % 20);
    sprintf_s(_fapp_azure_message_buffer, sizeof(_fapp_azure_message_buffer), "{\"temperature\":%.2f,\"humidity\":%.2f}", temperature, humidity);

    /* Creates a new IoT hub message from a byte array */
    if ((messageHandle = IoTHubMessage_CreateFromByteArray((unsigned char *)_fapp_azure_message_buffer, strlen(_fapp_azure_message_buffer))) == NULL)
    {
        fnet_println("ERROR: IoTHubMessage_CreateFromByteArray() is failed!");
        _fapp_azure_is_running = FNET_FALSE;
    }
    else
    {
        (void)IoTHubMessage_SetMessageId(messageHandle, "MSG_ID");
        (void)IoTHubMessage_SetCorrelationId(messageHandle, "CORE_ID");
        (void)IoTHubMessage_SetContentTypeSystemProperty(messageHandle, "application%2Fjson");
        (void)IoTHubMessage_SetContentEncodingSystemProperty(messageHandle, "utf-8");

        /* Send message */
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, _fapp_azure_send_confirmation_callback, (void *)_fapp_azure_message_counter) != IOTHUB_CLIENT_OK)
        {
            fnet_println("[AZURE_APP] ERROR: IoTHubClient_LL_SendEventAsync() is failed");
        }
        else
        {
            fnet_println("[AZURE_APP] Sending message [%d]: %s", _fapp_azure_message_counter, _fapp_azure_message_buffer);
        }

        IoTHubMessage_Destroy(messageHandle);

        _fapp_azure_message_counter++;

        result = FNET_OK;
    }

    return result;
}

/************************************************************************
*  Start Azure client example (over MQTT)
************************************************************************/
static void fapp_azure_client_mqtt(void)
{
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    _fapp_azure_is_running = FNET_TRUE;
    _fapp_azure_is_unauthenticated = FNET_FALSE;

    int receiveContext = 0;

    /* Creates a IoT Hub client for communication with an existing IoT Hub using the specified connection string parameter. */
    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        fnet_println("ERROR: iotHubClient initialization is failed!");
    }
    else
    {
#if FAPP_CFG_AZURE_CMD_TRACE /* Enable Azure message trace. */
        {
            bool traceOn = true;
            IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);
        }
#endif

        /* Add the trustet certificate information */
        if (IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates) != IOTHUB_CLIENT_OK)
        {
            fnet_println("ERROR: Trusted certificate is failed.");
        }
        else
            /* Set the receive message call back. */
            if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, _fapp_azure_receive_message_callback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                fnet_println("ERROR: IoTHubClient_LL_SetMessageCallback() is failed");
            }
            else
                /* Set up the connection status callback to be invoked representing the status of the connection to IOT Hub. (Optional). */
                if (IoTHubClient_LL_SetConnectionStatusCallback(iotHubClientHandle, _fapp_azure_client_connection_callback, NULL) != IOTHUB_CLIENT_OK)
                {
                    fnet_println("ERROR: IoTHubClient_LL_SetConnectionStatusCallback() is failed");
                }
                else
                {
                    fnet_time_t timestamp = fnet_timer_get_seconds() + FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD;

                    _fapp_azure_message_counter = 0;

                    do
                    {
                        if((_fapp_azure_is_unauthenticated == FNET_TRUE)
                           && ((fnet_timer_get_seconds() - timestamp) > FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD ))
                        {
                            if(_fapp_azure_client_send_message(iotHubClientHandle) == FNET_ERR)
                            {
                                break;
                            }
                            else
                            {
                                timestamp = fnet_timer_get_seconds();
                            }
                        }

                        /* Poll IoT Hub Client */
                        IoTHubClient_LL_DoWork(iotHubClientHandle);
                        ThreadAPI_Sleep(1);
                    }
                    while((_fapp_azure_is_running == FNET_TRUE) && (_fapp_azure_message_counter < FAPP_CFG_AZURE_CMD_MESSAGE_COUNT));

                    /* Finish work (send, confim received messages etc.). Required before client destroy.*/
                    timestamp = fnet_timer_get_seconds();
                    while( (fnet_timer_get_seconds() - timestamp) < 2 /* sec */ )
                    {
                        IoTHubClient_LL_DoWork(iotHubClientHandle); /* Poll IoT Hub Client */
                        ThreadAPI_Sleep(1);
                    }
                }

        /* Release Azure IoT Hub client */
        IoTHubClient_LL_Destroy(iotHubClientHandle);
    }
}
#endif /* FAPP_CFG_AZURE_CMD && FNET_CFG_AZURE */
