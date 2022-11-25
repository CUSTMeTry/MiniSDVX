//
// Created by ijink on 2022/11/24.
//
#include "i2c.h"
#include "bsp_key.h"
#include "bsp_encoder.h"
#include "bsp_rgb.h"

extern uint8_t keyCode[9];
extern uint8_t keyCodeDefault[9];
extern uint8_t stepL;
extern uint8_t stepR;

/**
 * @brief BSP硬件总中断
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) {
        keyEXIT();
        return;
    }
    encoderEXIT(GPIO_Pin);
}

/**
 * @brief BSP数据初始化
 */
uint8_t bspDataInit(){
    if (HAL_I2C_Mem_Read(&hi2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, keyCode, 9, 1000) == HAL_OK)
    {
        for(uint8_t i = 0; i < 9; i ++){
            if(keyCode[0] == 0xFF){
                keyCode[i] = keyCodeDefault[i];
            }
        }
    }else{
        return 0x01;
    }
    if (HAL_I2C_Mem_Read(&hi2c1, 0xA0, 9, I2C_MEMADD_SIZE_8BIT, &stepL, 1, 1000) == HAL_OK)
    {
        if(stepL == 0xFF){
            stepL = 4;
        }else{
            stepL /= 10;
        }
    }else{
        return 0x02;
    }
    if (HAL_I2C_Mem_Read(&hi2c1, 0xA0, 10, I2C_MEMADD_SIZE_8BIT, &stepR, 1, 1000) == HAL_OK)
    {
        if(stepR == 0xFF){
            stepR = 10;
        }else{
            stepR /= 5;
        }
    }else{
        return 0x03;
    }
    return 0;
}

/**
 * @brief BSP硬件初始化
 */
uint8_t bspInit(){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    WS_CloseAll();
    WS_CloseAllK();
    bspDataInit();

}

