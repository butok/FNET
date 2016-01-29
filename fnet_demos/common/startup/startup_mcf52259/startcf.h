/******************************************************************************
  FILE    : startcf.h
  PURPOSE : startup code for ColdFire
  LANGUAGE: C


  Notes:
        1) Default entry point is _startup. 
           . disable interrupts
           . the SP is set to __SP_AFTER_RESET
           . SP must be initialized to valid memory 
             in case the memory it points to is not valid using MEMORY_INIT macro
        2) __initialize_hardware is called. Here you can initialize memory and some peripherics
           at this point global variables are not initialized yet
        3) After __initialize_hardware memory is setup; initialize SP to _SP_INIT and perform 
           needed initialisations for the language (clear memory, data rom copy).
        4) Jump to main 

*/
/********************************************************************************/

#ifndef STARTCF_H
#define STARTCF_H


#include "support_common.h"

extern unsigned long __SP_INIT[];
extern unsigned long __SP_AFTER_RESET[];


#ifndef MEMORY_INIT
/* If MEMORY_INIT is set then it performs
   minimal memory initialization (to preset SP to __SP_AFTER_RESET, etc...)
*/
#define MEMORY_INIT
#endif
							 

void _startup(void);

#ifndef SUPPORT_ROM_TO_RAM
  /*
   * If SUPPORT_ROM_TO_RAM is set, _S_romp is used to define the copy to be performed.
   * If it is not set, there's a single block to copy, performed directly without 
   * using the __S_romp structure, based on __DATA_RAM, __DATA_ROM and
   * __DATA_END symbols.
   *
   * Set to 0 for more aggressive dead stripping ...
   */
#define SUPPORT_ROM_TO_RAM 1
#endif

/* format of the ROM table info entry ... */
typedef struct RomInfo {
	void            *Source;
	void            *Target;
	unsigned long    Size;
} RomInfo;

/* imported data */
extern RomInfo _S_romp[];		/* linker defined symbol */


void __copy_rom_sections_to_ram(void);
void clear_mem(char *dst, unsigned long n);
void __copy_rom_section(char* dst, const char* src, unsigned long size);


#endif
