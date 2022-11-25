#ifndef BSP_KEY_H__
#define BSP_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void keyboardGetData(void);
uint8_t keyboardBitDecode(void);
void keyEXIT();

#ifdef __cplusplus
extern }
#endif

#endif