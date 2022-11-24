#include "bsp_key.h"
#include "spi.h"

uint16_t lastData;
uint16_t keyData;

uint8_t keyCode[9] = {0};
uint8_t keyCodeDefault[9] = {4, 5, 6, 7, 8, 9, 10, 11, 12};
uint16_t keyFlag[8] = {0x0001 << 8, 0x0001 << 9, 0x0001 << 10, 0x0001 << 11, 0x0001 << 15, 0x0001 << 14, 0x0001 << 13, 0x0001 << 12};
uint16_t functionKeyFlag[4] = {0x0001 << 0, 0x0001 << 1, 0x0001 << 2, 0x0001 << 3};

uint8_t keyboardReport[11] = {0};
uint8_t isKeyPressed[9] = {0};
uint8_t isFunctionKeyPressed[4] = {0};

uint8_t changeFlag;

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
                isKeyPressed[i + 1] = 0;
                keyboardReport[i + 3] = 0;
            }
            else
            {
                isKeyPressed[i + 1] = 1;
                keyboardReport[i + 3] = keyCode[i];
            }
        }

        for(i = 0; i < 4; i++)
        {
            if(functionKeyFlag[i] & keyData)
            {
                isFunctionKeyPressed[i] = 0;
            }
            else
            {
                isFunctionKeyPressed[i] = 1;
            }
        }
        lastData = keyData;
        changeFlag = 1;
    }
    return changeFlag;
}

void keyEXIT(){

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
        isKeyPressed[0] = 0;
        keyboardReport[2] = 0;
    }
    else
    {
        isKeyPressed[0] = 1;
        keyboardReport[2] = keyCode[8];
    }

}
