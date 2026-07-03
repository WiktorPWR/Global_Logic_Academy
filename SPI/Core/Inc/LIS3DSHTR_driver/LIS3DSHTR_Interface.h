#ifndef LIS3DSHTR_INTERFACE_H
#define LIS3DSHTR_INTERFACE_H

#include "main.h"
#include <stdint.h>
#include "LIS3DSHTR_object.h"

extern struct LIS3DSHTR_Object lis302dl;

/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */

enum LIS3DSHTR_Interrut_Block{
    LIS3DSHTR_INT_BLOCK_1 = 0,
    LIS3DSHTR_INT_BLOCK_2 = 1
};


#define LIS3DSHTR_SPI_TIMEOUT 100

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;
    struct LIS3DSHTR_Object data; /* Local shadow copy of the registers */
} LIS3DSHTR_HandleTypeDef;




#endif /* LIS3DSHTR_INTERFACE_H */