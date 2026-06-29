#include "PCA9685/Interface.h"
#include "PCA9685/Registers.h"
#include "main.h"

/* External hardware handles from CubeMX */
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;

/**
 * @brief  Calculates the 12-bit counter value with inverted logic for active-low LEDs.
 * @note   Internal helper function utilizing fast integer math.
 * @param  duty_cycle: Target duty cycle value in percent (0 to 100).
 * @retval 12-bit numerical step representation for the register (0 to 4095).
 */
static uint16_t duty_cycle_calculation(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        duty_cycle = 100;
    }
    
    /* Invert the user percentage because LEDs are hardware-connected to VCC (active-low) */
    uint8_t inverted_duty = 100 - duty_cycle;
    
    return (uint16_t)((inverted_duty * PCA9685_LED_VALUE_MASK) / 100);
}

/**
 * @brief  Sets the PWM duty cycle for a specific channel with hardware glitch protection.
 * @note   Handles extreme boundaries (0% and 100%) using dedicated hardware full-on/off overrides.
 * @param  canal: Target PCA9685 channel index (0 to 15).
 * @param  duty_cycle: Desired PWM duty cycle value in percent (0 to 100).
 * @retval None
 */
void canal_set_pwm_duty_cycle(uint8_t canal, uint8_t duty_cycle) {
    uint8_t data[5];
    data[0] = PCA9685_GET_REG_LED_ON_L(canal);
    
    /* 1. Handle HARDWARE 100% state (Active-Low logic -> Force continuous GND via FULL_OFF) */
    if (duty_cycle == 100) {
        data[1] = 0x00; /* LEDn_ON_L */
        data[2] = 0x00; /* LEDn_ON_H */
        data[3] = 0x00; /* LEDn_OFF_L */
        data[4] = 0x10; /* LEDn_OFF_H (Sets the FULL_OFF bit to force permanent GND) */
    } 
    /* 2. Handle HARDWARE 0% state (Active-Low logic -> Force continuous VCC via FULL_ON) */
    else if (duty_cycle == 0) {
        data[1] = 0x00; /* LEDn_ON_L */
        data[2] = 0x10; /* LEDn_ON_H (Sets the FULL_ON bit to force permanent VCC) */
        data[3] = 0x00; /* LEDn_OFF_L */
        data[4] = 0x00; /* LEDn_OFF_H */
    } 
    /* 3. Handle standard PWM modulation (1% to 99%) */
    else {
        uint16_t calculated_steps = duty_cycle_calculation(duty_cycle);
        
        data[1] = 0x00; /* LEDn_ON_L */
        data[2] = 0x00; /* LEDn_ON_H */
        data[3] = calculated_steps & 0xFF;        /* LEDn_OFF_L (lower 8 bits) */
        data[4] = (calculated_steps >> 8) & 0x07; /* LEDn_OFF_H (upper 3 bits inside secure 0x07 mask) */
    }

    /* Send the 5-byte transaction over I2C */
    uint16_t dev_address = (PCA9685_I2C_BASE_ADDR << 1); 
    HAL_I2C_Master_Transmit(&hi2c1, dev_address, data, 5, HAL_MAX_DELAY);
}

/**
 * @brief  Wakes up the PCA9685 device from low-power sleep mode.
 * @note   This procedure reads the register state first to prevent overwriting other configuration flags,
 * implements a mandatory polling delay using TIM2 for oscillator stabilization,
 * and utilizes the RESTART bit to resume active channels without losing parameters.
 * @retval None
 */
void wake_up_procedure(void) {
    uint8_t mode1 = 0x00;
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Read the current state of MODE1 to preserve other flags (like Auto-Increment) */
    HAL_I2C_Mem_Read(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 2. Clear the SLEEP bit to wake up the internal oscillator */
    mode1 &= ~PCA9685_MODE1_SLEEP_BIT; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 3. Mandatory hardware delay for oscillator stabilization (min. 500 microseconds) */
    __HAL_TIM_SET_COUNTER(&htim2, 0);              /* Reset counter to absolute 0 */
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);  /* Clear any old overflow/update flags */
    
    HAL_TIM_Base_Start(&htim2);                    /* Start the timer (it will count up to 600 and stop) */
    
    /* Elegant blocking poll - wait until the timer finishes its 600us single run */
    while (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) == RESET) {
        /* Just wait for the hardware flag to switch to SET */
    }
    
    HAL_TIM_Base_Stop(&htim2);                     /* Stop the timer cleanly */

    /* 4. Set the RESTART bit to resume previous PWM channels synchronously */
    mode1 |= PCA9685_MODE1_RESTART_BIT; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
}

/**
 * @brief  Puts the PCA9685 device into low-power sleep mode.
 * @note   Preserves existing bit configurations while explicitly shutting down the internal oscillator.
 * Clears the RESTART bit before entering sleep as requested by the specifications.
 * @retval None
 */
