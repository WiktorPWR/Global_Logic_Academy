/**
 * ******************************************************************************
 * @file    lis3dsh_regs.h
 * @brief   Complete Register map and bit definitions for the LIS3DSH accelerometer.
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
/* COMPLETE REGISTER MAP (Section 7, Table 16, Page 29-31)                    */
/* ========================================================================== */
#define LIS3DSH_REG_OUT_T               0x0C /**< Temperature output register [Type: R] */
#define LIS3DSH_REG_INFO1               0x0D /**< Information register 1 [Type: R] [Default: 0x21] */
#define LIS3DSH_REG_INFO2               0x0E /**< Information register 2 [Type: R] [Default: 0x00] */
#define LIS3DSH_REG_WHO_AM_I            0x0F /**< Device identification register [Type: R] [Default: 0x3F] */

/* User Offset / Calibration Registers */
#define LIS3DSH_REG_OFF_X               0x10 /**< X-axis offset correction [Type: RW] */
#define LIS3DSH_REG_OFF_Y               0x11 /**< Y-axis offset correction [Type: RW] */
#define LIS3DSH_REG_OFF_Z               0x12 /**< Z-axis offset correction [Type: RW] */
#define LIS3DSH_REG_CS_X                0x13 /**< Constant shift signed value X-axis [Type: RW] */
#define LIS3DSH_REG_CS_Y                0x14 /**< Constant shift signed value Y-axis [Type: RW] */
#define LIS3DSH_REG_CS_Z                0x15 /**< Constant shift signed value Z-axis [Type: RW] */

/* System Long Counter & Interrupt Synchronization */
#define LIS3DSH_REG_LC_L                0x16 /**< Long counter register Low byte [Type: RW] */
#define LIS3DSH_REG_LC_H                0x17 /**< Long counter register High byte [Type: RW] */
#define LIS3DSH_REG_STAT                0x18 /**< Interrupt synchronization register [Type: R] */
#define LIS3DSH_REG_PEAK1               0x19 /**< Peak detection value register for SM1 [Type: R] */
#define LIS3DSH_REG_PEAK2               0x1A /**< Peak detection value register for SM2 [Type: R] */

/* Vector Filter Coefficients */
#define LIS3DSH_REG_VFC_1               0x1B /**< Vector filter coefficient register 1 [Type: RW] */
#define LIS3DSH_REG_VFC_2               0x1C /**< Vector filter coefficient register 2 [Type: RW] */
#define LIS3DSH_REG_VFC_3               0x1D /**< Vector filter coefficient register 3 [Type: RW] */
#define LIS3DSH_REG_VFC_4               0x1E /**< Vector filter coefficient register 4 [Type: RW] */
#define LIS3DSH_REG_THRS3               0x1F /**< Threshold value register 3 [Type: RW] */

/* Main Control Registers */
#define LIS3DSH_REG_CTRL_REG4           0x20 /**< Control register 4 (ODR, Axes enable) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG1           0x21 /**< State Machine 1 Control register [Type: RW] */
#define LIS3DSH_REG_CTRL_REG2           0x22 /**< State Machine 2 Control register [Type: RW] */
#define LIS3DSH_REG_CTRL_REG3           0x23 /**< Control register 3 (Interrupts, Reset) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG5           0x24 /**< Control register 5 (Full-Scale, BW, ST) [Type: RW] */
#define LIS3DSH_REG_CTRL_REG6           0x25 /**< Control register 6 (FIFO, Addr Inc) [Type: RW] */
#define LIS3DSH_REG_STATUS              0x27 /**< Data status register [Type: R] */

/* Raw Acceleration Output Data Registers (16-bit, Two's Complement) */
#define LIS3DSH_REG_OUT_X_L             0x28 /**< X-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_X_H             0x29 /**< X-axis output data High byte [Type: R] */
#define LIS3DSH_REG_OUT_Y_L             0x2A /**< Y-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_Y_H             0x2B /**< Y-axis output data High byte [Type: R] */
#define LIS3DSH_REG_OUT_Z_L             0x2C /**< Z-axis output data Low byte [Type: R] */
#define LIS3DSH_REG_OUT_Z_H             0x2D /**< Z-axis output data High byte [Type: R] */

