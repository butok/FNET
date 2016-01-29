/**************************************************************************
*
* Copyright 2013 by Jon Elliott, Andrey Butok. FNET Community.
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
* @file fnet_ucosIII.c
*
* @author Jon Elliott, Andrey Butok
*
* @brief Default uCOS-III specific functions. @n
*        Experimental. Not supported.
*
***************************************************************************/ 

#include	"fnet.h"

#if FNET_CFG_OS && FNET_CFG_OS_UCOSIII && FNET_MCF


/* <PUT HERE uCOS-III include/header files.> */ 

#if FNET_CFG_OS_TIMER

/************************************************************************
* NAME: fnet_os_timer_init
*
* DESCRIPTION: Starts OS-Timer/Event. delay_ms - period of timer (ms).
*************************************************************************/
int fnet_os_timer_init( unsigned int period_ms )
{
	/* <PUT HERE uCOS-III event initialization, etc.> */ 
}

/************************************************************************
* NAME: fnet_os_timer_release
*
* DESCRIPTION: Releases OS-Timer/Event.
*              
*************************************************************************/
void fnet_os_timer_release( void )
{
	/* <PUT HERE uCOS-III event release, etc.> */ 
}

#endif /* FNET_CFG_OS_TIMER */

#endif /* FNET_CFG_OS && FNET_CFG_OS_UCOSIII && FNET_MCF */
