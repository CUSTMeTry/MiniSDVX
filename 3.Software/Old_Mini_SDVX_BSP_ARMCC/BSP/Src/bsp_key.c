#include "bsp_key.h"
#include "spi.h"
#include "i2c.h"
#include "bsp_rgb.h"

uint16_t lastData;
uint16_t keyData;

uint8_t LCount;
uint8_t RCount;

uint8_t step = 2;

uint8_t keyCode[9] = {4, 5, 6, 7, 8, 9, 10, 11, 12};
uint16_t keyFlag[8] = {0x0001 << 8, 0x0001 << 9, 0x0001 << 10, 0x0001 << 11, 0x0001 << 15, 0x0001 << 14, 0x0001 << 13, 0x0001 << 12};

uint8_t keyboardReport[11] = {0};
uint8_t keyIsPress[9] = {0};

uint8_t changeFlag;

uint8_t _keyCodeinit()
{
    if (HAL_I2C_Mem_Read(&hi2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, keyCode, 9, 1000) == HAL_OK)
    {
        return 0;
    }
    // if (HAL_I2C_Mem_Read(&hi2c1, 0xA0, 9, I2C_MEMADD_SIZE_8BIT, step, 1, 1000) == HAL_OK)
    // {
    //     return 0;
    // }
    return 0x01;
}

/**
 * @brief 初始化键盘
 */
void keyboardInit(void)
{
    _keyCodeinit();
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
}
/**
-- * @brief 键盘位解码
 *  |k6|k7|k8|k9| |k5|k4|k3|k2| |1|1|1|1| |s6|s5|s4|s3|
 */
uint8_t keyboardBitDecode()
{
    if (keyData == lastData)
    {
        changeFlag = 0;
    }
    else
    {
        uint8_t i;
        keyboardGetData();

        for (i = 0; i < 8; i++)
        {
            if (keyFlag[i] & keyData)
            {
                keyIsPress[i + 1] = 0;
                keyboardReport[i + 3] = 0;
            }
            else
            {
                keyIsPress[i + 1] = 1;
                keyboardReport[i + 3] = keyCode[i];
            }
        }
        lastData = keyData;
        changeFlag = 1;
    }
    return changeFlag;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
        {
            keyIsPress[0] = 0;
            keyboardReport[2] = 0;
        }
        else
        {
            keyIsPress[0] = 1;
            keyboardReport[2] = keyCode[8];
        }
        return;
    }

    if (GPIO_Pin == ENC_LA_Pin)
    {
        if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
        {
            if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
                LCount--;
            else
                LCount++;
        }
        else
        {
            if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
                LCount++;
            else
                LCount--;
        }
    }
    else if (GPIO_Pin == ENC_LB_Pin)
    {
        if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
        {
            if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
                LCount++;
            else
                LCount--;
        }
        else
        {
            if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
                LCount--;
            else
                LCount++;
        }
    }
    else if (GPIO_Pin == ENC_RA_Pin)
    {
        if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
        {
            if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
                RCount--;
            else
                RCount++;
        }
        else
        {
            if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
                RCount++;
            else
                RCount--;
        }
    }
    else if (GPIO_Pin == ENC_RB_Pin)
    {
        if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
        {
            if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
                RCount++;
            else
                RCount--;
        }
        else
        {
            if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
                RCount--;
            else
                RCount++;
        }
    }
}