/* Hardware FIFO Buffer System */
#define LIS3DSH_REG_FIFO_CTRL           0x2E /**< FIFO control register [Type: RW] */
#define LIS3DSH_REG_FIFO_SRC            0x2F /**< FIFO source register [Type: R] */

/* ========================================================================== */
/* STATE MACHINE 1 CODE & SYSTEM REGISTERS (Addresses 0x40 to 0x5F)          */
/* ========================================================================== */
#define LIS3DSH_REG_ST1_1               0x40 /**< SM1 Code Step 1 register [Type: W] */
#define LIS3DSH_REG_ST1_2               0x41 /**< SM1 Code Step 2 register [Type: W] */
#define LIS3DSH_REG_ST1_3               0x42 /**< SM1 Code Step 3 register [Type: W] */
#define LIS3DSH_REG_ST1_4               0x43 /**< SM1 Code Step 4 register [Type: W] */
#define LIS3DSH_REG_ST1_5               0x44 /**< SM1 Code Step 5 register [Type: W] */
#define LIS3DSH_REG_ST1_6               0x45 /**< SM1 Code Step 6 register [Type: W] */
#define LIS3DSH_REG_ST1_7               0x46 /**< SM1 Code Step 7 register [Type: W] */
#define LIS3DSH_REG_ST1_8               0x47 /**< SM1 Code Step 8 register [Type: W] */
#define LIS3DSH_REG_ST1_9               0x48 /**< SM1 Code Step 9 register [Type: W] */
#define LIS3DSH_REG_ST1_10              0x49 /**< SM1 Code Step 10 register [Type: W] */
#define LIS3DSH_REG_ST1_11              0x4A /**< SM1 Code Step 11 register [Type: W] */
#define LIS3DSH_REG_ST1_12              0x4B /**< SM1 Code Step 12 register [Type: W] */
#define LIS3DSH_REG_ST1_13              0x4C /**< SM1 Code Step 13 register [Type: W] */
#define LIS3DSH_REG_ST1_14              0x4D /**< SM1 Code Step 14 register [Type: W] */
#define LIS3DSH_REG_ST1_15              0x4E /**< SM1 Code Step 15 register [Type: W] */
#define LIS3DSH_REG_ST1_16              0x4F /**< SM1 Code Step 16 register [Type: W] */

#define LIS3DSH_REG_TIM4_1              0x50 /**< SM1 General Timer 4 [Type: W] */
#define LIS3DSH_REG_TIM3_1              0x51 /**< SM1 General Timer 3 [Type: W] */
#define LIS3DSH_REG_TIM2_1_L            0x52 /**< SM1 General Timer 2 Low byte [Type: W] */
#define LIS3DSH_REG_TIM2_1_H            0x53 /**< SM1 General Timer 2 High byte [Type: W] */
#define LIS3DSH_REG_TIM1_1_L            0x54 /**< SM1 General Timer 1 Low byte [Type: W] */
#define LIS3DSH_REG_TIM1_1_H            0x55 /**< SM1 General Timer 1 High byte [Type: W] */
#define LIS3DSH_REG_THRS2_1             0x56 /**< SM1 Threshold Value 1 [Type: W] */
#define LIS3DSH_REG_THRS1_1             0x57 /**< SM1 Threshold Value 2 [Type: W] */
/* Adres 0x58 jest zarezerwowany (brak rejestru DES w strukturze SM1) */
#define LIS3DSH_REG_MASK1_B             0x59 /**< SM1 Axis and Sign Swap Mask [Type: W] */
#define LIS3DSH_REG_MASK1_A             0x5A /**< SM1 Axis and Sign Default Mask [Type: W] */
#define LIS3DSH_REG_SETT1               0x5B /**< SM1 Detection Settings and Flags [Type: W] */
#define LIS3DSH_REG_PR1                 0x5C /**< SM1 Program and Reset Pointer [Type: R] */
#define LIS3DSH_REG_TC1_L               0x5D /**< SM1 Timer Counter Output Low byte [Type: R] */
#define LIS3DSH_REG_TC1_H               0x5E /**< SM1 Timer Counter Output High byte [Type: R] */
#define LIS3DSH_REG_OUTS1               0x5F /**< SM1 Axis Output Flag Management [Type: R] */

