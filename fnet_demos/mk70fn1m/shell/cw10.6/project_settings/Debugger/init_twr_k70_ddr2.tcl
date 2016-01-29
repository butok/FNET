# this method initializes debug modules which are not affected by software reset 
# register names should be referenced including the register group name to improve performance 

proc init_debug_modules {} {
    # clear DWT function registers
	reg "Core Debug Registers/DEMCR" = 0x1000001
	reg "Data Watchpoint and Trace Unit Registers/DWT_FUNCTION0" = 0x0
	reg "Data Watchpoint and Trace Unit Registers/DWT_FUNCTION1" = 0x0
	reg "Data Watchpoint and Trace Unit Registers/DWT_FUNCTION2" = 0x0
	reg "Data Watchpoint and Trace Unit Registers/DWT_FUNCTION3" = 0x0
	# clear FPB comparators
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP0" = 0x0
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP1" = 0x0
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP2" = 0x0
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP3" = 0x0
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP4" = 0x0
	reg "Flash Patch and Breakpoint Unit Registers/FP_COMP5" = 0x0
}

proc init_trace_modules {} {
    # clear DWT registers
	reg "Data Watchpoint and Trace Unit Registers/DWT_CTRL" =0x40000000
	reg "Data Watchpoint and Trace Unit Registers/DWT_CYCCNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_CPICNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_EXCCNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_SLEEPCNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_LSUCNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_FOLDCNT" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_COMP0" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_COMP1" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_COMP2" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_COMP3" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_MASK0" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_MASK1" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_MASK2" =0x0
    reg "Data Watchpoint and Trace Unit Registers/DWT_MASK3" =0x0
	# clear ITM registers
    reg "Instrumentation Trace Macrocell Registers/ITM_LAR" =0xc5acce55
	reg "Instrumentation Trace Macrocell Registers/ITM_TER" =0x0
	reg "Instrumentation Trace Macrocell Registers/ITM_TPR" =0x0
    reg "Instrumentation Trace Macrocell Registers/ITM_TCR" =0x0
    reg "Instrumentation Trace Macrocell Registers/ITM_LAR" =0x1
    # reset Funnel registers
    reg "Embedded Trace Funnel Registers/ETF_FCR" =0x300
    # clear MCM registers
    reg "Core Platform Miscellaneous Control Module (MCM) Registers/MCM_ETBCC" =0x0
    reg "Core Platform Miscellaneous Control Module (MCM) Registers/MCM_ETBRL" =0x0
    # set SCB_VTOR register for RAM
    reg "System Control Registers/SCB_VTOR" =0x20000000
}

proc envsetup {} {
	# Environment Setup
	radix x 
	config hexprefix 0x
	config MemIdentifier p 
	config MemWidth 32 
	config MemAccess 32 
	config MemSwap off
}

proc init_stack_pointer {} {
	reg SP = 0x2000FFF8	
}

proc disable_wdt {} {
	# First unlock the watchdog so that we can write to registers */
    # Write 0xC520 to the unlock register WDOG_UNLOCK*/
    reg "Generation 2008 Watchdog Timer (WDOG)/WDOG_UNLOCK" =0xC520
 	
  	# Followed by 0xD928 to complete the unlock */
    reg "Generation 2008 Watchdog Timer (WDOG)/WDOG_UNLOCK" =0xD928
	
	# Clear the WDOGEN bit to disable the watchdog */
    reg "Generation 2008 Watchdog Timer (WDOG)/WDOG_STCTRLH" =0x01D2
}

proc init_pll {} {
	# Initialize SIM dividers
	reg "System Integration Module (SIM)/SIM_SCGC5" = 0x00047F82
	reg "System Integration Module (SIM)/SIM_CLKDIV1" = 0x01250000

	# Initialize PLL1
	reg "Multipurpose Clock Generator module (MCG)/MCG_C2" = 0x10
	reg "Multipurpose Clock Generator module (MCG)/MCG_C1" = 0xA8
	reg "Multipurpose Clock Generator module (MCG)/MCG_C5" = 0x04
	reg "Multipurpose Clock Generator module (MCG)/MCG_C6" = 0x68
	reg "Multipurpose Clock Generator module (MCG)/MCG_C5" = 0x44
	reg "Multipurpose Clock Generator module (MCG)/MCG_C1" = 0x28
	
	# Initialize PLL1
	reg "Multipurpose Clock Generator module (MCG)/MCG_C10" = 0x14
	reg "Multipurpose Clock Generator module (MCG)/MCG_C12" = 0x0E
	reg "Multipurpose Clock Generator module (MCG)/MCG_C11" = 0x44
	
	# Delay to allow the PLL time to lock 
	wait (100)
}