void sleep_procedure(void) {
    uint8_t mode1 = 0x00;
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Read the current state of MODE1 to preserve other flags (like Auto-Increment) */
    HAL_I2C_Mem_Read(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 2. Set the SLEEP bit to 1, and CLEAR the RESTART bit (required by datasheet when entering sleep) */
    mode1 = (mode1 & ~PCA9685_MODE1_RESTART_BIT) | PCA9685_MODE1_SLEEP_BIT;

    /* 3. Write the updated byte back to the device */
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
}

/**
 * @brief  Changes the global PWM output modulation frequency for all channels.
 * @note   The device must sleep to unlock the PRE_SCALE register. This implementation 
 * uses integer-only arithmetic with a rounding trick to preserve accuracy without floats.
 * @param  frequency: Target output modulation frequency in Hz (typically 40 Hz to 1000 Hz).
 * @retval None
 */
void canals_set_pwm_frequency(uint16_t frequency) {
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);

    /* 1. Calculate the denominator (4096 * Frequency) */
    uint32_t denominator = PCA9685_COUNTER_STEPS * frequency;

    /* 2. Apply the integer rounding trick: (Numerator + Denominator / 2) / Denominator */
    /* Internal oscillator clock is exactly 25 000 000 Hz */
    uint32_t prescale_exact = (PCA9685_INTERNAL_OSC_HZ + (denominator / 2)) / denominator;
    
    /* 3. Subtract 1 as required by the datasheet formula */
    int32_t prescale_int = (int32_t)prescale_exact - 1;

    /* 4. Hardware constraints check (PCA9685 allows values between 3 and 255) */
    if (prescale_int < PCA9685_MIN_PRESCALE) {
        prescale_int = PCA9685_MIN_PRESCALE;
    } else if (prescale_int > PCA9685_MAX_PRESCALE) {
        prescale_int = PCA9685_MAX_PRESCALE;
    }

    uint8_t prescale_reg_value = (uint8_t)prescale_int;

    /* 5. Execute the hardware update sequence */
    sleep_procedure(); /* Put the device into sleep mode (unlocks PRE_SCALE register) */

    /* Write to the PRE_SCALE register (0xFE) */
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_PRE_SCALE, I2C_MEMADD_SIZE_8BIT, &prescale_reg_value, 1, HAL_MAX_DELAY);

    wake_up_procedure(); /* Wake up the device and safely resume active outputs */
}

/**
 * @brief  Sets the identical PWM duty cycle globally across all 16 channels simultaneously.
 * @note   Utilizes the dedicated ALL_LED registers to broadcast data to all outputs.
 * Reduces I2C bus traffic down to a single 5-byte transaction instead of 16 individual calls.
 * @param  duty_cycle: Desired global PWM duty cycle value in percent (0 to 100).
 * @retval None
 */
void canals_set_pwm_duty_cycle(uint8_t duty_cycle) {
    uint8_t data[5];
    
    /* Starting address is set to ALL_LED_ON_L broadcast register */
    data[0] = PCA9685_REG_ALL_LED_ON_L;
    
    /* 1. Global Hardware 100% ON (Active-Low logic -> Force continuous GND via FULL_OFF) */
    if (duty_cycle == 100) {
        data[1] = 0x00; /* ALL_LED_ON_L */
        data[2] = 0x00; /* ALL_LED_ON_H */
        data[3] = 0x00; /* ALL_LED_OFF_L */
        data[4] = 0x10; /* ALL_LED_OFF_H = FULL_OFF */
    } 
    /* 2. Global Hardware 0% OFF (Active-Low logic -> Force continuous VCC via FULL_ON) */
    else if (duty_cycle == 0) {
        data[1] = 0x00; /* ALL_LED_ON_L */
        data[2] = 0x10; /* ALL_LED_ON_H = FULL_ON */
        data[3] = 0x00; /* ALL_LED_OFF_L */
        data[4] = 0x00; /* ALL_LED_OFF_H */
    } 
    /* 3. Global standard PWM modulation (1% to 99%) */
    else {
        uint16_t calculated_steps = duty_cycle_calculation(duty_cycle);
        
        data[1] = 0x00; /* ALL_LED_ON_L */
        data[2] = 0x00; /* ALL_LED_ON_H */
        data[3] = calculated_steps & 0xFF;        /* ALL_LED_OFF_L */
        data[4] = (calculated_steps >> 8) & 0x07; /* FIXED: Changed from 0x0F to secure 0x07 mask */
    }

    /* Send everything in one single I2C transaction */
    uint16_t dev_address = (PCA9685_I2C_BASE_ADDR << 1); 
    HAL_I2C_Master_Transmit(&hi2c1, dev_address, data, 5, HAL_MAX_DELAY);
}

/**
 * @brief  Initializes the PCA9685 hardware controller.
 * @note   Enables the Auto-Increment (AI) feature in MODE1 and configures Totem-Pole (Push-Pull) outputs in MODE2.
 * @retval None
 */
void pca9685_init(void){
    uint16_t dev_addr = (PCA9685_I2C_BASE_ADDR << 1);
    uint8_t mode1 = 0x00;
    uint8_t mode2 = 0x00;
    
    /* 1. Read current state, inject Auto-Increment flag, and update MODE1 register */
    HAL_I2C_Mem_Read(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);
    mode1 |= PCA9685_MODE1_AI_BIT; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE1, I2C_MEMADD_SIZE_8BIT, &mode1, 1, HAL_MAX_DELAY);

    /* 2. Force hardware outputs out of Open-Drain into Totem-Pole (Push-Pull) mode */
    /* Value 0x04 sets bit 2 (OUTDRV) to 1. Essential for direct connection to P3 header pins */
    mode2 = 0x04; 
    HAL_I2C_Mem_Write(&hi2c1, dev_addr, PCA9685_REG_MODE2, I2C_MEMADD_SIZE_8BIT, &mode2, 1, HAL_MAX_DELAY);

    /* 3. CRITICAL: Explicitly wake up the device so the internal clock starts ticking! */
    wake_up_procedure();
}