/* ========================================================================== */
/* STATE MACHINE 2 CODE & SYSTEM REGISTERS (Addresses 0x60 to 0x7F)          */
/* ========================================================================== */
#define LIS3DSH_REG_ST2_1               0x60 /**< SM2 Code Step 1 register [Type: W] */
#define LIS3DSH_REG_ST2_2               0x61 /**< SM2 Code Step 2 register [Type: W] */
#define LIS3DSH_REG_ST2_3               0x62 /**< SM2 Code Step 3 register [Type: W] */
#define LIS3DSH_REG_ST2_4               0x63 /**< SM2 Code Step 4 register [Type: W] */
#define LIS3DSH_REG_ST2_5               0x64 /**< SM2 Code Step 5 register [Type: W] */
#define LIS3DSH_REG_ST2_6               0x65 /**< SM2 Code Step 6 register [Type: W] */
#define LIS3DSH_REG_ST2_7               0x66 /**< SM2 Code Step 7 register [Type: W] */
#define LIS3DSH_REG_ST2_8               0x67 /**< SM2 Code Step 8 register [Type: W] */
#define LIS3DSH_REG_ST2_9               0x68 /**< SM2 Code Step 9 register [Type: W] */
#define LIS3DSH_REG_ST2_10              0x69 /**< SM2 Code Step 10 register [Type: W] */
#define LIS3DSH_REG_ST2_11              0x6A /**< SM2 Code Step 11 register [Type: W] */
#define LIS3DSH_REG_ST2_12              0x6B /**< SM2 Code Step 12 register [Type: W] */
#define LIS3DSH_REG_ST2_13              0x6C /**< SM2 Code Step 13 register [Type: W] */
#define LIS3DSH_REG_ST2_14              0x6D /**< SM2 Code Step 14 register [Type: W] */
#define LIS3DSH_REG_ST2_15              0x6E /**< SM2 Code Step 15 register [Type: W] */
#define LIS3DSH_REG_ST2_16              0x6F /**< SM2 Code Step 16 register [Type: W] */

#define LIS3DSH_REG_TIM4_2              0x70 /**< SM2 General Timer 4 [Type: W] */
#define LIS3DSH_REG_TIM3_2              0x71 /**< SM2 General Timer 3 [Type: W] */
#define LIS3DSH_REG_TIM2_2_L            0x72 /**< SM2 General Timer 2 Low byte [Type: W] */
#define LIS3DSH_REG_TIM2_2_H            0x73 /**< SM2 General Timer 2 High byte [Type: W] */
#define LIS3DSH_REG_TIM1_2_L            0x74 /**< SM2 General Timer 1 Low byte [Type: W] */
#define LIS3DSH_REG_TIM1_2_H            0x75 /**< SM2 General Timer 1 High byte [Type: W] */
#define LIS3DSH_REG_THRS2_2             0x76 /**< SM2 Threshold Value 1 [Type: W] */
#define LIS3DSH_REG_THRS1_2             0x77 /**< SM2 Threshold Value 2 [Type: W] */
#define LIS3DSH_REG_DES2                0x78 /**< SM2 Decimation Factor register [Type: W] */
#define LIS3DSH_REG_MASK2_B             0x79 /**< SM2 Axis and Sign Swap Mask [Type: W] */
#define LIS3DSH_REG_MASK2_A             0x7A /**< SM2 Axis and Sign Default Mask [Type: W] */
#define LIS3DSH_REG_SETT2               0x7B /**< SM2 Detection Settings and Flags [Type: W] */
#define LIS3DSH_REG_PR2                 0x7C /**< SM2 Program and Reset Pointer [Type: R] */
#define LIS3DSH_REG_TC2_L               0x7D /**< SM2 Timer Counter Output Low byte [Type: R] */
#define LIS3DSH_REG_TC2_H               0x7E /**< SM2 Timer Counter Output High byte [Type: R] */
#define LIS3DSH_REG_OUTS2               0x7F /**< SM2 Axis Output Flag Management [Type: R] */


