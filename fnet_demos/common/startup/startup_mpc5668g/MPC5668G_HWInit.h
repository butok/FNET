
/*
 *
 * FILE : MPC5668G_HWInit.h
 *
 * DESCRIPTION:
 *  This file contains all MPC5668G derivative needed initializations, 
 *  and all initializations for the MPC5668G boards which are supported.
 *  This includes setting up the External Bus Interface to allow access to 
 *  memory on the external bus, and ensuring there is a valid entry in the
 *  MMU for the external memory access.
 */

#ifndef _MPC5668G_HWINIT_H_
#define _MPC5668G_HWINIT_H_

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/* MPC5668G derivative specific hardware initialization */
/*******************************************************/ 

__asm void INIT_Derivative(void); 

/*******************************************************/
/* MPC5668G boards specific hardware initializations    */
/*******************************************************/

/*----------------------------------------------------------------------------*/
/* Used Board selection                                                       */
/*----------------------------------------------------------------------------*/

/* Supported Boards */
#define NO_EXTERNAL_MEMORY            0
#define MPC5633DEMO_AXM_0321          1

/* Used board */
#define INIT_USED_BOARD NO_EXTERNAL_MEMORY

#ifndef INIT_USED_BOARD
#pragma error INIT_USED_BOARD should be defined !
#endif

/*----------------------------------------------------------------------------*/
/* Function declarations                                                      */
/*----------------------------------------------------------------------------*/

/* call this function to setup the external bus, 
   chip selects and MMU entries for the selected board
*/
__asm void INIT_ExternalBusAndMemory(void);

/*----------------------------------------------------------------------------*/
/* MMU Table Entries Defines                                                  */
/*----------------------------------------------------------------------------*/

/**
 * Generate MMU Assist 0 value from the parameters provided.
 * In accordance with the PowerPC Zen core specification the TLBSEL value 
 * is always set to 01b to maintain future compatibility.
 *
 */
#define MAS0_VALUE(eselcam) ((unsigned long)(0x10000000 | (eselcam << 16)))

/** 
 * Generate MMU Assist 1 value from the parameters provided
 *
 * parameter valid:   1 if the MMU entry is valid, otherwise \c 0 (invalid).
 * parameter iprot:   Invalidation protection value
 * parameter tid:     the translation ID
 * parameter ts:      the translation space value
 * parameter tsize:   the translation size
 */
#define MAS1_VALUE(valid, iprot, tid, ts, tsize) \
          ((unsigned long)((valid << 31) | (iprot << 30) | (tid << 16) | (ts << 12) | (tsize << 8)))

/** V TLB valid bit */
#define V_INVALID 0
#define V_VALID   1

/** IPROT TLB Invalidate protect bit */
#define IPROT_NOTPROTECTED 0
#define IPROT_PROTECTED    1

 /** Translation ID defines the TID as global and matches all process IDs */
#define TID_GLOBAL          0

/** Translation size */
#define TSIZE_4KB           1
#define TSIZE_16KB          2
#define TSIZE_64KB          3
#define TSIZE_256KB         4
#define TSIZE_1MB           5
#define TSIZE_4MB           6
#define TSIZE_16MB          7
#define TSIZE_64MB          8
#define TSIZE_256MB         9

/**
 * Generate MMU Assist 2 value from the parameters provided
 *
 * Effective Page Number (start address of logical memory region) 
 * must be computed directely in the assembly code.
 * 
 * parameter   w:   Write-through Required
 * parameter   i:   Cache Inhibited
 * parameter   m:   Memory Coherency Required
 * parameter   g:   Guarded
 * parameter   e:   Endianness
 */
#define MAS2_FLAGS(sharen, w, i, m, g, e) \
  ((unsigned long)((sharen << 9)| (w << 4)| (i << 3)| (m << 2)| (g << 1)| (e)))

/** MAS2[SHAREN]: Cache fills do not use the shared cache state for this page. */
#define SHARED_CACHE_STATE_NOT_USED   0
/** MAS2[SHAREN]: Cache fills use the shared cache state for this page. */
#define SHARED_CACHE_STATE_USED       1

/** MAS2[W]: Update data in the cache only */
#define WRITE_BACK          0
/** MAS2[W]: All stores performed are written through to memory */
#define WRITE_THROUGH       1

/** MAS2[I]: The page is considered cacheable */
#define CACHEABLE           0
/** MAS2[I]: The page is cache-inhibited */
#define CACHE_INHIBIT       1

/** MAS2[M]: Memory Coherence is not-required */
#define MEM_COHERENCE_NREQ  0
/** MAS2[M]: Memory Coherence is required */
#define MEM_COHENRECE_REQ   1

/** MAS2[G]: Access to page is not guarded */
#define NOT_GUARDED         0
/** MAS2[G]: All loads and stores are performed without speculation */
#define GUARDED             1

/** MAS2[E]: Page is accessed in big-endian order */
#define BIG_ENDIAN          0
/** MAS2[E]: Page is accessed in little-endian order */
#define LITTLE_ENDIAN       1
 
/**
 * Generate MMU Assist 3 flags from the parameters provided
 *
 * Real Page Number (start address of physical memory region) 
 * must be computed directely in the assembly code
 *
 * parameter permissions:  Permission bits
 */
#define MAS3_FLAGS(permissions) ((unsigned long)(permissions))

/** MAS3[U/S{XWR}]: Read. Write and Execute permission */
#define READ_WRITE_EXECUTE  0x3f
/** MAS3[U/S{XWR}]: Read and Execute permission */
#define READ_EXECUTE        0x33


#ifdef __cplusplus
}
#endif

#endif
