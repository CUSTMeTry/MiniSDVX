#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "spi.h"

#define K1 0x0001 << 4
#define K2 0x0001 << 8
#define K3 0x0001 << 9
#define K4 0x0001 << 10
#define K5 0x0001 << 11
#define K6 0x0001 << 15
#define K7 0x0001 << 14
#define K8 0x0001 << 13
#define K9 0x0001 << 12

uint8_t keyboardBitDecode(void);
void keyboardInit(void);

#ifdef __cplusplus
}
#endif

#endif
