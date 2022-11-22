#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

uint8_t _keyCodeinit();


void keyboardInit(void);
void keyboardGetData(void);
uint8_t keyboardBitDecode(void);


#ifdef __cplusplus
extern }
#endif

#endif