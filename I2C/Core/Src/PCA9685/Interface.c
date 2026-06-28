#include "PCA9685/Interface.h"
#include "PCA9685/Registers.h"
#include "main.h"

/* External hardware handles from CubeMX */
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;

/**
 * @brief  Calculates the 12-bit counter value matching a 0-100% duty cycle.
 * @note   Internal helper function utilizing fast integer math.
 * @param  duty_cycle: Target duty cycle value in percent (0 to 100).
 * @retval 12-bit numerical step representation for the register (0 to 4095).
 */
static uint16_t duty_cycle_calculation(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        duty_cycle = 100; /* Saturate to 100% boundary to prevent register overflow */
    }
    return (uint16_t)((duty_cycle * PCA9685_LED_VALUE_MASK) / 100); /* Scale percentage to 12-bit resolution */
}

/**
 * @brief  Sets the PWM duty cycle for a specific channel.
 * @note   Utilizes I2C sequential writing to fill all 4 channel configuration 
 * registers in a single bus transaction[cite: 351, 352].
 * @param  canal: Target PCA9685 channel index (0 to 15)[cite: 7].
 * @param  duty_cycle: Desired PWM duty cycle value in percent (0 to 100).
 * @retval None
 */
void canal_set_pwm_duty_cycle(uint8_t canal, uint8_t duty_cycle) {
    /* 1. Calculate the final 12-bit value (0 - 4095) [cite: 415] */
    uint16_t calculated_steps = duty_cycle_calculation(duty_cycle);
    if (calculated_steps > PCA9685_LED_VALUE_MASK) {
        calculated_steps = PCA9685_LED_VALUE_MASK;
    }

    /* 2. Prepare the buffer: 1 byte for register address + 4 bytes for data */
    uint8_t data[5];
    
    /* Starting sequential address points to the LEDn_ON_L register of the chosen channel [cite: 408] */
    data[0] = PCA9685_GET_REG_LED_ON_L(canal);
    
    /* LEDn_ON count = 0 (Turn ON immediately at the start of the counter cycle) [cite: 416] */
    data[1] = 0x00; /* LEDn_ON_L [cite: 622] */
    data[2] = 0x00; /* LEDn_ON_H [cite: 622] */
    
    /* LEDn_OFF count = duty_cycle (Turn OFF when counter matches this value) [cite: 416] */
    data[3] = calculated_steps & 0xFF;         /* LEDn_OFF_L (lower 8 bits) [cite: 622] */
    data[4] = (calculated_steps >> 8) & 0x0F;  /* LEDn_OFF_H (upper 4 bits) [cite: 622] */

    /* 3. Send everything in one single 8-bit I2C master transaction */
    uint16_t dev_address = (PCA9685_I2C_BASE_ADDR << 1); 
    
    HAL_I2C_Master_Transmit(&hi2c1, dev_address, data, 5, HAL_MAX_DELAY);
}

/**
 * @brief  Wakes up the PCA9685 device from low-power sleep mode[cite: 350].
 * @note   This procedure reads the register state first to prevent overwriting other configuration flags,
 * implements a mandatory polling delay using TIM2 for oscillator stabilization[cite: 357, 375],
 * and utilizes the RESTART bit to resume active channels without losing parameters[cite: 370, 371, 377].
 * @retval None
 */
void wake_up_procedure(void) {
    uint8_t mode1 = 0x00;
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Read the current state of MODE1 to preserve other flags (like Auto-Increment) [cite: 373] */
    HAL_I2C_Mem_Read(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 2. Clear the SLEEP bit to wake up the internal oscillator [cite: 374] */
    mode1 &= ~PCA9685_MODE1_SLEEP_BIT; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 3. Mandatory hardware delay for oscillator stabilization (min. 500 microseconds) [cite: 357, 375] */
    __HAL_TIM_SET_COUNTER(&htim2, 0);              /* Reset counter to absolute 0 */
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);  /* Clear any old overflow/update flags */
    
    HAL_TIM_Base_Start(&htim2);                    /* Start the timer (it will count up to 600 and stop) */
    
    /* Elegant blocking poll - wait until the timer finishes its 600us single run */
    while (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) == RESET) {
        /* Just wait for the hardware flag to switch to SET */
    }
    
    HAL_TIM_Base_Stop(&htim2);                     /* Stop the timer cleanly */

    /* 4. Set the RESTART bit to resume previous PWM channels synchronously [cite: 376, 377] */
    mode1 |= PCA9685_MODE1_RESTART_BIT; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
}

/**
 * @brief  Puts the PCA9685 device into low-power sleep mode[cite: 350].
 * @note   Preserves existing bit configurations while explicitly shutting down the internal oscillator[cite: 350].
 * Clears the RESTART bit before entering sleep as requested by the specifications[cite: 350, 384].
 * @retval None
 */
