
/*! Include headers */
#include "bsp_24c02.h"
#include "main.h"

// void IIC_Start(void)
// {
//     SDA_OUT();
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET); // IIC_SDA=1;
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET); // IIC_SCL=1;
//     HAL_Delay(4);
//     /* IIC_SDA=0; START:when CLK is high,DATA change form high to low */
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
//     HAL_Delay(4);
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
// }

// void IIC_Stop(void)
// {
//     SDA_OUT();
//     /* IIC_SCL=0;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//     /* IIC_SDA=0; STOP:when CLK is high DATA change form low to high----------*/
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
//     HAL_Delay(4);
//     /* IIC_SCL=1;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//     /* IIC_SDA=1; ����I2C���߽����ź�------------------------------------------*/
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
//     HAL_Delay(4);
// }

// uint8_t IIC_Wait_Ack(void)
// {
//     uint8_t ucErrTime = 0;
//     SDA_IN();
//     /* IIC_SDA=1;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
//     HAL_Delay(1);
//     /* IIC_SCL=1;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//     HAL_Delay(1);
//     while (HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin) == GPIO_PIN_SET)
//     {
//         ucErrTime++;
//         if (ucErrTime > 250)
//         {
//             IIC_Stop();
//             return 1;
//         }
//     }
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//     return 0;
// }
// void IIC_Ack(void)
// {
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//     SDA_OUT();
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
//     HAL_Delay(2);
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//     HAL_Delay(2);
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
// }
// void IIC_NAck(void)
// {
//     /* IIC_SCL=0;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//     SDA_OUT();
//     /* IIC_SDA=1;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
//     HAL_Delay(2);
//     /* IIC_SCL=1;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//     HAL_Delay(2);
//     /* IIC_SCL=0;-------------------------------------------------------------*/
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
// }

// void IIC_Send_Byte(uint8_t txd)
// {
//     uint8_t t;
//     SDA_OUT();
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//     for (t = 0; t < 8; t++)
//     {
//         // IIC_SDA = ( txd & 0x80 ) >> 7;
//         if ((txd & 0x80) >> 7)
//         {
//             HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
//         }
//         else
//         {
//             HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
//         }
//         txd <<= 1;
//         HAL_Delay(2);
//         /* IIC_SCL=1; --------------------------------------------------------*/
//         HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//         HAL_Delay(2);
//         HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//         HAL_Delay(2);
//     }
// }
// uint8_t IIC_Read_Byte(unsigned char ack)
// {
//     unsigned char i, receive = 0;
//     SDA_IN();
//     for (i = 0; i < 8; i++)
//     {
//         /* IIC_SCL=0;---------------------------------------------------------*/
//         HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
//         HAL_Delay(2);
//         /* IIC_SCL=1;---------------------------------------------------------*/
//         HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
//         receive <<= 1;
//         if (HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin) == GPIO_PIN_SET)
//             receive++;
//         HAL_Delay(1);
//     }
//     if (!ack)
//         IIC_NAck();
//     else
//         IIC_Ack();
//     return receive;
// }

// void AT24CXX_Init(void)
// {
//     HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
//     HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
// }

// uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
// {
//     uint8_t temp = 0;
//     IIC_Start();
//     if (EE_TYPE > AT24C16)
//     {
//         IIC_Send_Byte(0XA0);
//         IIC_Wait_Ack();
//         IIC_Send_Byte(ReadAddr >> 8);
//     }
//     else
//         IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));
//     IIC_Wait_Ack();
//     IIC_Send_Byte(ReadAddr % 256);
//     IIC_Wait_Ack();
//     IIC_Start();
//     IIC_Send_Byte(0XA1);
//     IIC_Wait_Ack();
//     temp = IIC_Read_Byte(0);
//     IIC_Stop();
//     return temp;
// }

// void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
// {
//     IIC_Start();
//     if (EE_TYPE > AT24C16)
//     {
//         IIC_Send_Byte(0XA0);
//         IIC_Wait_Ack();
//         IIC_Send_Byte(WriteAddr >> 8);
//     }
//     else
//         IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));
//     IIC_Wait_Ack();
//     IIC_Send_Byte(WriteAddr % 256);
//     IIC_Wait_Ack();
//     IIC_Send_Byte(DataToWrite);
//     IIC_Wait_Ack();
//     IIC_Stop();
//     HAL_Delay(10);
// }

// void AT24CXX_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
// {
//     uint8_t t;
//     for (t = 0; t < Len; t++)
//     {
//         AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
//     }
// }

// uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
// {
//     uint8_t t;
//     uint32_t temp = 0;
//     for (t = 0; t < Len; t++)
//     {
//         temp <<= 8;
//         temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
//     }
//     return temp;
// }

// uint8_t AT24CXX_Check(void)
// {
//     uint8_t temp;
//     temp = AT24CXX_ReadOneByte(255);
//     if (temp == 0X55)
//         return 0;
//     else
//     {
//         AT24CXX_WriteOneByte(255, 0X55);
//         temp = AT24CXX_ReadOneByte(255);
//         if (temp == 0X55)
//             return 0;
//     }
//     return 1;
// }

// void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
// {
//     while (NumToRead)
//     {
//         *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
//         NumToRead--;
//     }
// }

// void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
// {
//     while (NumToWrite--)
//     {
//         AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
//         WriteAddr++;
//         pBuffer++;
//     }
// }
