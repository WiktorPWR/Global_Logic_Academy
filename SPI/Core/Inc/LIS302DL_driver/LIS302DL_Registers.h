/**
 * ******************************************************************************
 * @file    lis302dl_regs.h
 * @brief   Register map and bit definitions for the LIS302DL accelerometer.
 * Based on STMicroelectronics technical datasheet (Rev 1).
 * ******************************************************************************
 */

#ifndef LIS302DL_REGS_H
#define LIS302DL_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* REGISTER MAP                                                               */
/* ========================================================================== */
#define LIS302DL_REG_WHO_AM_I           0x0F /**< Device identification register [Type: R]  [Default: 0x3B] */
#define LIS302DL_REG_CTRL_REG1          0x20 /**< Control register 1             [Type: RW] [Default: 0x07] */
#define LIS302DL_REG_CTRL_REG2          0x21 /**< Control register 2             [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_CTRL_REG3          0x22 /**< Control register 3 (INT pad)   [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_HP_FILTER_RESET    0x23 /**< High-pass filter reset         [Type: R]  [Dummy register] */
#define LIS302DL_REG_STATUS_REG         0x27 /**< Data status register           [Type: R]  [Default: 0x00] */
#define LIS302DL_REG_OUT_X              0x29 /**< X-axis output data             [Type: R]  */
#define LIS302DL_REG_OUT_Y              0x2B /**< Y-axis output data             [Type: R]  */
#define LIS302DL_REG_OUT_Z              0x2D /**< Z-axis output data             [Type: R]  */

/* Interrupt Generator 1 (Free-Fall / Wake-Up 1) */
#define LIS302DL_REG_FF_WU_CFG_1        0x30 /**< Generator 1 configuration      [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_FF_WU_SRC_1        0x31 /**< Generator 1 source/status      [Type: R]  [Default: 0x00] */
#define LIS302DL_REG_FF_WU_THS_1        0x32 /**< Generator 1 threshold          [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_FF_WU_DURATION_1   0x33 /**< Generator 1 duration           [Type: RW] [Default: 0x00] */

/* Interrupt Generator 2 (Free-Fall / Wake-Up 2) */
#define LIS302DL_REG_FF_WU_CFG_2        0x34 /**< Generator 2 configuration      [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_FF_WU_SRC_2        0x35 /**< Generator 2 source/status      [Type: R]  [Default: 0x00] */
#define LIS302DL_REG_FF_WU_THS_2        0x36 /**< Generator 2 threshold          [Type: RW] [Default: 0x00] */
#define LIS302DL_REG_FF_WU_DURATION_2   0x37 /**< Generator 2 duration           [Type: RW] [Default: 0x00] */


/* ========================================================================== */
/* CONSTANTS AND DEFAULT VALUES                                               */
/* ========================================================================== */
#define LIS302DL_WHO_AM_I_VAL           0x3B /**< Expected WHO_AM_I register content */

/* Device I2C Slave Addresses (dependent on SDO pin connection) */
#define LIS302DL_I2C_ADDR_GND           0x1C /**< I2C address when SDO is connected to GND (0011100b) */
#define LIS302DL_I2C_ADDR_VCC           0x1D /**< I2C address when SDO is connected to VDD (0011101b) */


/* ========================================================================== */
/* REGISTER BIT MASKS                                                         */
/* ========================================================================== */

/**
 * @brief CTRL_REG1 (20h) - Configuration bits
 */
#define LIS302DL_CR1_DR_MASK            (1 << 7) /**< Data Rate selection mask */
#define LIS302DL_CR1_DR_100HZ           (0 << 7) /**< 100 Hz output data rate */
#define LIS302DL_CR1_DR_400HZ           (1 << 7) /**< 400 Hz output data rate */

#define LIS302DL_CR1_PD_MASK            (1 << 6) /**< Power Down Control mask */
#define LIS302DL_CR1_PD_POWER_DOWN      (0 << 6) /**< Power down mode */
#define LIS302DL_CR1_PD_ACTIVE          (1 << 6) /**< Active mode */

#define LIS302DL_CR1_FS_MASK            (1 << 5) /**< Full Scale selection mask */
#define LIS302DL_CR1_FS_2G              (0 << 5) /**< +/-2g measurement range */
#define LIS302DL_CR1_FS_8G              (1 << 5) /**< +/-8g measurement range */

