#ifndef _BUMPERS_H_
#define _BUMPERS_H_

#include <SYSTEM/include.h>

//*********************************<Bumpers>************************************
#define BUMPERS_BALL          (0x20)

#define BUMPERS_ALL           (0xDE)
#define BUMPERS_HINTEN_LINKS  (0x02)
#define BUMPERS_HINTEN_RECHTS (0x04)
#define BUMPERS_RECHTS_RECHTS (0x08)
#define BUMPERS_RECHTS_LINKS  (0x10)
#define BUMPERS_LINKS_LINKS   (0x40)
#define BUMPERS_LINKS_RECHTS  (0x80)

int8_t bumpers_get(uint8_t mask);


#endif
