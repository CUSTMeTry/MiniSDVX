
/*! Include headers */
#include "bsp_24c02.h"
#include "i2c.h"

#define AT24CXX_Write_ADDR 0xA0
#define AT24CXX_Read_ADDR  0xA1
#define AT24CXX_MAX_SIZE   256
#define AT24CXX_PAGE_SIZE  8
#define AT24CXX_PAGE_TOTAL (AT24CXX_MAX_SIZE/AT24CXX_PAGE_SIZE)

HAL_StatusTypeDef bsp_24c02DataRead(uint8_t *buf, uint8_t size){
    return HAL_I2C_Mem_Read(&hi2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, buf, size, 1000);
}

HAL_StatusTypeDef bsp_24c02Write(uint8_t *buf, uint8_t size){
   
    return HAL_I2C_Mem_Write(&hi2c1, 0xA1, 0, I2C_MEMADD_SIZE_8BIT,buf,size, 1000);

}