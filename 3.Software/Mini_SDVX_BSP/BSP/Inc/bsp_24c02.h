#ifndef __BSP_24C02_H__
#define __BSP_24C02_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

HAL_StatusTypeDef bsp_24c02DataRead(uint8_t *buf, uint8_t size);
HAL_StatusTypeDef bsp_24c02Write(uint8_t *buf, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif