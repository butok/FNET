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
    #error Please define FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING device connection string in fapp_user_config.h as "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>". More info: http://fnet.sourceforge.net/manual/quick_start_azure.html
#endif

/************************************************************************
*     Definitions
*************************************************************************/
typedef enum
{
    FAPP_AZURE_STATE_DISABLED = 0,  /* Disabled */
    FAPP_AZURE_STATE_ENABLED,       /* Sending/receiving Azure messages */
    FAPP_AZURE_STATE_RELEASING,     /* Releasing Azure client */
} fapp_azure_state_t;

/************************************************************************
*    Azure client example interface structure.
*************************************************************************/
typedef struct
{
    IOTHUB_CLIENT_LL_HANDLE iothub_client_handle;
    fapp_azure_state_t      state;
    fnet_time_t             state_timestamp_ms;
    fnet_bool_t             is_unauthenticated;
    fnet_service_desc_t     service_descriptor;
    fnet_shell_desc_t       shell_desc;
    fnet_index_t            message_counter;
}
fapp_azure_client_if_t;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t _fapp_azure_client_mqtt_init(fnet_shell_desc_t shell_desc);
#ifndef NO_LOGGING
    static void _fapp_azure_log_function(LOG_CATEGORY log_category, const char *file, const char *func, int line, unsigned int options, const char *format, ...);
#endif
static void _fapp_azure_on_ctrlc(fnet_shell_desc_t desc, void *cookie);
static fnet_return_t _fapp_azure_client_send_message(fapp_azure_client_if_t *azure_client_if);
static IOTHUBMESSAGE_DISPOSITION_RESULT _fapp_azure_receive_message_callback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback);
static void _fapp_azure_client_mqtt_poll(void *iotHubClientHandle_p);

/************************************************************************
*     Variables
*************************************************************************/
/* Azure client interface structure */
static fapp_azure_client_if_t   _fapp_azure_client_if;

/************************************************************************
* Ctr+C termination handler.
************************************************************************/
static void _fapp_azure_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    fapp_azure_client_if_t *azure_client_if = (fapp_azure_client_if_t *)cookie;

    /* Terminate Azure example. */
    azure_client_if->state_timestamp_ms = fnet_timer_get_ms();
    azure_client_if->state = FAPP_AZURE_STATE_RELEASING;
    fnet_shell_println(desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* Start Azure-IOT client.
************************************************************************/
void fapp_azure_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
#ifndef NO_LOGGING
    xlogging_set_log_function(_fapp_azure_log_function); /* Register Azure log function (optional) */
#endif

    /* Initialize Azure client example (over MQTT) */
    if(_fapp_azure_client_mqtt_init(desc) == FNET_OK)
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, " Azure IoT Hub Telemetry example");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Period (sec)", FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD_SEC );
#if FAPP_CFG_AZURE_CMD_MESSAGE_NUMBER != (-1)
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Message number", FAPP_CFG_AZURE_CMD_MESSAGE_NUMBER );
#else
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Message number", "infinite" );
#endif
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
    }
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
            /* Terminate Azure example. */
            _fapp_azure_client_if.state_timestamp_ms = fnet_timer_get_ms();
            _fapp_azure_client_if.state = FAPP_AZURE_STATE_RELEASING;
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

    fapp_azure_client_if_t *azure_client_if = (fapp_azure_client_if_t *)userContextCallback;

    switch(result)
    {
        case IOTHUB_CLIENT_CONNECTION_AUTHENTICATED:
            status_str = "authenticated";
            azure_client_if->is_unauthenticated = FNET_TRUE;
            break;
        case IOTHUB_CLIENT_CONNECTION_UNAUTHENTICATED:
            status_str = "unauthenticated";
            azure_client_if->is_unauthenticated = FNET_FALSE;
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

    fnet_shell_println(azure_client_if->shell_desc, "[AZURE_APP] IoT Hub Connection is %s (%s)", status_str, reason_str);
}