void sleep_procedure(void) {
    uint8_t mode1 = 0x00;
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Read the current state of MODE1 to preserve other flags (like Auto-Increment) [cite: 373] */
    HAL_I2C_Mem_Read(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 2. Set the SLEEP bit to 1, and CLEAR the RESTART bit (required by datasheet when entering sleep) [cite: 350, 384] */
    mode1 = (mode1 & ~PCA9685_MODE1_RESTART_BIT) | PCA9685_MODE1_SLEEP_BIT;

    /* 3. Write the updated byte back to the device */
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
}

/**
 * @brief  Changes the global PWM output modulation frequency for all channels[cite: 8, 688].
 * @note   The device must sleep to unlock the PRE_SCALE register[cite: 335, 696]. This implementation 
 * uses integer-only arithmetic with a rounding trick to preserve accuracy without floats.
 * @param  frequency: Target output modulation frequency in Hz (typically 40 Hz to 1000 Hz)[cite: 7].
 * @retval None
 */
void canals_set_pwm_frequency(uint16_t frequency) {
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Calculate the denominator (4096 * Frequency) [cite: 690] */
    /* We use uint32_t to prevent any chance of overflow during multiplication */
    uint32_t denominator = PCA9685_COUNTER_STEPS * frequency;

    /* 2. Apply the integer rounding trick: (Numerator + Denominator / 2) / Denominator [cite: 690] */
    /* Internal oscillator clock is exactly 25 000 000 Hz [cite: 49] */
    uint32_t prescale_exact = (PCA9685_INTERNAL_OSC_HZ + (denominator / 2)) / denominator;
    
    /* 3. Subtract 1 as required by the datasheet formula [cite: 690] */
    int32_t prescale_int = (int32_t)prescale_exact - 1;

    /* 4. Hardware constraints check (PCA9685 allows values between 3 and 255) [cite: 687] */
    if (prescale_int < PCA9685_MIN_PRESCALE) {
        prescale_int = PCA9685_MIN_PRESCALE;
    } else if (prescale_int > PCA9685_MAX_PRESCALE) {
        prescale_int = PCA9685_MAX_PRESCALE;
    }

    uint8_t prescale_reg_value = (uint8_t)prescale_int;

    /* 5. Execute the hardware update sequence */
    sleep_procedure(); /* Put the device into sleep mode (unlocks PRE_SCALE register) [cite: 335, 696] */

    /* Write to the PRE_SCALE register (0xFE) [cite: 334] */
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_PRE_SCALE, I2C_MEMADD_SIZE_8BIT, &prescale_reg_value, 1, HAL_MAX_DELAY);

    wake_up_procedure(); /* Wake up the device and safely resume active outputs [cite: 372, 377] */
}

/**
 * @brief  Sets the identical PWM duty cycle globally across all 16 channels simultaneously[cite: 678].
 * @note   Utilizes the dedicated ALL_LED registers to broadcast data to all outputs[cite: 677, 678].
 * Reduces I2C bus traffic down to a single 5-byte transaction instead of 16 individual calls[cite: 35, 678].
 * @param  duty_cycle: Desired global PWM duty cycle value in percent (0 to 100).
 * @retval None
 */
void canals_set_pwm_duty_cycle(uint8_t duty_cycle) {
    /* 1. Calculate the final 12-bit value (0 - 4095) [cite: 415, 683] */
    uint16_t calculated_steps = duty_cycle_calculation(duty_cycle);
    if (calculated_steps > PCA9685_LED_VALUE_MASK) {
        calculated_steps = PCA9685_LED_VALUE_MASK;
    }

    /* 2. Prepare the buffer: 1 byte for register address + 4 bytes for data */
    uint8_t data[5];
    
    /* Starting address is set to ALL_LED_ON_L broadcast register [cite: 679] */
    data[0] = PCA9685_REG_ALL_LED_ON_L;
    
    /* ALL_LED_ON count = 0 (Turn ON immediately at the start of the counter cycle) [cite: 680] */
    data[1] = 0x00; /* ALL_LED_ON_L [cite: 680] */
    data[2] = 0x00; /* ALL_LED_ON_H [cite: 680] */
    
    /* ALL_LED_OFF count = duty_cycle (Turn OFF when counter matches this value) [cite: 680] */
    data[3] = calculated_steps & 0xFF;         /* ALL_LED_OFF_L (lower 8 bits) [cite: 680] */
    data[4] = (calculated_steps >> 8) & 0x0F;  /* ALL_LED_OFF_H (upper 4 bits) [cite: 680] */

    /* 3. Send everything in one single I2C transaction */
    uint16_t dev_address = (PCA9685_I2C_BASE_ADDR << 1); 
    
    HAL_I2C_Master_Transmit(&hi2c1, dev_address, data, 5, HAL_MAX_DELAY);
}

/**
 * @brief  Initializes the PCA9685 hardware controller.
 * @note   Enables the Auto-Increment (AI) feature in the MODE1 register[cite: 350].
 * This is essential for multi-byte sequential register writes to work correctly[cite: 351, 352].
 * @retval None
 */
void pca9685_init(void) {
    uint8_t mode1 = 0x00;
    
    /* Set the AI (Auto-Increment) flag to active state [cite: 350] */
    mode1 |= PCA9685_MODE1_AI_BIT; 
    
    /* Write to the MODE1 register (0x00) [cite: 349] */
    HAL_I2C_Mem_Write(&hi2c1, (PCA9685_I2C_BASE_ADDR << 1), PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
}