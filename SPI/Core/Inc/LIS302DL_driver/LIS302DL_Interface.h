#ifndef LIS302DL_INTERFACE_H
#define LIS302DL_INTERFACE_H

#include "main.h"
#include <stdint.h>

/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */

/**
 * @brief  Initializes the LIS302DL device with safe default parameters.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Init(void);

/**
 * @brief  Checks device availability by reading and validating the WHO_AM_I register.
 * @retval HAL_OK if device is present, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef LIS302DL_Check_ID(void);


HAL_StatusTypeDef LIS302DL_Core_Config(uint8_t data_rate, uint8_t power_mode, uint8_t full_scale);



/* ========================================================================== */
/* 2. DATA ACQUISITION & ACCELERATION READING                                 */
/* ========================================================================== */

/**
 * @brief  Reads current raw 8-bit acceleration data for X, Y, and Z axes.
 * @param  x, y, z: Pointers to int8_t variables to store the results.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Read_XYZ_Values(int8_t *x, int8_t *y, int8_t *z);

/**
 * @brief  Reads the internal data status register (data ready, overrun flags).
 * @param  status: Pointer to a variable where status mask will be saved.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Read_Status(uint8_t *status);


/* ========================================================================== */
/* 3. HARDWARE HIGH-PASS FILTER CONFIGURATION                                 */
/* ========================================================================== */

/**
 * @brief  Configures internal high-pass filter paths and cut-off frequencies.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Filter_Config(uint8_t filter_config);

/**
 * @brief  Instantaneously resets the high-pass filter internal memory.
 * @note   Highly useful to clear false transient states after reorientation.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Filter_Reset(void);


/* ========================================================================== */
/* 4. INTERRUPT GENERATION MANAGEMENT (Wake-Up / Free-Fall)                   */
/* ========================================================================== */

/**
 * @brief  Enables interrupt conditions for selected axes and directions (High/Low).
 * @param  gen_id: Select generator (1 for FF_WU_1, 2 for FF_WU_2).
 * @param  cfg_mask: Bitmask combining event axes (e.g., LIS302DL_FF_WU_CFG_XHIE).
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Interrupt_Config(uint8_t gen_id, uint8_t cfg_mask);

/**
 * @brief  Sets the acceleration threshold for the chosen interrupt generator.
 * @param  gen_id: Select generator (1 or 2).
 * @param  threshold: 7-bit threshold value.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Interrupt_Threshold_Config(uint8_t gen_id, uint8_t threshold);

/**
 * @brief  Sets the minimum event duration before an interrupt is triggered.
 * @param  gen_id: Select generator (1 or 2).
 * @param  duration: Minimum duration value (step depends on ODR).
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Interrupt_Duration_Config(uint8_t gen_id, uint8_t duration);

/**
 * @brief  Configures the internal routing of interrupt signals to the physical pads.
 * @param  pad_config: Value configuring active levels, push-pull/OD, and signal mapping.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Interrupt_Signal_Mapping(uint8_t pad_config);

/**
 * @brief  Reads the source register of an interrupt generator to identify the axis that fired.
 * @note   Reading this register automatically clears the latched interrupt line.
 * @param  gen_id: Select generator (1 or 2).
 * @param  source: Pointer to store the interrupt source bitmask.
 * @retval HAL Status
 */
HAL_StatusTypeDef LIS302DL_Interrupt_Read_Source(uint8_t gen_id, uint8_t *source);


#endif /* LIS302DL_INTERFACE_H */