/************************************************************************
*  Prepare and send telemetry message to Azure IoT hub
************************************************************************/
static fnet_return_t _fapp_azure_client_send_message(fapp_azure_client_if_t *azure_client_if)
{
    static char     message_buffer[512];
    fnet_return_t   result = FNET_ERR;
    double          temperature = 20.0;
    double          humidity = 60.0;

    IOTHUB_MESSAGE_HANDLE messageHandle;

    /* Prepare and send telemetry message to Azure IoT hub */
    temperature += (rand() % 10);
    humidity += (rand() % 20);
    sprintf_s(message_buffer, sizeof(message_buffer), "{\"temperature\":%.2f,\"humidity\":%.2f}", temperature, humidity);

    /* Creates a new IoT hub message from a byte array */
    if ((messageHandle = IoTHubMessage_CreateFromByteArray((unsigned char *)message_buffer, strlen(message_buffer))) == NULL)
    {
        fnet_println("ERROR: IoTHubMessage_CreateFromByteArray() is failed!");
#if 0
        /* Terminate Azure example. */
        azure_client_if->state_timestamp_ms = fnet_timer_get_ms();
        azure_client_if->state = FAPP_AZURE_STATE_RELEASING;
#endif
    }
    else
    {
        (void)IoTHubMessage_SetMessageId(messageHandle, "MSG_ID");
        (void)IoTHubMessage_SetCorrelationId(messageHandle, "CORE_ID");
        (void)IoTHubMessage_SetContentTypeSystemProperty(messageHandle, "application%2Fjson");
        (void)IoTHubMessage_SetContentEncodingSystemProperty(messageHandle, "utf-8");

        /* Send message */
        if (IoTHubClient_LL_SendEventAsync(azure_client_if->iothub_client_handle, messageHandle, _fapp_azure_send_confirmation_callback, (void *)azure_client_if->message_counter) != IOTHUB_CLIENT_OK)
        {
            fnet_println("[AZURE_APP] ERROR: IoTHubClient_LL_SendEventAsync() is failed");
        }
        else
        {
            fnet_println("[AZURE_APP] Sending message [%d]: %s", azure_client_if->message_counter, message_buffer);
        }

        IoTHubMessage_Destroy(messageHandle);

        azure_client_if->message_counter++;

#if FAPP_CFG_AZURE_CMD_MESSAGE_NUMBER != (-1)
        if(azure_client_if->message_counter >= FAPP_CFG_AZURE_CMD_MESSAGE_NUMBER)
        {
            azure_client_if->state = FAPP_AZURE_STATE_RELEASING;
        }
#endif

        result = FNET_OK;
    }

    return result;
}