#define LIS302DL_CR1_STP                (1 << 4) /**< Self Test P enable */
#define LIS302DL_CR1_STM                (1 << 3) /**< Self Test M enable */
#define LIS302DL_CR1_ZEN                (1 << 2) /**< Z-axis enable */
#define LIS302DL_CR1_YEN                (1 << 1) /**< Y-axis enable */
#define LIS302DL_CR1_XEN                (1 << 0) /**< X-axis enable */

/**
 * @brief CTRL_REG2 (21h) - Configuration bits
 */
#define LIS302DL_CR2_SIM_MASK           (1 << 7) /**< SPI Serial Interface Mode: 0 = 4-wire, 1 = 3-wire */
#define LIS302DL_CR2_BOOT               (1 << 6) /**< Reboot memory content */
#define LIS302DL_CR2_FDS                (1 << 4) /**< Filtered Data Selection: 0 = internal filter bypassed, 1 = filter to output */
#define LIS302DL_CR2_HP_FF_WU2          (1 << 3) /**< High Pass filter enabled for Free-Fall/Wake-Up #2 */
#define LIS302DL_CR2_HP_FF_WU1          (1 << 2) /**< High Pass filter enabled for Free-Fall/Wake-Up #1 */

/* High-pass filter cut-off frequency configuration (HPcoeff2, HPcoeff1 bits) */
#define LIS302DL_CR2_HP_COEFF_MASK      (0x03 << 0)
#define LIS302DL_CR2_HP_COEFF_00        (0x00 << 0) /**< ft = 2Hz @100Hz ODR / 8Hz @400Hz ODR */
#define LIS302DL_CR2_HP_COEFF_01        (0x01 << 0) /**< ft = 1Hz @100Hz ODR / 4Hz @400Hz ODR */
#define LIS302DL_CR2_HP_COEFF_10        (0x02 << 0) /**< ft = 0.5Hz @100Hz ODR / 2Hz @400Hz ODR */
#define LIS302DL_CR2_HP_COEFF_11        (0x03 << 0) /**< ft = 0.25Hz @100Hz ODR / 1Hz @400Hz ODR */

/**
 * @brief CTRL_REG3 (22h) - Interrupt configuration bits
 */
#define LIS302DL_CR3_IHL                (1 << 7) /**< Interrupt active level: 0 = active high, 1 = active low */
#define LIS302DL_CR3_PP_OD              (1 << 6) /**< Interrupt pad electrical configuration: 0 = push-pull, 1 = open drain */

/* Data Signal on Int2 pad control bits (I2CFG2, I2CFG1, I2CFG0) */
#define LIS302DL_CR3_I2CFG_MASK         (0x07 << 3)
#define LIS302DL_CR3_I2CFG_GND          (0x00 << 3) /**< Int2 pad tied to GND */
#define LIS302DL_CR3_I2CFG_FF_WU_1      (0x01 << 3) /**< Int2 pad maps FF_WU_1 signal */
#define LIS302DL_CR3_I2CFG_FF_WU_2      (0x02 << 3) /**< Int2 pad maps FF_WU_2 signal */
#define LIS302DL_CR3_I2CFG_FF_WU_1_OR_2 (0x03 << 3) /**< Int2 pad maps logical OR of FF_WU_1 and FF_WU_2 */
#define LIS302DL_CR3_I2CFG_DATA_READY   (0x04 << 3) /**< Int2 pad maps Data Ready signal */

/* Data Signal on Int1 pad control bits (I1CFG2, I1CFG1, I1CFG0) */
#define LIS302DL_CR3_I1CFG_MASK         (0x07 << 0)
#define LIS302DL_CR3_I1CFG_GND          (0x00 << 0) /**< Int1 pad tied to GND */
#define LIS302DL_CR3_I1CFG_FF_WU_1      (0x01 << 0) /**< Int1 pad maps FF_WU_1 signal */
#define LIS302DL_CR3_I1CFG_FF_WU_2      (0x02 << 0) /**< Int1 pad maps FF_WU_2 signal */
#define LIS302DL_CR3_I1CFG_FF_WU_1_OR_2 (0x03 << 0) /**< Int1 pad maps logical OR of FF_WU_1 and FF_WU_2 */
#define LIS302DL_CR3_I1CFG_DATA_READY   (0x04 << 0) /**< Int1 pad maps Data Ready signal */

