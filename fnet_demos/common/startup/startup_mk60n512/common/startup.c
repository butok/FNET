/*
 * File:    startup.c
 * Purpose: Generic Kinetis startup code
 *
 * Notes:
 */

#include "fnet.h"


#include "common.h"

#if FNET_CFG_COMP_IAR
    #pragma section = ".data"
    #pragma section = ".data_init"
    #pragma section = ".bss"
    #pragma section = "CodeRelocate"
    #pragma section = "CodeRelocateRam"
#endif /* FNET_CFG_COMP_IAR */

#if FNET_CFG_COMP_CW
/* format of the ROM table info entry ... */
typedef struct RomInfo
{
    unsigned long	Source;
    unsigned long	Target;
    unsigned long 	Size;
} RomInfo;

/* linker defined symbol */
extern RomInfo __S_romp[];
extern char __START_BSS[];
extern char __END_BSS[];

typedef void (*StaticInitializer)(void);
extern StaticInitializer __sinit__[];
/*
 *	Routine to copy a single section from ROM to RAM ...
 */
void copy_rom_section(unsigned long dst, unsigned long src, unsigned long size)
{
    unsigned long len = size;

    const int size_int = sizeof(int);
    const int mask_int = sizeof(int) - 1;

    const int size_short = sizeof(short);
    const int mask_short = sizeof(short) - 1;

    const int size_char = sizeof(char);

    if( dst == src || size == 0)
    {
        return;
    }


    while( len > 0)
    {

        if( !(src & mask_int) && !(dst & mask_int) && len >= size_int)
        {
            *((int *)dst)  = *((int *)src);
            dst += size_int;
            src += size_int;
            len -= size_int;
        }
        else if( !(src & mask_short) && !(dst & mask_short) && len >= size_short)
        {
            *((short *)dst)  = *((short *)src);
            dst += size_short;
            src += size_short;
            len -= size_short;
        }
        else
        {
            *((char *)dst)  = *((char *)src);
            dst += size_char;
            src += size_char;
            len -= size_char;
        }
    }
}
#endif /* FNET_CFG_COMP_CW */

/********************************************************************/
void
common_startup(void)
{
    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;

    /* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
#if defined(__CC_ARM)
    extern uint32_t Image$$VECTOR_ROM$$Base[];
    extern uint32_t Image$$VECTOR_RAM$$Base[];
    extern uint32_t Image$$RW_m_data$$Base[];

#define __VECTOR_TABLE Image$$VECTOR_ROM$$Base
#define __VECTOR_RAM Image$$VECTOR_RAM$$Base
#define __RAM_VECTOR_TABLE_SIZE (((uint32_t)Image$$RW_m_data$$Base - (uint32_t)Image$$VECTOR_RAM$$Base))
#elif defined(__ICCARM__)
    extern uint32_t __RAM_VECTOR_TABLE_SIZE[];
    extern uint32_t __VECTOR_TABLE[];
    extern uint32_t __VECTOR_RAM[];
#elif defined(__GNUC__)
    extern uint32_t __VECTOR_TABLE[];
    extern uint32_t __VECTOR_RAM[];
    extern uint32_t __RAM_VECTOR_TABLE_SIZE_BYTES[];
    uint32_t __RAM_VECTOR_TABLE_SIZE = (uint32_t)(__RAM_VECTOR_TABLE_SIZE_BYTES);
#endif

    extern uint32 __vector_table[];

    /* Copy the vector table to RAM */
    if ((uint32 *)__VECTOR_RAM != __vector_table)
    {
        uint32 *vector_ram = (uint32 *)__VECTOR_RAM;

        for (n = 0; n < (0x410u / sizeof(uint32_t)); n++)
        {
            *vector_ram++ = __vector_table[n];
        }
    }

    /* Point the VTOR to the new copy of the vector table */
    write_vtor((uint32)FNET_CFG_CPU_VECTOR_TABLE);

#if FNET_CFG_COMP_CW
    {
        RomInfo *ptr_tmp = __S_romp;
        StaticInitializer s, *p;

        /* Zero bss section*/
        fnet_memset(__START_BSS, 0, (__END_BSS - __START_BSS));

        /* Copying sections from ROM to RAM.*/
        if((uint32)ptr_tmp)
        {
            int	index;

            /*
             *	Go through the entire table, copying sections from ROM to RAM.
             */
            for (index = 0;
                 __S_romp[index].Source != 0 ||
                 __S_romp[index].Target != 0 ||
                 __S_romp[index].Size != 0;
                 ++index)
            {
                copy_rom_section( __S_romp[index].Target,
                                  __S_romp[index].Source,
                                  __S_romp[index].Size );
            }
        }

        /*	See if the static initializer table exists	*/
        if (__sinit__)
        {
            /*	call all static initializers in the table	*/
            for (p = __sinit__; p && (s = *p) != 0; p++)
                s();
        }
    }
#endif /* FNET_CFG_COMP_CW */

#if FNET_CFG_COMP_IAR
    {
        /* Get the addresses for the .data section (initialized data section) */
        uint8 *data_ram = __section_begin(".data");
        uint8 *data_rom = __section_begin(".data_init");
        uint8 *data_rom_end = __section_end(".data_init");

        /* Copy initialized data from ROM to RAM */
        n = data_rom_end - data_rom;
        while (n--)
            *data_ram++ = *data_rom++;


        /* Get the addresses for the .bss section (zero-initialized data) */
        uint8 *bss_start = __section_begin(".bss");
        uint8 *bss_end = __section_end(".bss");

        /* Clear the zero-initialized data section */
        n = bss_end - bss_start;
        while(n--)
            *bss_start++ = 0;

        /* Get addresses for any code sections that need to be copied from ROM to RAM.
         * The IAR tools have a predefined keyword that can be used to mark individual
         * functions for execution from RAM. Add "__ramfunc" before the return type in
         * the function prototype for any routines you need to execute from RAM instead
         * of ROM. ex: __ramfunc void foo(void);
         */
        uint8 *code_relocate_ram = __section_begin("CodeRelocateRam");
        uint8 *code_relocate = __section_begin("CodeRelocate");
        uint8 *code_relocate_end = __section_end("CodeRelocate");

        /* Copy functions from ROM to RAM */
        n = code_relocate_end - code_relocate;
        while (n--)
            *code_relocate_ram++ = *code_relocate++;
    }
#endif /*FNET_CFG_COMP_IAR*/
}
/********************************************************************/
