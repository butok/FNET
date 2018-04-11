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
*  FNET Shell Demo implementation, FreeRTOS-specific.
*
***************************************************************************/

#include "fapp_freertos.h"
#include "fapp.h"

#if FAPP_CFG_FREERTOS

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#if FNET_CFG_MULTITHREADING
#include "semphr.h"
static fnet_return_t fapp_freertos_mutex_init( fnet_mutex_t *counter);
static void fapp_freertos_mutex_release( fnet_mutex_t *counter );
static void fapp_freertos_mutex_lock( fnet_mutex_t *counter );
static void fapp_freertos_mutex_unlock( fnet_mutex_t *counter );
static const fnet_mutex_api_t fapp_freertos_mutex_api =
{
    .mutex_init = fapp_freertos_mutex_init,
    .mutex_release = fapp_freertos_mutex_release,
    .mutex_lock = fapp_freertos_mutex_lock,
    .mutex_unlock = fapp_freertos_mutex_unlock,
};
#endif

/* RX activity event. */
#if FNET_CFG_SOCKET_CALLBACK_ON_RX
    #define FAPP_FREERTOS_EVENT_RX  (0x1)
    static EventGroupHandle_t fapp_freertos_event_group;
#endif

/************************************************************************
* DESCRIPTION: FreeRTOS application poll.
************************************************************************/
void fapp_freertos_poll(void)
{
#if FNET_CFG_SOCKET_CALLBACK_ON_RX /* Sleep for some time or for the FNET RX event */

    /* Wait a maximum of FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS ms for either any bit to be set within
        the event group.  Clear the bits before exiting. */
    xEventGroupWaitBits(
        fapp_freertos_event_group,   /* The event group being tested. */
        FAPP_FREERTOS_EVENT_RX, /* The bits within the event group to wait for. */
        pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
        pdFALSE,       /* Don't wait for both bits, either bit will do. */
        FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS / portTICK_PERIOD_MS ); /* Ticks to wait for either bit to be set. */
    /* Returned on Socket RX event or ticks passed.*/

#else /* Sleep for some time. */

    vTaskDelay(FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS / portTICK_PERIOD_MS);

#endif
}

/************************************************************************
* DESCRIPTION: Socket RX callback.
************************************************************************/
#if FNET_CFG_SOCKET_CALLBACK_ON_RX
static void fapp_freertos_socket_callback_on_rx(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    if(xEventGroupSetBitsFromISR(fapp_freertos_event_group, FAPP_FREERTOS_EVENT_RX, &xHigherPriorityTaskWoken) != pdFAIL)
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}
#endif

/************************************************************************
* DESCRIPTION: FreeRTOS task.
************************************************************************/
static void fapp_freertos_task(void *params)
{
    /* FNET demo application */
    fapp_main();
}

/************************************************************************
* DESCRIPTION: Main entry point of the FreeRTOS shell demo.
************************************************************************/
void fapp_freertos_main( void )
{
    /* Create FNET demo application task */
    if(xTaskCreate(
           fapp_freertos_task,  /* pointer to the task */
           FAPP_CFG_NAME, /* task name for kernel awareness debugging */
           FAPP_CFG_FREERTOS_TASK_STACK_SIZE / sizeof(portSTACK_TYPE), /* task stack size */
           NULL, /* optional task startup argument */
           FAPP_CFG_FREERTOS_TASK_PRIORITY,  /* initial priority */
           NULL /* optional task handle to create */
       ) != pdPASS)
    {
        fnet_println("[FREERTOS] Failed to create task."); /* Probably out of memory */
    }
    else
    {
#if FNET_CFG_SOCKET_CALLBACK_ON_RX
        /* Attempt to create the event group. */
        fapp_freertos_event_group = xEventGroupCreate();

        /* Was the event group created successfully? */
        if( fapp_freertos_event_group == NULL )
        {
            /* The event group was not created because there was insufficient heap available. */
            fnet_println("[FREERTOS] Failed to create event group.");
        }
        else
        {
            /* Registers the "socket layer activity" event handler.*/
            fnet_socket_set_callback_on_rx(fapp_freertos_socket_callback_on_rx);
#endif

            fnet_println("[FREERTOS] Starting scheduler.");
            vTaskStartScheduler();

            /* The code should never reach here. */
#if FNET_CFG_SOCKET_CALLBACK_ON_RX
        }
#endif
    }
}

/************************************************************************
* DESCRIPTION: FreeRTOS-specific parameters.
************************************************************************/
void fapp_freertos_params_init(struct fnet_init_params  *init_params)
{
#if FNET_CFG_MULTITHREADING
    init_params->mutex_api = &fapp_freertos_mutex_api;
#endif
}

#if FNET_CFG_MULTITHREADING

/************************************************************************
* DESCRIPTION: Create a new mutex
************************************************************************/
static fnet_return_t fapp_freertos_mutex_init( fnet_mutex_t *mutex)
{
    SemaphoreHandle_t   xSemaphore;
    fnet_return_t       result = FNET_ERR;

    if(mutex )
    {
        xSemaphore = xSemaphoreCreateRecursiveMutex();

        if(xSemaphore)
        {
            *mutex = (fnet_mutex_t)xSemaphore;
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Delete a mutex.
************************************************************************/
static void fapp_freertos_mutex_release( fnet_mutex_t *mutex )
{
    SemaphoreHandle_t xSemaphore = (SemaphoreHandle_t) * mutex;
    vSemaphoreDelete(xSemaphore);
}

/************************************************************************
* DESCRIPTION: Lock a mutex.
************************************************************************/
static void fapp_freertos_mutex_lock( fnet_mutex_t *mutex )
{
    SemaphoreHandle_t   xSemaphore = (SemaphoreHandle_t) * mutex;
    BaseType_t          result;

    result = xSemaphoreTakeRecursive(xSemaphore, portMAX_DELAY);

    if(result != pdPASS )
    {
        FNET_ASSERT_CRITICAL(0);
    }
}

/************************************************************************
* DESCRIPTION: Unlock a mutex.
************************************************************************/
static void fapp_freertos_mutex_unlock( fnet_mutex_t *mutex )
{
    SemaphoreHandle_t   xSemaphore = (SemaphoreHandle_t) * mutex;
    BaseType_t          result;

    result = xSemaphoreGiveRecursive(xSemaphore);

    if(result != pdPASS )
    {
        FNET_ASSERT_CRITICAL(0);
    }
}
#endif /* FNET_CFG_MULTITHREADING */

#endif /* FAPP_CFG_FREERTOS */
