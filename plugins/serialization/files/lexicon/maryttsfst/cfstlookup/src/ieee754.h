#ifndef _STUFFD_IEEE754_H
#define _STUFFD_IEEE754_H

#include "stuffd/common.h"
#include <stdint.h>

S_BEGIN_C_DECLS

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

uint64_t pack754(long double f, unsigned bits, unsigned expbits);

long double unpack754(uint64_t i, unsigned bits, unsigned expbits);

S_END_C_DECLS

#endif /* _STUFFD_IEEE754_H */
