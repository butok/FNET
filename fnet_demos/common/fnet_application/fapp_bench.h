/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
*  FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_BENCH_H_

#define _FAPP_BENCH_H_

#include "fapp.h"

#if FAPP_CFG_BENCH_CMD && FNET_CFG_BENCH_SRV && FNET_CFG_BENCH_CLN

/************************************************************************
* Benchmark definitions
************************************************************************/
#define FAPP_BENCH_TX_MESSAGE_SIZE_DEFAULT       (1472)   /* Default message size */
#define FAPP_BENCH_TX_MESSAGE_NUMBER_DEFAULT     (10000)  /* Default number of messages */

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_bench_cln_cmd( fnet_shell_desc_t shell_desc, fnet_index_t argc, fnet_char_t **argv );

void fapp_bench_srv_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_bench_srv_info(fnet_shell_desc_t desc);
void fapp_bench_srv_release(void);

#if defined(__cplusplus)
}
#endif

#endif

#endif
