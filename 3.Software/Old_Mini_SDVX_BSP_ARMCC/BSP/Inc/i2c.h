/*
 * @Author: Snitro
 * @Date: 2021-02-22 20:57:06
 * @LastEditors: Snitro
 * @LastEditTime: 2021-02-23 10:56:40
 * @Description: 软件I2C
 */

#ifndef __SOFTWARE_I2C_H
#define __SOFTWARE_I2C_H

//修改为所用型号
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define SOFTWARE_I2C_GPIO_PORT GPIOB
#define SOFTWARE_I2C_SCL_PIN GPIO_PIN_6  // SCL引脚
#define SOFTWARE_I2C_SDA_PIN GPIO_PIN_7  // SDA引脚

#define I2C_SCL_WritePin(_x)                                        \
    HAL_GPIO_WritePin(SOFTWARE_I2C_GPIO_PORT, SOFTWARE_I2C_SCL_PIN, \
                      ((_x) ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define I2C_SDA_WritePin(_x)                                        \
    HAL_GPIO_WritePin(SOFTWARE_I2C_GPIO_PORT, SOFTWARE_I2C_SDA_PIN, \
                      ((_x) ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define I2C_SDA_ReadPin() \
    HAL_GPIO_ReadPin(SOFTWARE_I2C_GPIO_PORT, SOFTWARE_I2C_SDA_PIN)

void Software_I2C_Start(void);
void Software_I2C_Stop(void);
HAL_StatusTypeDef Software_I2C_WaitACK(void);
//void Software_I2C_ACK(void);
//void Software_I2C_NACK(void);
void Software_I2C_WriteByte(uint8_t WriteData);
//uint8_t Software_I2C_ReadByte(void);

#endif