proc init_ddr {} {
      
    #  Enable DDR controller clock 
	reg "System Integration Module (SIM)/SIM_SCGC3" = 0x00004000

	# Enable DDR pads and set slew rate 
	reg "System Integration Module (SIM)/SIM_MCR" = 0x1C4

	wait (10)
	
	reg "System Integration Module (SIM)/SIM_MCR" = 0x0C4	
		
	# I/O Pad Control (PAD_CTRL) register.*/
	mem 0x400Ae1ac = 0x01030203

	# Initialize the DDR controller
	reg "DRAM Controller (DDR)/DDR_CR00" = 0x00000400
	reg "DRAM Controller (DDR)/DDR_CR01" = 0x01000000
	reg "DRAM Controller (DDR)/DDR_CR02" = 0x02000031
	reg "DRAM Controller (DDR)/DDR_CR03" = 0x02020506
	reg "DRAM Controller (DDR)/DDR_CR04" = 0x06090202
	reg "DRAM Controller (DDR)/DDR_CR05" = 0x02020302
	reg "DRAM Controller (DDR)/DDR_CR06" = 0x02904002
	reg "DRAM Controller (DDR)/DDR_CR07" = 0x01000303
	reg "DRAM Controller (DDR)/DDR_CR08" = 0x05030201
	reg "DRAM Controller (DDR)/DDR_CR09" = 0x020000c8
	reg "DRAM Controller (DDR)/DDR_CR10" = 0x03003207
	reg "DRAM Controller (DDR)/DDR_CR11" = 0x01000000
	reg "DRAM Controller (DDR)/DDR_CR12" = 0x04920031
	reg "DRAM Controller (DDR)/DDR_CR13" = 0x00000005
	reg "DRAM Controller (DDR)/DDR_CR14" = 0x00C80002
	reg "DRAM Controller (DDR)/DDR_CR15" = 0x00000032
	reg "DRAM Controller (DDR)/DDR_CR16" = 0x00000001
	reg "DRAM Controller (DDR)/DDR_CR20" = 0x00030300
	reg "DRAM Controller (DDR)/DDR_CR21" = 0x00040232
	reg "DRAM Controller (DDR)/DDR_CR22" = 0x00000000
	reg "DRAM Controller (DDR)/DDR_CR23" = 0x00040302
	reg "DRAM Controller (DDR)/DDR_CR25" = 0x0A010201
	reg "DRAM Controller (DDR)/DDR_CR26" = 0x0101FFFF
	reg "DRAM Controller (DDR)/DDR_CR27" = 0x01010101
	reg "DRAM Controller (DDR)/DDR_CR28" = 0x00000003
	reg "DRAM Controller (DDR)/DDR_CR29" = 0x00000000
	reg "DRAM Controller (DDR)/DDR_CR30" = 0x00000001
	reg "DRAM Controller (DDR)/DDR_CR34" = 0x02020101
	reg "DRAM Controller (DDR)/DDR_CR36" = 0x01010201
	reg "DRAM Controller (DDR)/DDR_CR37" = 0x00000200
	reg "DRAM Controller (DDR)/DDR_CR38" = 0x00200000
	reg "DRAM Controller (DDR)/DDR_CR39" = 0x01010020
	reg "DRAM Controller (DDR)/DDR_CR40" = 0x00002000
	reg "DRAM Controller (DDR)/DDR_CR41" = 0x01010020
	reg "DRAM Controller (DDR)/DDR_CR42" = 0x00002000
	reg "DRAM Controller (DDR)/DDR_CR43" = 0x01010020
	reg "DRAM Controller (DDR)/DDR_CR44" = 0x00000000
	reg "DRAM Controller (DDR)/DDR_CR45" = 0x03030303
	reg "DRAM Controller (DDR)/DDR_CR46" = 0x02006401
	reg "DRAM Controller (DDR)/DDR_CR47" = 0x01020202
	reg "DRAM Controller (DDR)/DDR_CR48" = 0x01010064
	reg "DRAM Controller (DDR)/DDR_CR49" = 0x00020101
	reg "DRAM Controller (DDR)/DDR_CR50" = 0x00000064
	reg "DRAM Controller (DDR)/DDR_CR52" = 0x02000602
	reg "DRAM Controller (DDR)/DDR_CR53" = 0x03c80000
	reg "DRAM Controller (DDR)/DDR_CR54" = 0x03c803c8
	reg "DRAM Controller (DDR)/DDR_CR55" = 0x03c803c8
	reg "DRAM Controller (DDR)/DDR_CR56" = 0x020303c8
	reg "DRAM Controller (DDR)/DDR_CR57" = 0x01010002

	# Set the START bit
	reg "DRAM Controller (DDR)/DDR_CR00" = 0x00000401

	# Set the SDRAM size in the MCM
	reg "Core Platform Miscellaneous Control Module (MCM) Registers/MCM_CR" = 0x00100000
}
#-------------------------------------------------------------------------------
# Main                                                                          
#-------------------------------------------------------------------------------

  envsetup
  init_debug_modules
  init_trace_modules
  init_stack_pointer
  disable_wdt
  init_pll
  init_ddr
