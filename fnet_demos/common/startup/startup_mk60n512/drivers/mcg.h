/*
 * File:    pll_init.h
 * Purpose: pll_driver specific declarations
 *
 * Notes:
 */
#ifndef __MCG_H__
#define __MCG_H__
/********************************************************************/

unsigned char pll_init(unsigned char, unsigned char);

void mcg_pee_2_blpi(void);
void mcg_blpi_2_pee(void);
void mcg_pbe_2_pee(void);

#if FNET_CFG_COMP_CW
    __declspec(data) 
#endif
#if FNET_CFG_COMP_IAR 
    __ramfunc
#endif
void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);

enum clk_option
{
  PLL50,
  PLL100,
  PLL96,
  PLL48
};

enum crystal_val
{
  XTAL2,
  XTAL4,
  XTAL6,
  XTAL8,
  XTAL10,
  XTAL12,
  XTAL14,
  XTAL16,
  XTAL18,
  XTAL20,
  XTAL22,
  XTAL24,
  XTAL26,
  XTAL28,
  XTAL30,
  XTAL32
};

/********************************************************************/
#endif /* __MCG_H__ */
