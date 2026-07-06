/**
 * ******************************************************************************
 * @file    lis3dshtr_object.h
 * @brief   Local RAM shadow object structures for LIS3DSH registers.
 * Based on STMicroelectronics technical datasheet (Doc ID 022405 Rev 1).
 * ******************************************************************************
 */

#ifndef LIS3DSHTR_OBJECT_H
#define LIS3DSHTR_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ========================================================================== */
/* SUB-STRUCTURES FOR REGISTERS GROUPS                                        */
/* ========================================================================== */

/**
 * @brief Configuration registers container (CTRL_REG3 to CTRL_REG6)
 */
typedef struct {
    uint8_t CTRL_REG3; /**< Interrupt pad configuration (23h) */
    uint8_t CTRL_REG4; /**< Output data rate and axis enablement (20h) */
    uint8_t CTRL_REG5; /**< Full-scale, Anti-aliasing, Self-test (24h) */
    uint8_t CTRL_REG6; /**< FIFO control and address increment (25h) */
} LIS3DSH_Ctrl_Regs;

/**
 * @brief 16-bit signed output data registers for X, Y, Z axes
 */
typedef struct {
    uint8_t OUT_X_L;   /**< X-axis output Low byte (28h) */
    uint8_t OUT_X_H   ; /**< X-axis output High byte (29h) */
    uint8_t OUT_Y_L;   /**< Y-axis output data Low byte (2Ah) */
    uint8_t OUT_Y_H;   /**< Y-axis output data High byte (2Bh) */
    uint8_t OUT_Z_L;   /**< Z-axis output data Low byte [Type: R] */
    uint8_t OUT_Z_H;   /**< Z-axis output data High byte [Type: R] */
} LIS3DSH_Out_Regs;

/**
 * @brief Embedded State Machine internal sub-registers (Page 30)
 */
typedef struct {
    uint8_t CTRL_REG;       /**< SM Control register (CTRL_REG1 for SM1 / CTRL_REG2 for SM2) */
    uint8_t STx[16];        /**< State Machine Code registers (16 steps opcode) */
    uint8_t sm_tim4; /**< 8-bit general timer 4 for State Machine */
    uint8_t sm_tim3; /**< 8-bit general timer 3 for State Machine */
    uint8_t TIM2_L;         /**< 16-bit general timer 2 Low byte */
    uint8_t TIM2_H;         /**< 16-bit general timer 2 High byte */
    uint8_t TIM1_L;         /**< 16-bit general timer 1 Low byte */
    uint8_t TIM1_H;         /**< 16-bit general timer 1 High byte */
    uint8_t THRS2;          /**< Threshold value 2 */
    uint8_t THRS1;          /**< Threshold value 1 */
    uint8_t MASK_B;         /**< Axis and sign swap mask */
    uint8_t MASK_A;         /**< Axis and sign default mask */
    uint8_t SETT;           /**< Peak detection and reset settings */
    uint8_t PR;             /**< Program-reset pointer */
    uint8_t TC_L;           /**< Timer counter Low byte */
    uint8_t TC_H;           /**< Timer counter High byte */
    uint8_t OUTS;           /**< Output flags axis state */
    uint8_t PEAK;           /**< Peak detected value */
    uint8_t DES;            /**< Decimation factor (SM2 only, reserved/dummy in SM1) */
} LIS3DSH_SM_CFGs;

/* ========================================================================== */
/* MAIN ACCELEROMETER LOCAL COPY OBJECT                                       */
/* ========================================================================== */

/**
 * @brief Complete shadow register map structure of the LIS3DSH hardware state
 */
typedef struct {
    /* Device Info & ID */
    uint8_t OUT_T;          /**< Temperature sensor output data (0Ch)  */
    uint8_t INFO1;          /**< Information register 1 (0Dh)  */
    uint8_t INFO2;          /**< Information register 2 (0Eh)  */
    uint8_t WHO_AM_I;       /**< Device Identification (0Fh)  */

    /* User Calibrations */
    uint8_t OFF_X;          /**< X-axis offset correction (10h)  */
    uint8_t OFF_Y;          /**< Y-axis offset correction (11h)  */
    uint8_t OFF_Z;          /**< Z-axis offset correction (12h)  */
    uint8_t CS_X;           /**< Constant shift signed value X-axis (13h)  */
    uint8_t CS_Y;           /**< Constant shift signed value Y-axis (14h)  */
    uint8_t CS_Z;           /**< Constant shift signed value Z-axis (15h)  */

    /* Core Status and Core Configurations */
    uint8_t STAT;           /**< Interrupt synchronization status (18h)  */
    LIS3DSH_Ctrl_Regs CTRL_REGS; /**< Block of control registers (20h, 23h-25h)  */
    uint8_t STATUS;         /**< Data Status indicator register (27h)  */
    
    /* System Timers & Extra Thresholds */
    uint16_t LC;            /**< 16-bit long-counter register value (16h-17h)  */
    uint8_t VFC_1;          /**< Vector filter coefficient register 1 (1Bh)  */
    uint8_t VFC_2;          /**< Vector filter coefficient register 2 (1Ch)  */
    uint8_t VFC_3;          /**< Vector filter coefficient register 3 (1Dh)  */
    uint8_t VFC_4;          /**< Vector filter coefficient register 4 (1Eh)  */
    uint8_t THRS3;          /**< Threshold value register 3 (1Fh)  */

    /* Raw Acceleration Deliverables */
    LIS3DSH_Out_Regs OUT_REGS; /**< Block of 16-bit axis output data (28h-2Dh)  */

    /* FIFO Buffers Management */
    uint8_t FIFO_CTRL;      /**< FIFO control configuration (2Eh)  */
    uint8_t FIFO_SRC;       /**< FIFO source status flags (2Fh)  */

    /* State Machines Blocks */
    LIS3DSH_SM_CFGs SM1; /**< State Machine 1 fully addressable block (Registers from 21h, 40h-5Fh) [cite: 56, 640, 647] */
    LIS3DSH_SM_CFGs SM2; /**< State Machine 2 fully addressable block (Registers from 22h, 60h-7Fh) [cite: 56, 647] */
}LIS3DSHTR_Object;

#ifdef __cplusplus
}
#endif

#endif /* LIS3DSHTR_OBJECT_H */