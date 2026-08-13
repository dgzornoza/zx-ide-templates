
#ifndef __CORE_UTILS_ZX0_H__
#define __CORE_UTILS_ZX0_H__

#include <sys/compiler.h>

/** ZX0 Decompresses data, the intermediate version of the decompressor, providing
 *  a balance between speed and size. (126 bytes, 21% faster) */
extern unsigned char *dzx0_turbo(void *src, void *dst) __z88dk_callee;

#endif // __CORE_UTILS_ZX0_H__
