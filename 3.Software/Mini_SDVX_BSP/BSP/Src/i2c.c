/*
 * @Author: Snitro
 * @Date: 2021-02-22 20:57:11
 * @LastEditors: Snitro
 * @LastEditTime: 2021-02-23 10:56:26
 * @Description: 软件I2c
 */

#include "i2c.h"

/**
 * @description: 以阻塞的方式延迟微妙
 * @param {uint32_t} t 延迟时间
 * @return {*}
 */
__weak void Delay_us(uint32_t t) {
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(t - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

/**
 * @description: I2C起始信号
 * @param {*}
 * @return {*}
 */
void Software_I2C_Start(void) {
    I2C_SDA_WritePin(1);
    I2C_SCL_WritePin(1);
    Delay_us(5);
    I2C_SDA_WritePin(0);
    Delay_us(2);
}

/**
 * @description: I2C停止信号
 * @param {*}
 * @return {*}
 */
void Software_I2C_Stop(void) {
    I2C_SCL_WritePin(1);
    I2C_SDA_WritePin(0);
    Delay_us(5);
    I2C_SDA_WritePin(1);
    Delay_us(2);
}

/**
 * @description:  等待从机应答
 * @param {*}
 * @return {HAL_OK} 应答
 * @return {HAL_ERROR} 否应答
 */
HAL_StatusTypeDef Software_I2C_WaitACK(void) {
    uint8_t ErrorTime = 0xFF;
    I2C_SDA_WritePin(1);
    I2C_SCL_WritePin(1);
    while (I2C_SDA_ReadPin() && (--ErrorTime))
        ;
    if (!ErrorTime) return HAL_ERROR;

    Delay_us(4);
    I2C_SCL_WritePin(0);
    Delay_us(5);
    return HAL_OK;
}

void Software_I2C_WriteByte(uint8_t byte) {
    I2C_SCL_WritePin(0);
    for (uint8_t i = 0; i < 8; i++) {
        I2C_SDA_WritePin(byte & 0x80);  //高位先写
        byte <<= 1;
        Delay_us(4);
        I2C_SCL_WritePin(1);
        Delay_us(5);
        I2C_SCL_WritePin(0);
        Delay_us(5);
    }
}