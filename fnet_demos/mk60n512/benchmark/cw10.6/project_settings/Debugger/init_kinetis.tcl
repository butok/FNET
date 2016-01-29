# this method initializes debug modules which are not affected by software reset 

proc init_debug_modules {} {
    # clear DWT function registers
	reg DEMCR=0x1000001
	reg DWT_FUNCTION0=0x0
	reg DWT_FUNCTION1=0x0
	reg DWT_FUNCTION2=0x0
	reg DWT_FUNCTION3=0x0
	# clear FPB comparators
	reg FP_COMP0=0x0
	reg FP_COMP1=0x0
	reg FP_COMP2=0x0
	reg FP_COMP3=0x0
	reg FP_COMP4=0x0
	reg FP_COMP5=0x0
}

proc init_trace_modules {} {
    # clear DWT registers
	reg DWT_CTRL=0x40000000
	reg DWT_CYCCNT=0x0
    reg DWT_CPICNT=0x0
    reg DWT_EXCCNT=0x0
    reg DWT_SLEEPCNT=0x0
    reg DWT_LSUCNT=0x0
    reg DWT_FOLDCNT=0x0
    reg DWT_COMP0=0x0
    reg DWT_COMP1=0x0
    reg DWT_COMP2=0x0
    reg DWT_COMP3=0x0
    reg DWT_MASK0=0x0
    reg DWT_MASK1=0x0
    reg DWT_MASK2=0x0
    reg DWT_MASK3=0x0
	# clear ITM registers
    reg ITM_LAR=0xc5acce55
	reg ITM_TER=0x0
	reg ITM_TPR=0x0
    reg ITM_TCR=0x0
    reg ITM_LAR=0x1
    # reset Funnel registers
    reg ETF_FCR=0x300
    # clear MCM registers
    reg MCM_ETBCC=0x0
    reg MCM_ETBRL=0x0
    # set SCB_VTOR register for RAM
    reg SCB_VTOR=0x20000000
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

#-------------------------------------------------------------------------------
# Main                                                                          
#-------------------------------------------------------------------------------

  envsetup
  init_debug_modules
  init_trace_modules
