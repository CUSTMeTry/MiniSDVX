#include "key.h"
#include "keycode.h"
uint16_t lastData;
uint16_t keyData;

uint8_t keyCode[9] = {Keyboard_a, Keyboard_b, Keyboard_c, Keyboard_d, Keyboard_e, Keyboard_f, Keyboard_g, Keyboard_h, Keyboard_i};
uint16_t keyFlag[8] = {0x0001 << 8, 0x0001 << 9, 0x0001 << 10, 0x0001 << 11, 0x0001 << 15, 0x0001 << 14, 0x0001 << 13, 0x0001 << 12};
uint8_t keyboardReport[13] = {0};
uint8_t keyboardReportNull[13] = {0};
uint8_t changeFlag;
/**
 * @brief 初始化键盘
 */
void keyboardInit(void)
{
    keyboardReport[0] = 1;
    keyboardReportNull[0] = 1;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

/**
 * @brief 更新一次键盘数据
 */
void keyboardGetData(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_SPI_Receive(&hspi3, (uint8_t *)&keyData, 2, 10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    if(keyData == lastData){
        changeFlag = 0;
    }else{
        changeFlag = 1;
    }
    lastData = keyData;
}

/**
 * @brief 键盘位解码
 *  |k6|k7|k8|k9| |k5|k4|k3|k2| |1|1|1|1| |s6|s5|s4|s3|
 */
uint8_t keyboardBitDecode(void)
{
    uint8_t i;
    keyboardGetData();
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
        keyboardReport[3] = 0;
    }
    else
    {
        keyboardReport[3] = keyCode[8];
        
    }
    
    for (i = 0; i < 8; i++)
    {
        if(keyFlag[i] & keyData){
            keyboardReport[i + 4] = 0;
        }else
        {
            keyboardReport[i + 4] = keyCode[i];
        }
        
    }
    return changeFlag;
}
