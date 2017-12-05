
/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the definitions of the generic
*   PCB (Packet Control Block) structure.  
*   Since this structure is common to
*   various protocols, this file is distributed with
*   several products.
*
*
*END************************************************************************/
#ifndef __pcb_h__
#define __pcb_h__

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */


#define __PCB__

#define PCB_free(pcb_ptr)  ((pcb_ptr)->FREE(pcb_ptr))

/*--------------------------------------------------------------------------*/
/*
 *                          DATA STRUCTURES
 */

/*
 * The PCB (Packet Control Block)
 * 
 * One PCB contains exactly one packet, possibly split over several areas of
 * memory.
 * 
 * The PCB structure consists of two pointers (FREE and PRIVATE) followed by a
 * variable-length array of PCB_FRAGMENT structures.  The array is terminated
 * by an entry with LENGTH=0 and FRAGMENT=NULL.
 *
 * The PCB's owner (i.e. the module that allocated it) must initialize the
 * FREE field to point to a function to free the PCB.  PRIVATE can be used by
 * the PCB's owner to store any information it wishes.
 */

struct pcb;
typedef void (*  PCB_FREE_FPTR)(struct pcb *);

/*!
 * \brief Structure that contains fragment of PCB. 
 */ 
typedef struct pcb_fragment 
{
   /*! \brief Length of PCB fragment. */
   uint32_t           LENGTH;
   /*! \brief Pointer to fragment. */
   unsigned char         *FRAGMENT;
} PCB_FRAGMENT, * PCB_FRAGMENT_PTR;

/*!
 * \brief PCB structure. 
 * 
 * The array is terminated by an entry with LENGTH=0 and FRAGMENT=NULL.
 */ 
typedef struct pcb 
{
   /*! \brief Function that frees PCB. */
   PCB_FREE_FPTR     FREE;
   /*! \brief Private PCB information. */
   void             *PRIVATE;
   /*! \brief Pointer to PCB fragment. */
   PCB_FRAGMENT      FRAG[1];
} PCB, * PCB_PTR;

// The minimum size of a PCB must contain two fragments, so the following represents
// the smallest PCB
/*!
 * \brief PCB structure contains two fragments.
 * 
 * The array is terminated by an entry with LENGTH=0 and FRAGMENT=NULL. 
 */
typedef struct pcb2
{
   /*! \brief Function that frees PCB. */
   PCB_FREE_FPTR     FREE;
   /*! \brief Private PCB information. */
   void             *PRIVATE;
   /*! \brief Pointers to two PCB fragments. */
   PCB_FRAGMENT      FRAG[2];
} PCB2, * PCB2_PTR;

#define PCB_MINIMUM_SIZE (sizeof(PCB2))

/*--------------------------------------------------------------------------*/
/*                           EXTERNAL DECLARATIONS                          */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