/************************************************************************
*  Start Azure client example (over MQTT)
************************************************************************/
static fnet_return_t _fapp_azure_client_mqtt_init(fnet_shell_desc_t shell_desc)
{
    fnet_memset_zero(&_fapp_azure_client_if, sizeof(_fapp_azure_client_if));
    _fapp_azure_client_if.shell_desc = shell_desc;
    int receiveContext = 0;

    /* Creates a IoT Hub client for communication with an existing IoT Hub using the specified connection string parameter. */
    _fapp_azure_client_if.iothub_client_handle = IoTHubClient_LL_CreateFromConnectionString(FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING, MQTT_Protocol);
    if (_fapp_azure_client_if.iothub_client_handle == NULL)
    {
        fnet_shell_println(shell_desc, "ERROR: iotHubClient initialization is failed!");
    }
    else
    {

#if FAPP_CFG_AZURE_CMD_TRACE /* Enable Azure message trace. */
        {
            bool traceOn = true;
            IoTHubClient_LL_SetOption(_fapp_azure_client_if.iothub_client_handle, OPTION_LOG_TRACE, &traceOn);
        }
#endif

        /* Add the trustet certificate information */
        if (IoTHubClient_LL_SetOption(_fapp_azure_client_if.iothub_client_handle, OPTION_TRUSTED_CERT, certificates) != IOTHUB_CLIENT_OK)
        {
            fnet_shell_println(shell_desc, "ERROR: Trusted certificate is failed.");
        }
        else
            /* Set the receive message call back. */
            if (IoTHubClient_LL_SetMessageCallback(_fapp_azure_client_if.iothub_client_handle, _fapp_azure_receive_message_callback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                fnet_shell_println(shell_desc, "ERROR: IoTHubClient_LL_SetMessageCallback() is failed");
            }
            else
                /* Set up the connection status callback to be invoked representing the status of the connection to IOT Hub. (Optional). */
                if (IoTHubClient_LL_SetConnectionStatusCallback(_fapp_azure_client_if.iothub_client_handle, _fapp_azure_client_connection_callback, &_fapp_azure_client_if) != IOTHUB_CLIENT_OK)
                {
                    fnet_shell_println(shell_desc, "ERROR: IoTHubClient_LL_SetConnectionStatusCallback() is failed");
                }
                else
                {

                    /* Register Azure client as a service. */
                    _fapp_azure_client_if.service_descriptor = fnet_service_register(_fapp_azure_client_mqtt_poll, &_fapp_azure_client_if);
                    if(_fapp_azure_client_if.service_descriptor)
                    {
                        fnet_shell_block(_fapp_azure_client_if.shell_desc, _fapp_azure_on_ctrlc, &_fapp_azure_client_if); /* Block the shell input.*/
                        _fapp_azure_client_if.state_timestamp_ms = fnet_timer_get_ms() + (FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD_SEC * FNET_TIMER_MS_IN_SEC);
                        _fapp_azure_client_if.state = FAPP_AZURE_STATE_ENABLED;

                        return FNET_OK;
                    }
                }

        /* Release Azure IoT Hub client if any error*/
        IoTHubClient_LL_Destroy(_fapp_azure_client_if.iothub_client_handle);
    }

    return FNET_ERR;
}

/************************************************************************
*   Azure client example poll
************************************************************************/
static void _fapp_azure_client_mqtt_poll(void *fapp_azure_client_if_p)
{
    fapp_azure_client_if_t *azure_client_if = (fapp_azure_client_if_t *)fapp_azure_client_if_p;

    switch(azure_client_if->state)
    {
        case FAPP_AZURE_STATE_ENABLED:
            if((azure_client_if->is_unauthenticated == FNET_TRUE)
               && ((fnet_timer_get_ms() - azure_client_if->state_timestamp_ms) > (FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD_SEC * FNET_TIMER_MS_IN_SEC) ))
            {
                if(_fapp_azure_client_send_message(azure_client_if) == FNET_ERR)
                {
                    break;
                }
                else
                {
                    azure_client_if->state_timestamp_ms = fnet_timer_get_ms();
                }
            }

            /* Poll IoT Hub Client */
            IoTHubClient_LL_DoWork(azure_client_if->iothub_client_handle);

            break;
        case FAPP_AZURE_STATE_RELEASING:
            /* Finish work (send, confim received messages etc.). Required before client destroy.*/
            IoTHubClient_LL_DoWork(azure_client_if->iothub_client_handle); /* Poll IoT Hub Client */

            /* Finish work (send, confim received messages etc.). Required before client destroy.*/
            if( (fnet_timer_get_ms() - azure_client_if->state_timestamp_ms) > (2 * FNET_TIMER_MS_IN_SEC) /* sec */ )
            {
                /* Release Azure IoT Hub client */
                IoTHubClient_LL_Destroy(azure_client_if->iothub_client_handle);
                azure_client_if->state = FAPP_AZURE_STATE_DISABLED;

                fnet_service_unregister(azure_client_if->service_descriptor);

                fnet_shell_unblock(azure_client_if->shell_desc); /* Unblock shell, just in case.*/
            }
            break;
        default:
            break;
    }
}

#endif /* FAPP_CFG_AZURE_CMD && FNET_CFG_AZURE */
