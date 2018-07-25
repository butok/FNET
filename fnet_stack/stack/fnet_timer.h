/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community
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
*  FNET Timer API.
*
***************************************************************************/

#ifndef _FNET_TIMER_H

#define _FNET_TIMER_H

/*! @addtogroup fnet_timer
* An application can use the @ref fnet_timer_get_ticks() function to get a number of
* ticks (periods, defined by the @ref FNET_TIMER_PERIOD_MS) since the
* hardware timer startup.
*/

/*! @{ */

/**************************************************************************/ /*!
 * @brief Timer period in milliseconds (period of one timer tick).
 ******************************************************************************/
#define FNET_TIMER_PERIOD_MS        (10U) /* It may be 1-100ms */

/**************************************************************************/ /*!
 * @brief Number of timer ticks in one hour.
 ******************************************************************************/
#define FNET_TIMER_TICKS_IN_HOUR    ((1000U*60U*60U)/FNET_TIMER_PERIOD_MS)

/**************************************************************************/ /*!
 * @brief Number of timer ticks in one minute.
 ******************************************************************************/
#define FNET_TIMER_TICKS_IN_MIN     ((1000U*60U)/FNET_TIMER_PERIOD_MS)

/**************************************************************************/ /*!
 * @brief Number of timer ticks in one second.
 ******************************************************************************/
#define FNET_TIMER_TICKS_IN_SEC     (1000U/FNET_TIMER_PERIOD_MS)

#if defined(__cplusplus)
extern "C" {
#endif

/**************************************************************************/ /*!
 * @brief Unsigned integer type representing time uinits.
 * It can be ticks, seconds or milliseconds.
 ******************************************************************************/
typedef fnet_uint32_t fnet_time_t;

/***************************************************************************/ /*!
 *
 * @brief    Get the timer counter value in ticks.
 *
 * @return   This function returns a current value of the timer counter in ticks.
 *
 * @see fnet_timer_get_seconds()
 *
 ******************************************************************************
 *
 * This function returns a current value of the timer counter that
 * contains a number of periods from the moment of the hardware
 * timer initialization (it's done in the FNET stack initialization).@n
 * The period of one timer tick is defined by the @ref FNET_TIMER_PERIOD_MS.
 *
 ******************************************************************************/
fnet_time_t fnet_timer_get_ticks( void );

/***************************************************************************/ /*!
 *
 * @brief    Get the timer counter value in seconds.
 *
 * @return   This function returns a current value of the timer counter
 *           in seconds.
 *
 * @see fnet_timer_get_ticks()
 *
 ******************************************************************************
 *
 * This function returns a current value of the timer counter in seconds,
 * from the moment of the hardware timer initialization
 * (it's done in the FNET stack initialization).
 *
 ******************************************************************************/
fnet_time_t fnet_timer_get_seconds( void );

/***************************************************************************/ /*!
 *
 * @brief    Get the timer counter value in milliseconds.
 *
 * @return   This function returns a current value of the timer counter
 *           in milliseconds.
 *
 * @see fnet_timer_get_ms()
 *
 ******************************************************************************
 *
 * This function returns a current value of the timer counter in milliseconds,
 * from the moment of the hardware timer initialization
 * (it's done in the FNET stack initialization).@n
 *
 ******************************************************************************/
fnet_time_t fnet_timer_get_ms( void );

/***************************************************************************/ /*!
 *
 * @brief    Convert milliseconds to timer ticks.
 *
 * @param    time_ms Time value in milliseconds.
 *
 * @return   This function returns the time value in timer ticks.
 *
 ******************************************************************************
 *
 * This function converts the time value @c time_ms in milliseconds to the time
 * value in timer ticks.@n
 * The period of one timer tick is defined by the @ref FNET_TIMER_PERIOD_MS.
 *
 ******************************************************************************/
fnet_time_t fnet_timer_ms2ticks( fnet_time_t time_ms );

/***************************************************************************/ /*!
 *
 * @brief    Calculate an interval between two moments in time.
 *
 * @param    start Start time in ticks.
 *
 * @param    end   End time in ticks.
 *
 * @return   This function returns an interval value between two time moments
 *           (in timer ticks).
 *
 ******************************************************************************
 *
 * This function calculates an interval between two moments in time, @c start
 * and @c end.
 * This function takes into account also a possible counter overrun @c (start>end).
 *
 ******************************************************************************/
fnet_time_t fnet_timer_get_interval( fnet_time_t start, fnet_time_t end );

/***************************************************************************/ /*!
 *
 * @brief    Perform a delay for the given number of timer ticks.
 *
 * @param    delay_ticks Time value used for delay, in ticks.
 *
 ******************************************************************************
 *
 * This function performs a delay for a given number of timer ticks.
 * The function is blocked, till the @c delay_ticks expires.
 *
 ******************************************************************************/
void fnet_timer_delay( fnet_time_t delay_ticks );

/***************************************************************************/ /*!
 *
 * @brief    Poll SW timers.
 *
 * @see FNET_CFG_TIMER_POLL_AUTOMATIC
 *
 ******************************************************************************
 *
 * This function checks timeouts for TCP, ARP, IP and other stack activities.@n
 * The user application should call this function periodically,
 * after the FNET stack initialization, and only if @ref FNET_CFG_TIMER_POLL_AUTOMATIC is 0.@n
 * Recommended polling period is 100ms or less.@n
 * If @ref FNET_CFG_TIMER_POLL_AUTOMATIC is 1, this function is called automatically
 * by FNET in HW interrupt.
 *
 ******************************************************************************/
void fnet_timer_poll(void);

#if FNET_CFG_TIME || defined(__DOXYGEN__)
#include <time.h>

#define fnet_time   time  /* Compliance with <time.h> */

/***************************************************************************/ /*!
 *
 * @brief    Set time since the Epoch (00:00:00 UTC, January 1, 1970).
 *
 * @param [in] sec     Time since the Epoch (00:00:00 UTC, January 1, 1970), in seconds.
 *
 * @see fnet_time(), FNET_CFG_TIME
 *
 ******************************************************************************
 *
 * This function sets the time since the Epoch (00:00:00 UTC, January 1, 1970),
 * in seconds. @n
 * This function is available only if FNET_CFG_TIME is set.
 *
 ******************************************************************************/
void fnet_time_set(time_t sec);

/***************************************************************************/ /*!
 *
 * @brief    Get time since the Epoch (00:00:00 UTC, January 1, 1970).
 *
 * @param [out] sec     If the pointer is not NULL, it will get the time since the Epoch (00:00:00 UTC, January 1, 1970), in seconds.
 *
 * @return   This function returns time since the Epoch, in seconds.
 *
 * @see fnet_time_set(), FNET_CFG_TIME
 *
 ******************************************************************************
 *
 * This function returns the time since the Epoch (00:00:00 UTC, January 1, 1970),
 * in seconds. It is defined by standard C library @c time.h, and may be used as time(). @n
 * This function is available only if FNET_CFG_TIME is set.
 *
 ******************************************************************************/
time_t fnet_time(time_t *sec);

#endif /* FNET_CFG_TIME */

/*! @} */

#ifndef  FNET_HW_TIMER_INIT
#define FNET_HW_TIMER_INIT    fnet_cpu_timer_init
#endif
#ifndef  FNET_HW_TIMER_RELEASE
#define FNET_HW_TIMER_RELEASE fnet_cpu_timer_release
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FNET_TIMER_H */
