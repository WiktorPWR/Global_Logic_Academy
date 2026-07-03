/**
 * ******************************************************************************
 * @file    lis3dsh_regs.h
 * @brief   Register map and bit definitions for the LIS3DSH accelerometer.
 * Based on STMicroelectronics technical datasheet (Doc ID 022405 Rev 1).
 * ******************************************************************************
 */

#ifndef LIS3DSH_REGS_H
#define LIS3DSH_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ========================================================================== */
/* REGISTER MAP (Table 16, Page 29)                                           */
/* ========================================================================== */
#define LIS3DSH_REG_OUT_T               0x0C /**< Temperature output register [Type: R] */
#define LIS3DSH_REG_INFO1               0x0D /**< Information register 1 [Type: R] [Default: 0x21] */
#define LIS3DSH_REG_INFO2               0x0E /**< Information register 2 [Type: R] [Default: 0x00] */
#define LIS3DSH_REG_WHO_AM_I            0x0F /**< Device identification register [Type: R] [Default: 0x3F] */

#define LIS3DSH_REG_OFF_X               0x10 /**< X-axis offset correction [Type: RW] */
#define LIS3DSH_REG_OFF_Y               0x11 /**< Y-axis offset correction [Type: RW] */
#define LIS3DSH_REG_OFF_Z               0x12 /**< Z-axis offset correction [Type: RW] */

#define LIS3DSH_REG_STAT                0x18 /**< Interrupt synchronization register [Type: R] */

#define LIS3DSH_REG_CTRL_REG4           0x20 /**< Control register 4 (ODR, Axes enable) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG1           0x21 /**< State Machine 1 Control register [Type: RW] */
#define LIS3DSH_REG_CTRL_REG2           0x22 /**< State Machine 2 Control register [Type: RW] */
#define LIS3DSH_REG_CTRL_REG3           0x23 /**< Control register 3 (Interrupts, Reset) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG5           0x24 /**< Control register 5 (Full-Scale, BW, ST) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG6           0x25 /**< Control register 6 (FIFO, Addr Inc) [Type: RW] */
#define LIS3DSH_REG_STATUS              0x27 /**< Data status register [Type: R] */

/* Output Data Registers (16-bit resolution, two's complement) */
#define LIS3DSH_REG_OUT_X_L             0x28 /**< X-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_X_H             0x29 /**< X-axis output data High byte [Type: R] */
#define LIS3DSH_REG_OUT_Y_L             0x2A /**< Y-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_Y_H             0x2B /**< Y-axis output data High byte [Type: R] */
#define LIS3DSH_REG_OUT_Z_L             0x2C /**< Z-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_Z_H             0x2D /**< Z-axis output data High byte [Type: R] */

#define LIS3DSH_REG_FIFO_CTRL           0x2E /**< FIFO control register [Type: RW] */
#define LIS3DSH_REG_FIFO_SRC            0x2F /**< FIFO source register [Type: R] */


/* ========================================================================== */
/* CONSTANTS AND EXPECTED VALUES                                              */
/* ========================================================================== */
#define LIS3DSH_WHO_AM_I_VAL            0x3F /**< Expected WHO_AM_I register content (Page 29, 32) */

/* SPI Protocol Bitmasks (Page 25) */
#define LIS3DSH_SPI_READ                (1 << 7) /**< Bit 0: 1 = Read mode */
#define LIS3DSH_SPI_WRITE               (0 << 7) /**< Bit 0: 0 = Write mode */


/* ========================================================================== */
/* REGISTER BIT MASKS & CONFIG VALUES                                         */
/* ========================================================================== */

/**
 * @brief CTRL_REG4 (20h) - Output Data Rate and Axes Enable (Page 33)
 */
#define LIS3DSH_CR4_ODR_MASK            (0x0F << 4)
#define LIS3DSH_CR4_ODR_POWER_DOWN      (0x00 << 4)
#define LIS3DSH_CR4_ODR_3_125HZ         (0x01 << 4)
#define LIS3DSH_CR4_ODR_6_25HZ          (0x02 << 4)
#define LIS3DSH_CR4_ODR_12_5HZ          (0x03 << 4)
#define LIS3DSH_CR4_ODR_25HZ            (0x04 << 4)
#define LIS3DSH_CR4_ODR_50HZ            (0x05 << 4)
#define LIS3DSH_CR4_ODR_100HZ           (0x06 << 4)
#define LIS3DSH_CR4_ODR_400HZ           (0x07 << 4)
#define LIS3DSH_CR4_ODR_800HZ           (0x08 << 4)
#define LIS3DSH_CR4_ODR_1600HZ          (0x09 << 4)

#define LIS3DSH_CR4_BDU                 (1 << 3)  /**< Block Data Update: 1 = inhibit update until MSB & LSB read */
#define LIS3DSH_CR4_ZEN                 (1 << 2)  /**< Z-axis enable */
#define LIS3DSH_CR4_YEN                 (1 << 1)  /**< Y-axis enable */
#define LIS3DSH_CR4_XEN                 (1 << 0)  /**< X-axis enable */

/**
 * @brief CTRL_REG3 (23h) - Interrupt and Reset Configuration (Page 32)
 */
