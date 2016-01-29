/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "arm_cm4.h"

#include "fnet.h"

/* 
 * Include the platform specific header file 
 */
#include "k60_tower.h"

/* 
 * Include the cpu specific header file 
 */
#if (defined(CPU_MK40N512VMD100))
  #include "MK40N512VMD100.h"
#elif (defined(CPU_MK60N512VMD100))
  #include "MK60N512VMD100.h"
#else
  #error "No valid CPU defined"
#endif


/* 
 * Include common utilities
 */
#include "startup.h"

#if FNET_CFG_COMP_IAR
  #include "intrinsics.h"
#endif /*FNET_CFG_COMP_IAR*/
/********************************************************************/

#endif /* _COMMON_H_ */
