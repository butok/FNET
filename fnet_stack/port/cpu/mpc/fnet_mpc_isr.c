/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2011 by Andrey Butok,Gordon Jahn. Freescale Semiconductor, Inc.
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
**********************************************************************/ /*!
*
* @file fnet_mpc_isr.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief Lowest level ISR routine for MPC.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MPC

/************************************************************************
* NAME: void fnet_cpu_isr(void);
*
* DESCRIPTION: This handler is executed on every FNET interrupt
*              (from ethernet and timer module).
*              Extructs vector number and calls fnet_isr_handler().
*************************************************************************/
void fnet_cpu_isr(void)
{
#if FNET_CFG_CPU_INDEX==0
    /* ICSR register [VECTACTIVE].*/
#if FNET_CFG_CPU_MPC5744P
    fnet_uint16_t vector_number = (fnet_uint16_t) ((FNET_MPC_INTC_IACKR_PRC0 & 0xFFC) >> 2);
#else
    fnet_uint16_t vector_number = (fnet_uint16_t) ((FNET_MPC_INTC_IACKR_PRC0 & 0x7FC) >> 2);
#endif
#else
    /* ICSR register [VECTACTIVE].*/
    fnet_uint16_t vector_number = (fnet_uint16_t) ((FNET_MPC_INTC_IACKR_PRC1 & 0x7FC) >> 2);
#endif

    /* Call FNET isr handler.*/
    fnet_isr_handler( vector_number );

#if FNET_CFG_CPU_INDEX==0
    FNET_MPC_INTC_EOIR_PRC0 = 0;
#else
    FNET_MPC_INTC_EOIR_PRC1 = 0;
#endif
}

#endif /*FNET_MPC*/