/* ========================================================================== */
/* CONSTANTS AND PROTOCOL HELPER VALUES                                      */
/* ========================================================================== */
#define LIS3DSH_WHO_AM_I_VAL            0x3F /**< Hardware WHO_AM_I value [cite: 56, 640, 676] */

/* SPI Bitmasks for transaction control (Section 6.2, Page 24-25) */
#define LIS3DSH_SPI_READ                (1 << 7) /**< Bit 0: 1 = Read mode [cite: 539, 567] */
#define LIS3DSH_SPI_WRITE               (0 << 7) /**< Bit 0: 0 = Write mode [cite: 539, 598] */

/* SPI M/S Auto-increment Bitmasks (Section 6.2, Page 25) */
#define LIS3DSH_SPI_MS_INCR             (0 << 6) /**< Bit 1: 0 = Address auto-incremented [cite: 526, 536] */
#define LIS3DSH_SPI_MS_STAY             (1 << 6) /**< Bit 1: 1 = Address remains unchanged [cite: 526, 536] */


/* ========================================================================== */
/* REGISTER BIT MASKS & VALUE MACROS                                          */
/* ========================================================================== */

/**
 * @brief CTRL_REG4 (20h) - Output Data Rate and Axes Enable (Page 33)
 */
#define LIS3DSH_CR4_ODR_MASK            (0x0F << 4) /**< [cite: 697, 701] */
#define LIS3DSH_CR4_ODR_POWER_DOWN      (0x00 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_3_125HZ         (0x01 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_6_25HZ          (0x02 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_12_5HZ          (0x03 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_25HZ            (0x04 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_50HZ            (0x05 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_100HZ           (0x06 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_400HZ           (0x07 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_800HZ           (0x08 << 4) /**< [cite: 705] */
#define LIS3DSH_CR4_ODR_1600HZ          (0x09 << 4) /**< [cite: 705] */

#define LIS3DSH_CR4_BDU                 (1 << 3)  /**< Block Data Update [cite: 697, 701] */
#define LIS3DSH_CR4_ZEN                 (1 << 2)  /**< Z-axis enable [cite: 697, 701] */
#define LIS3DSH_CR4_YEN                 (1 << 1)  /**< Y-axis enable [cite: 697, 701] */
#define LIS3DSH_CR4_XEN                 (1 << 0)  /**< X-axis enable [cite: 697, 701] */

/**
 * @brief CTRL_REG3 (23h) - Interrupt and Reset Configuration (Page 32)
 */
#define LIS3DSH_CR3_DR_EN               (1 << 7)  /**< DRDY signal enable to INT1 [cite: 681, 685] */
#define LIS3DSH_CR3_IEA                 (1 << 6)  /**< Interrupt polarity (0:ActiveL, 1:ActiveH) [cite: 681, 685] */
#define LIS3DSH_CR3_IEL                 (1 << 5)  /**< Interrupt latching (0:latched, 1:pulsed) [cite: 681, 685] */
#define LIS3DSH_CR3_INT2_EN             (1 << 4)  /**< INT2 signal enable [cite: 681, 685] */
#define LIS3DSH_CR3_INT1_EN             (1 << 3)  /**< INT1/DRDY signal enable [cite: 681, 685] */
#define LIS3DSH_CR3_VFILT               (1 << 2)  /**< Vector filter enable [cite: 681, 690] */
#define LIS3DSH_CR3_STRT                (1 << 0)  /**< Soft reset bit [cite: 681, 690] */

/**
 * @brief CTRL_REG5 (24h) - Full-Scale, BW, and SPI Interface Mode (Page 34)
 */
#define LIS3DSH_CR5_BW_MASK             (0x03 << 6) /**< [cite: 717, 720] */
#define LIS3DSH_CR5_BW_800HZ            (0x00 << 6) /**< [cite: 720] */
#define LIS3DSH_CR5_BW_400HZ            (0x01 << 6) /**< [cite: 720] */
#define LIS3DSH_CR5_BW_200HZ            (0x02 << 6) /**< [cite: 720] */
#define LIS3DSH_CR5_BW_50HZ             (0x03 << 6) /**< [cite: 720] */

