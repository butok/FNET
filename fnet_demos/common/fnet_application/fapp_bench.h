/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fapp_bench.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_BENCH_H_

#define _FAPP_BENCH_H_

#include "fapp.h"

#if FAPP_CFG_BENCH_CMD

/************************************************************************
* Benchmark definitions
************************************************************************/
#define FAPP_BENCH_PORT                         (FNET_HTONS(7007))      /* Port used by the benchmark application (in network byte order).*/
#define FAPP_BENCH_PACKET_SIZE_MAX              (8*1024)    /* Defines size of Applacation and Socket TX&RX buffers.*/
#define FAPP_BENCH_TX_PACKET_SIZE_DEFAULT       (1472)
#define FAPP_BENCH_TX_PACKET_NUMBER_DEFAULT     (10000)
#define FAPP_BENCH_TX_ITERATION_NUMBER_DEFAULT  (1)
#define FAPP_BENCH_TX_ITERATION_NUMBER_MAX      (10000)

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_benchrx_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_benchtx_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if defined(__cplusplus)
}
#endif

#endif


#endif
