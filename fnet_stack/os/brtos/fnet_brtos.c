/**************************************************************************
*
* Copyright 2012-2013 by Andrey Butok. FNET Community.
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
* @file fnet_brtos.c
*
* @author Andrey Butok, Gustavo Denardin
*
* @date Dec-19-2012
*
* @version 0.1.3.0
*
* @brief Default BRTOS-specific functions. @n
*        Experimental. Not supported.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_OS && FNET_CFG_OS_BRTOS

#include "BRTOS.h"

/************************************************************************
*     Global Data Structures 
*************************************************************************/
#define	FNET_BRTOS_MUTEX_PRIORITY	    (11)    /* Must be set to a priority 
                                                 * higher than the highest priority 
                                                 * task that will use fnet. */
#define FNET_BRTOS_SEMAPHORE_TIMEOUT    (3)                                             

/************************************************************************
*     Global Data Structures
*************************************************************************/
BRTOS_Sem       *FnetSemaphore;
BRTOS_Mutex     *FnetMutex;

/************************************************************************
* NAME: fnet_os_isr;
*
* DESCRIPTION: This handler is executed on every FNET interrupt 
*              (from ethernet and timer module).
*              Extructs vector number and calls fnet_isr_handler().
*************************************************************************/
void fnet_os_isr(void)
{
	  /*******************************
	   * OS-specific Interrupt Enter.
	   *******************************/
	  OS_SAVE_ISR();
	  OS_INT_ENTER();

	  /* Call original CPU handler*/
	  fnet_cpu_isr();
	  
	  /*******************************
	   * Interrupt Exit.
	   *******************************/
	  OS_INT_EXIT();  
	  OS_RESTORE_ISR();
}

/************************************************************************
* NAME: fnet_os_event_init
*
* DESCRIPTION: 
*************************************************************************/
int fnet_os_event_init(void)
{
	 if (OSSemCreate(0,&FnetSemaphore) == ALLOC_EVENT_OK)
		 return FNET_OK;
	 else
		 return FNET_ERR;
}

/************************************************************************
* NAME: fnet_os_event_wait
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_event_wait(void)
{
	(void)OSSemPend(FnetSemaphore,FNET_CFG_OS_SEMAPHORE_TIMEOUT);
}

/************************************************************************
* NAME: fnet_os_event_raise
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_event_raise(void)
{
	(void)OSSemPost(FnetSemaphore);
}

/************************************************************************
* NAME: fnet_os_mutex_init
*
* DESCRIPTION: 
*************************************************************************/
int fnet_os_mutex_init(void)
{
	 if (OSMutexCreate(&FnetMutex, FNET_BRTOS_MUTEX_PRIORITY) == ALLOC_EVENT_OK)
		 return FNET_OK;
	 else
		 return FNET_ERR;
}

/************************************************************************
* NAME: fnet_os_mutex_lock;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_lock(void)
{
	OSMutexAcquire(FnetMutex);
}

/************************************************************************
* NAME: fnet_os_mutex_unlock;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_unlock(void)
{
	OSMutexRelease(FnetMutex);
}

/************************************************************************
* NAME: fnet_os_mutex_release;
*
* DESCRIPTION: 
*************************************************************************/
void fnet_os_mutex_release(void)
{
	
}


#endif /* FNET_CFG_OS && FNET_CFG_OS_BRTOS */
