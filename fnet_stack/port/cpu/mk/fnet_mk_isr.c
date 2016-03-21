/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
**********************************************************************/
/*!
*
* @file fnet_mk_isr.c
*
* @author Andrey Butok
*
* @brief Lowest level ISR routine for Kinetis.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MK

/************************************************************************
* NAME: void fnet_cpu_isr(void);
*
* DESCRIPTION: This handler is executed on every FNET interrupt
*              (from ethernet and timer module).
*              Extructs vector number and calls fnet_isr_handler().
*************************************************************************/
void fnet_cpu_isr(void)
{
    /* ICSR register [VECTACTIVE].*/
    fnet_uint8_t vector_number = (*(volatile fnet_uint8_t *)(0xE000ED04u));

    /* Call FNET isr handler.*/
    fnet_isr_handler((fnet_uint32_t)vector_number );

    return;
}

#endif /*FNET_MK*/


