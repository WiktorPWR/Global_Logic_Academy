#ifndef LIS302DL_INTERFACE_H
#define LIS302DL_INTERFACE_H

#include "main.h"
#include <stdint.h>
#include "LIS302DL_object.h"

extern struct LIS302DL_Object lis302dl;

/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */

enum LIS302DL_Interrut_Block{
    LIS302DL_INT_BLOCK_1 = 0,
    LIS302DL_INT_BLOCK_2 = 1
};


#define LIS302DL_SPI_TIMEOUT 100

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;
    struct LIS302DL_Object data; /* Local shadow copy of the registers */
} LIS302DL_HandleTypeDef;


HAL_StatusTypeDef LIS302DL_Check_ID(LIS302DL_HandleTypeDef *dev);
HAL_StatusTypeDef LIS302DL_Init(LIS302DL_HandleTypeDef *dev);

HAL_StatusTypeDef LIS302DL_CtrlReg1_Config(LIS302DL_HandleTypeDef *dev, uint8_t data_rate, uint8_t power_mode, uint8_t full_scale, uint8_t stp, uint8_t stm, uint8_t zen, uint8_t yen, uint8_t xen);
HAL_StatusTypeDef LIS302DL_CtrlReg2_Config(LIS302DL_HandleTypeDef *dev, uint8_t sim, uint8_t boot, uint8_t fds, uint8_t hp_ff_wu2, uint8_t hp_ff_wu1);
HAL_StatusTypeDef LIS302DL_CtrlReg3_Config(LIS302DL_HandleTypeDef *dev, uint8_t i1_cfg, uint8_t i2_cfg, uint8_t ppod, uint8_t ihl);

HAL_StatusTypeDef LIS302DL_FF_WU_CFG(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t aoi, uint8_t lir, uint8_t zhie, uint8_t zlie, uint8_t yhie, uint8_t ylie, uint8_t xhie, uint8_t xlie);
HAL_StatusTypeDef LIS302DL_FF_WU_THS(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t dcrm, uint8_t threshold);
HAL_StatusTypeDef LIS302DL_FF_WU_DURATION(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t duration);

/* ========================================================================== */
/* 2. DATA ACQUISITION & ACCELERATION READING                                 */
/* ========================================================================== */

HAL_StatusTypeDef LIS302DL_Read_Status_Register(LIS302DL_HandleTypeDef *dev);
HAL_StatusTypeDef LIS302DL_Read_Acceleration(LIS302DL_HandleTypeDef *dev);
HAL_StatusTypeDef LIS302DL_Read_FF_WU_SRC(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block);

#endif /* LIS302DL_INTERFACE_H */