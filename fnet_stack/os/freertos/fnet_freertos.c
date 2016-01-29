/**************************************************************************
*
* Copyright 2012 by Christophe Graulle, Andrey Butok. FNET Community.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the FNET project give you
* permission to link the FNET sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the FNET sources, you may extend this exception 
* to your version of the FNET sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/ /*!
*
* @file fnet_freertos.c
*
* @author Christophe Graulle, Andrey Butok
*
* @brief Default FreeRTOS-specific functions. @n
*        Experimental. Not supported.
*
***************************************************************************/ 

#include	"fnet.h"

#if FNET_CFG_OS && FNET_CFG_OS_FREERTOS

#include	"FreeRTOS.h"
#include	"semphr.h"

#if FNET_CFG_OS_EVENT

xSemaphoreHandle FNetSemaphore = NULL;
portBASE_TYPE l = pdFALSE;

int fnet_os_event_init(void)
{
    if (FNetSemaphore == NULL)
    {
        vSemaphoreCreateBinary( FNetSemaphore);

        if (FNetSemaphore == NULL)
            return FNET_ERR;
        else
            return FNET_OK;
    }
    else
    {
        return FNET_OK;
    }
}

void fnet_os_event_wait(void)
{
    xSemaphoreTake( FNetSemaphore, portMAX_DELAY);
}

void fnet_os_event_raise(void)
{
    portBASE_TYPE    ret;
    
    ret = xSemaphoreGiveFromISR( FNetSemaphore, &l);
    
    
    portEND_SWITCHING_ISR( l );
    
    if ( ret != pdTRUE )
    {
        ret = 1;
    }
}
#endif /* FNET_CFG_OS_EVENT */


#if FNET_CFG_OS_MUTEX

/************************************************************************
*     Globals
*************************************************************************/
xSemaphoreHandle FNetMutex;

/************************************************************************
* NAME: fnet_os_mutex_init
*
* DESCRIPTION: 
*************************************************************************/
int fnet_os_mutex_init(void)
{
	FNetMutex = xSemaphoreCreateRecursiveMutex();
	if ( FNetMutex == NULL )
		return FNET_ERR;
	else
		return FNET_OK;
}

/************************************************************************
* NAME: fnet_os_mutex_lock;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_lock(void)
{
	xSemaphoreTakeRecursive( FNetMutex, portMAX_DELAY );
}

/************************************************************************
* NAME: fnet_os_mutex_unlock;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_unlock(void)
{
	xSemaphoreGiveRecursive( FNetMutex );
}

/************************************************************************
* NAME: fnet_os_mutex_release;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_release(void)
{
	vSemaphoreDelete( FNetMutex );
}
#endif /* FNET_CFG_OS_MUTEX */

#endif