#define LIS3DSH_CR3_DR_EN               (1 << 7)  /**< DRDY signal enable to INT1 */
#define LIS3DSH_CR3_IEA                 (1 << 6)  /**< Interrupt polarity: 0=Active Low, 1=Active High */
#define LIS3DSH_CR3_IEL                 (1 << 5)  /**< Interrupt latching: 0=latched, 1=pulsed */
#define LIS3DSH_CR3_INT2_EN             (1 << 4)  /**< INT2 signal enable */
#define LIS3DSH_CR3_INT1_EN             (1 << 3)  /**< INT1/DRDY signal enable */
#define LIS3DSH_CR3_VFILT               (1 << 2)  /**< Vector filter enable */
#define LIS3DSH_CR3_STRT                (1 << 0)  /**< Soft reset bit (POR function) */

/**
 * @brief CTRL_REG5 (24h) - Full-Scale, BW, and SPI Interface Mode (Page 34)
 */
#define LIS3DSH_CR5_BW_MASK             (0x03 << 6)
#define LIS3DSH_CR5_BW_800HZ            (0x00 << 6)
#define LIS3DSH_CR5_BW_400HZ            (0x01 << 6)
#define LIS3DSH_CR5_BW_200HZ            (0x02 << 6)
#define LIS3DSH_CR5_BW_50HZ             (0x03 << 6)

#define LIS3DSH_CR5_FSCALE_MASK         (0x07 << 3)
#define LIS3DSH_CR5_FS_2G               (0x00 << 3)
#define LIS3DSH_CR5_FS_4G               (0x01 << 3)
#define LIS3DSH_CR5_FS_6G               (0x02 << 3)
#define LIS3DSH_CR5_FS_8G               (0x03 << 3)
#define LIS3DSH_CR5_FS_16G              (0x04 << 3)

#define LIS3DSH_CR5_ST_MASK             (0x03 << 1)
#define LIS3DSH_CR5_ST_NORMAL           (0x00 << 1)
#define LIS3DSH_CR5_ST_POSITIVE         (0x01 << 1)
#define LIS3DSH_CR5_ST_NEGATIVE         (0x02 << 1)

#define LIS3DSH_CR5_SIM                 (1 << 0)  /**< SPI Mode: 0 = 4-wire, 1 = 3-wire */

/**
 * @brief CTRL_REG6 (25h) - FIFO and Address Increment (Page 34)
 */
#define LIS3DSH_CR6_BOOT                (1 << 7)  /**< Force reboot memory content */
#define LIS3DSH_CR6_FIFO_EN             (1 << 6)  /**< FIFO enable */
#define LIS3DSH_CR6_WTM_EN              (1 << 5)  /**< Enable FIFO Watermark */
#define LIS3DSH_CR6_ADD_INC             (1 << 4)  /**< Auto address increment during multiple byte access */

/**
 * @brief STATUS (27h) - Data Status Flags (Page 35)
 */
#define LIS3DSH_STATUS_ZYXOR            (1 << 7)  /**< X, Y, and Z axis data overrun */
#define LIS3DSH_STATUS_ZOR              (1 << 6)  /**< Z-axis data overrun */
#define LIS3DSH_STATUS_YOR              (1 << 5)  /**< Y-axis data overrun */
#define LIS3DSH_STATUS_XOR              (1 << 4)  /**< X-axis data overrun */
#define LIS3DSH_STATUS_ZYXDA            (1 << 3)  /**< X, Y, and Z axis new data available */
#define LIS3DSH_STATUS_ZDA              (1 << 2)  /**< Z-axis new data available */
#define LIS3DSH_STATUS_YDA              (1 << 1)  /**< Y-axis new data available */
#define LIS3DSH_STATUS_XDA              (1 << 0)  /**< X-axis new data available */


/* ========================================================================== */
/* OPERATIONAL HELPERS                                                        */
/* ========================================================================== */
static const uint8_t LIS3DSH_Register_Addresses[] = {
    LIS3DSH_REG_OUT_T, LIS3DSH_REG_INFO1, LIS3DSH_REG_INFO2, LIS3DSH_REG_WHO_AM_I,
    LIS3DSH_REG_OFF_X, LIS3DSH_REG_OFF_Y, LIS3DSH_REG_OFF_Z, LIS3DSH_REG_STAT,
    LIS3DSH_REG_CTRL_REG4, LIS3DSH_REG_CTRL_REG1, LIS3DSH_REG_CTRL_REG2,
    LIS3DSH_REG_CTRL_REG3, LIS3DSH_REG_CTRL_REG5, LIS3DSH_REG_CTRL_REG6,
    LIS3DSH_REG_STATUS, LIS3DSH_REG_OUT_X_L, LIS3DSH_REG_OUT_X_H,
    LIS3DSH_REG_OUT_Y_L, LIS3DSH_REG_OUT_Y_H, LIS3DSH_REG_OUT_Z_L, LIS3DSH_REG_OUT_Z_H
};

#define LIS3DSH_NUM_REGISTERS (sizeof(LIS3DSH_Register_Addresses) / sizeof(LIS3DSH_Register_Addresses[0]))

#ifdef __cplusplus
}
#endif

#endif /* LIS3DSH_REGS_H */