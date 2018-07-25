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
* FNET time(), defined by <time.h>.
*
***************************************************************************/

//!!!!!!!!!!!!!! UNDER DEVELOPMENT !!!!!!!!!!!!!!!!!!!!

#include "fapp.h"

#if FAPP_CFG_TIME
#include "fapp_prv.h"
#include "fapp_time.h"
#include "fnet.h"

/************************************************************************
*     Definitions.
*************************************************************************/

static time_t       _fapp_time_start = 0;
static fnet_time_t  _fapp_timer_start = 0;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static time_t _fapp_time_build(void);

/************************************************************************
* DESCRIPTION: Set current time as the elapsed time in seconds since 00:00:00, January 1, 1970.
************************************************************************/
void fapp_time_set(time_t sec)
{
    /* Get the timer counter value in seconds */
    _fapp_timer_start = fnet_timer_get_seconds();
    _fapp_time_start = sec;
}
/************************************************************************
* DESCRIPTION: Get current time as the elapsed time in seconds since 00:00:00, January 1, 1970.
************************************************************************/
time_t time(time_t *sec)
{
    time_t result;

    if(_fapp_time_start == 0) /* Never initialized before */
    {
        time_t time_buld = _fapp_time_build();
        fapp_time_set(time_buld);
    }

    result = _fapp_time_start + (fnet_timer_get_seconds() - _fapp_timer_start);

    if(sec)
    {
        *sec = result;
    }
    return result;
}

/************************************************************************
*      Build time Macros.
*************************************************************************/
#define FNET_BUILD_YEAR  ((__DATE__[0] == '?') ? 2018 : (((__DATE__[ 7] - '0') * 1000) + ((__DATE__[ 8] - '0') * 100) + ((__DATE__[ 9] - '0') * 10) + (__DATE__[10] - '0')))
#define FNET_BUILD_MONTH ((__DATE__[0] == '?') ? 6 : ((__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n') ?  1 : \
                          (__DATE__[0] == 'F') ?  2 : \
                          (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') ?  3 : \
                          (__DATE__[0] == 'A' && __DATE__[1] == 'p') ?  4 : \
                          (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') ?  5 : \
                          (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') ?  6 : \
                          (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') ?  7 : \
                          (__DATE__[0] == 'A' && __DATE__[1] == 'u') ?  8 : \
                          (__DATE__[0] == 'S') ?  9 : \
                          (__DATE__[0] == 'O') ? 10 : \
                          (__DATE__[0] == 'N') ? 11 : \
                          (__DATE__[0] == 'D') ? 12 : 6 ) )
#define FNET_BUILD_DAY   ((__DATE__[0] == '?') ? 14 : (((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + (__DATE__[5] - '0')))
#define FNET_BUILD_HOUR  ((__TIME__[0] == '?') ? 0 :  ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0'))
#define FNET_BUILD_MIN   ((__TIME__[0] == '?') ? 0 :  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0'))
#define FNET_BUILD_SEC   ((__TIME__[0] == '?') ? 0 :  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0'))

/************************************************************************
* DESCRIPTION: Get buld time as the elapsed time in seconds since 00:00:00, January 1, 1970.
************************************************************************/
static time_t _fapp_time_build(void)
{
    time_t      result;
    struct tm   date_time;

    /* date and time default value. */
    date_time.tm_sec = FNET_BUILD_SEC;          /* seconds after the minute	0-61 */
    date_time.tm_min = FNET_BUILD_MIN;          /* minutes after the hour	0-59 */
    date_time.tm_hour = FNET_BUILD_HOUR;        /* hours since midnight	0-23 */
    date_time.tm_mday = FNET_BUILD_DAY;         /* day of the month	1-31 */
    date_time.tm_mon = FNET_BUILD_MONTH - 1;    /* months since January	0-11 */
    date_time.tm_year = FNET_BUILD_YEAR - 1900; /* years since 1900 */
    date_time.tm_wday = -1;                     /* days since Sunday	0-6 */
    date_time.tm_yday = -1;                     /* days since January 1	0-365 */
    date_time.tm_isdst = -1;                    /* Daylight Saving Time flag */

    /* __DATE__: This macro expands to a string constant that describes the date on which the preprocessor is being run. The string constant contains eleven characters and looks
    like "Feb 12 1996". If the day of the month is less than 10, it is padded with a space on the left.*/

    /* __TIME__: This macro expands to a string constant that describes the time at which the preprocessor is being run. The string constant contains eight characters and looks like "23:59:01"*/


    result = mktime(&date_time);

    return result;
}

/************************************************************************
* DESCRIPTION: Print HTTP server status
*************************************************************************/
void fapp_time_info(fnet_shell_desc_t desc)
{
    time_t current_time = time(FNET_NULL);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Time", ctime(&current_time));
}


#endif /* FAPP_CFG_TIME */