#define LIS3DSH_CR5_FSCALE_MASK         (0x07 << 3) /**< [cite: 717, 720] */
#define LIS3DSH_CR5_FS_2G               (0x00 << 3) /**< [cite: 720] */
#define LIS3DSH_CR5_FS_4G               (0x01 << 3) /**< [cite: 720] */
#define LIS3DSH_CR5_FS_6G               (0x02 << 3) /**< [cite: 720] */
#define LIS3DSH_CR5_FS_8G               (0x03 << 3) /**< [cite: 720] */
#define LIS3DSH_CR5_FS_16G              (0x04 << 3) /**< [cite: 720] */

#define LIS3DSH_CR5_ST_MASK             (0x03 << 1) /**< [cite: 717, 720] */
#define LIS3DSH_CR5_ST_NORMAL           (0x00 << 1) /**< [cite: 723] */
#define LIS3DSH_CR5_ST_POSITIVE         (0x01 << 1) /**< [cite: 723] */
#define LIS3DSH_CR5_ST_NEGATIVE         (0x02 << 1) /**< [cite: 723] */

#define LIS3DSH_CR5_SIM                 (1 << 0)  /**< SPI Mode (0:4-wire, 1:3-wire) [cite: 717, 720] */

/**
 * @brief CTRL_REG6 (25h) - FIFO and Address Increment (Page 34)
 */
#define LIS3DSH_CR6_BOOT                (1 << 7)  /**< Force reboot memory content [cite: 727, 735] */
#define LIS3DSH_CR6_FIFO_EN             (1 << 6)  /**< FIFO enable [cite: 727, 735] */
#define LIS3DSH_CR6_WTM_EN              (1 << 5)  /**< Enable FIFO Watermark [cite: 727, 735] */
#define LIS3DSH_CR6_ADD_INC             (1 << 4)  /**< Auto address increment enable [cite: 727, 735] */

/**
 * @brief STATUS (27h) - Data Status Flags (Page 35)
 */
#define LIS3DSH_STATUS_ZYXOR            (1 << 7)  /**< X, Y, and Z axis data overrun [cite: 740, 743] */
#define LIS3DSH_STATUS_ZOR              (1 << 6)  /**< Z-axis data overrun [cite: 740, 743] */
#define LIS3DSH_STATUS_YOR              (1 << 5)  /**< Y-axis data overrun [cite: 740, 743] */
#define LIS3DSH_STATUS_XOR              (1 << 4)  /**< X-axis data overrun [cite: 740, 743] */
#define LIS3DSH_STATUS_ZYXDA            (1 << 3)  /**< X, Y, and Z axis new data available [cite: 740, 743] */
#define LIS3DSH_STATUS_ZDA              (1 << 2)  /**< Z-axis new data available [cite: 740, 743] */
#define LIS3DSH_STATUS_YDA              (1 << 1)  /**< Y-axis new data available [cite: 740, 749] */
#define LIS3DSH_STATUS_XDA              (1 << 0)  /**< X-axis new data available [cite: 740, 749] */

/**
 * @brief FIFO_CTRL (2Eh) - FIFO Operational Modes (Page 40)
 */
#define LIS3DSH_FIFO_MODE_BYPASS        (0x00 << 5) /**< [cite: 912, 917] */
#define LIS3DSH_FIFO_MODE_FIFO          (0x01 << 5) /**< [cite: 912, 917] */
#define LIS3DSH_FIFO_MODE_STREAM        (0x02 << 5) /**< [cite: 912, 917] */
#define LIS3DSH_FIFO_MODE_STREAM_FIFO   (0x03 << 5) /**< [cite: 912, 917] */
#define LIS3DSH_FIFO_MODE_BYPASS_STREAM (0x04 << 5) /**< [cite: 912, 917] */
#define LIS3DSH_FIFO_MODE_BYPASS_FIFO   (0x07 << 5) /**< [cite: 912, 917] */

#ifdef __cplusplus
}
#endif

#endif /* LIS3DSH_REGS_H */