#ifndef LIS3DSHTR_INTERFACE_H
#define LIS3DSHTR_INTERFACE_H

#include "main.h"
#include <stdint.h>
#include "LIS3DSHTR_object.h"

extern struct LIS3DSHTR_Object lis302dl;

/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */


#define LIS3DSHTR_SPI_TIMEOUT 100

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;
    LIS3DSHTR_Object data; /* Local shadow copy of the registers */
} LIS3DSHTR_HandleTypeDef;


typedef enum{
    LIS3DSH_FULL_SCALE_2G,
    LIS3DSH_FULL_SCALE_4G,
    LIS3DSH_FULL_SCALE_6G,
    LIS3DSH_FULL_SCALE_8G,
    LIS3DSH_FULL_SCALE_16G
}LIS3DHS_FullScaleMode;

typedef enum {
    LIS3DSH_POWER_DOWN = 0x00,
    LIS3DSH_DATA_RATE_3_125_HZ = 0x01,
    LIS3DSH_DATA_RATE_6_25_HZ = 0x02,
    LIS3DSH_DATA_RATE_12_5_HZ = 0x03,
    LIS3DSH_DATA_RATE_25_HZ = 0x04,
    LIS3DSH_DATA_RATE_50_HZ = 0x05,
    LIS3DSH_DATA_RATE_100_HZ = 0x06,
    LIS3DSH_DATA_RATE_400_HZ = 0x07,
    LIS3DSH_DATA_RATE_800_HZ = 0x08,
    LIS3DSH_DATA_RATE_1600_HZ = 0x09
}LIS3DSH_DataRate;

typedef enum{
    LIS3DSHTR_INT_1,
    LIS3DSHTR_INT_2
}LIS3DSHTR_InterruptPin;

typedef struct{
    LIS3DSHTR_InterruptPin pin;
    uint8_t polarity;
    uint8_t latching;

}LIS3DSH_InterruptConfig_t;


HAL_StatusTypeDef LIS3DSHTR_SPI_WriteReg(LIS3DSHTR_HandleTypeDef *dev, uint8_t reg_addr, uint8_t *data, uint16_t length);
HAL_StatusTypeDef LIS3DSHTR_SPI_ReadRegs(LIS3DSHTR_HandleTypeDef *dev, uint8_t reg_addr, uint16_t length);
HAL_StatusTypeDef LIS3DSH_SetDataRate_And_PowerMode(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_DataRate data_rate );
HAL_StatusTypeDef LIS3DSH_SetFullScale(LIS3DSHTR_HandleTypeDef *dev, LIS3DHS_FullScaleMode full_scale );
HAL_StatusTypeDef LIS3DSH_EnableInterrupt(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_InterruptConfig_t *config);
HAL_StatusTypeDef LIS3DSH_Init(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_DataRate data_rate);
HAL_StatusTypeDef LIS3DSH_Read_Status_Register(LIS3DSHTR_HandleTypeDef *dev);
HAL_StatusTypeDef LIS3DSH_Read_Acceleration(LIS3DSHTR_HandleTypeDef *dev);


#endif /* LIS3DSHTR_INTERFACE_H */