/**
 * @brief STATUS_REG (27h) - Data status masks
 */
#define LIS302DL_STATUS_ZYXOR           (1 << 7) /**< X, Y, and Z axis Data Overrun */
#define LIS302DL_STATUS_ZOR             (1 << 6) /**< Z-axis Data Overrun */
#define LIS302DL_STATUS_YOR             (1 << 5) /**< Y-axis Data Overrun */
#define LIS302DL_STATUS_XOR             (1 << 4) /**< X-axis Data Overrun */
#define LIS302DL_STATUS_ZYXDA           (1 << 3) /**< X, Y, and Z axis new Data Available */
#define LIS302DL_STATUS_ZDA             (1 << 2) /**< Z-axis new Data Available */
#define LIS302DL_STATUS_YDA             (1 << 1) /**< Y-axis new Data Available */
#define LIS302DL_STATUS_XDA             (1 << 0) /**< X-axis new Data Available */

/**
 * @brief FF_WU_CFG_x (30h / 34h) - Interrupt generator configuration bits
 */
#define LIS302DL_FF_WU_CFG_AOI          (1 << 7) /**< AND/OR combination: 0 = OR (Wake-Up mode), 1 = AND (Free-Fall mode) */
#define LIS302DL_FF_WU_CFG_LIR          (1 << 6) /**< Latch Interrupt request: 0 = request not latched, 1 = request latched */
#define LIS302DL_FF_WU_CFG_ZHIE         (1 << 5) /**< Enable interrupt generation on Z high event */
#define LIS302DL_FF_WU_CFG_ZLIE         (1 << 4) /**< Enable interrupt generation on Z low event */
#define LIS302DL_FF_WU_CFG_YHIE         (1 << 3) /**< Enable interrupt generation on Y high event */
#define LIS302DL_FF_WU_CFG_YLIE         (1 << 2) /**< Enable interrupt generation on Y low event */
#define LIS302DL_FF_WU_CFG_XHIE         (1 << 1) /**< Enable interrupt generation on X high event */
#define LIS302DL_FF_WU_CFG_XLIE         (1 << 0) /**< Enable interrupt generation on X low event */

/**
 * @brief FF_WU_SRC_x (31h / 35h) - Interrupt source/status bits
 */
#define LIS302DL_FF_WU_SRC_IA           (1 << 6) /**< Interrupt Active flag */
#define LIS302DL_FF_WU_SRC_ZH           (1 << 5) /**< Z High event occurred */
#define LIS302DL_FF_WU_SRC_ZL           (1 << 4) /**< Z Low event occurred */
#define LIS302DL_FF_WU_SRC_YH           (1 << 3) /**< Y High event occurred */
#define LIS302DL_FF_WU_SRC_YL           (1 << 2) /**< Y Low event occurred */
#define LIS302DL_FF_WU_SRC_XH           (1 << 1) /**< X High event occurred */
#define LIS302DL_FF_WU_SRC_XL           (1 << 0) /**< X Low event occurred */

/**
 * @brief FF_WU_THS_x (32h / 36h) - Interrupt threshold bits
 */
#define LIS302DL_FF_WU_THS_DCRM         (1 << 7) /**< Counter resetting mode selection: 0 = counter reset, 1 = counter decremented */
#define LIS302DL_FF_WU_THS_MASK         (0x7F << 0) /**< 7-bit Free-fall/wake-up Threshold mask (THS6 - THS0) */


/* ========================================================================== */
/* PROTOCOL PROTO HELPER MACROS                                               */
/* ========================================================================== */

/* SPI Bitmasks for the Sub-address byte */
#define LIS302DL_SPI_READ               (1 << 7) /**< Bit 0: 1 = Read mode */
#define LIS302DL_SPI_WRITE              (0 << 7) /**< Bit 0: 0 = Write mode */
#define LIS302DL_SPI_MS_INCR            (1 << 6) /**< Bit 1: 1 = Address auto-incremented in multiple block transfers */
#define LIS302DL_SPI_MS_STAY            (0 << 6) /**< Bit 1: 0 = Address remains unchanged */

/* I2C Bitmask for the Sub-address field */
#define LIS302DL_I2C_SUB_INC_MASK       (1 << 7) /**< Most Significant Bit enables sub-address auto-increment */

#ifdef __cplusplus
}
#endif

#endif /* LIS302DL_REGS_H */