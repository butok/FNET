
#ifndef __CACHE_H__
#define __CACHE_H__

uint8 cache_init(uint8 cache);
uint8 cache_disable(uint8 cache);
uint8 cache_invalidate(uint8 cache);


#define CODE_CACHE	0
#define SYS_CACHE	1
#define DATA_SEL        0
#define TAG_SEL         1
#define WAY0_SEL        0
#define WAY1_SEL        1

#define CACHE_OK               0
#define CACHE_INVALID_PARAM    1

#endif /* __CACHE_H__ */

