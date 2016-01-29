/*
 * File:		cache.c
 * Purpose:		useful functions for the cache
 *
 */

#include "common.h"
#include "cache.h"


/* To Initialize both caches, make these two function calls
 *  cache_init(CODE_CACHE);
 *  cache_init(SYS_CACHE);
 */

uint8 cache_init(uint8 cache)
{

  if (cache == CODE_CACHE)
  {

      /* Code Cache Init */

      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM_PCCCR = LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM_PCCCR |= LMEM_PCCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM_PCCCR & LMEM_PCCCR_GO_MASK)>>LMEM_PCCCR_GO_SHIFT)== 0x1)
      {};

      /* enable write buffer */
      LMEM_PCCCR |= LMEM_PCCCR_ENWRBUF_MASK;

      /* enable cache */
      LMEM_PCCCR |= LMEM_PCCCR_ENCACHE_MASK;

  }else if (cache == SYS_CACHE)
  {

      /* System Cache Init */

      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM_PSCCR = LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM_PSCCR |= LMEM_PSCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM_PSCCR & LMEM_PSCCR_GO_MASK)>>LMEM_PSCCR_GO_SHIFT)== 0x1)
      {};

      /* enable write buffer */
      LMEM_PSCCR |= LMEM_PSCCR_ENWRBUF_MASK;

      /* enable cache */
      LMEM_PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

  } else
  {
     return CACHE_INVALID_PARAM;
  }

  return CACHE_OK;

}


uint8 cache_disable(uint8 cache)
{

  if (cache == CODE_CACHE)
  {
       /* Disable code cache */
       LMEM_PCCCR = 0;

  }else if (cache == SYS_CACHE)
  {
       /* Disable system cache */
       LMEM_PSCCR = 0;

  } else
  {
     return CACHE_INVALID_PARAM;
  }

  return CACHE_OK;

}


uint8 cache_invalidate(uint8 cache)
{

  if (cache == CODE_CACHE)
  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM_PCCCR = LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM_PCCCR |= LMEM_PCCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM_PCCCR & LMEM_PCCCR_GO_MASK)>>LMEM_PCCCR_GO_SHIFT)== 0x1)
      {};

  }else if (cache == SYS_CACHE)
  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM_PSCCR = LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM_PSCCR |= LMEM_PSCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM_PSCCR & LMEM_PSCCR_GO_MASK)>>LMEM_PSCCR_GO_SHIFT)== 0x1)
      {};

  } else
  {
     return CACHE_INVALID_PARAM;
  }

  return CACHE_OK;

}

