#ifndef PCA9685_REGISTERS_H
#define PCA9685_REGISTERS_H

/**
 * @brief Default I2C addresses for the PCA9685 controller
 */
#define PCA9685_I2C_BASE_ADDR      0x40  // Base hardware address (depends on A0-A5 pins)
#define PCA9685_I2C_ALLCALLADDR    0xE0  // LED All Call I2C-bus address (active on start-up)
#define PCA9685_I2C_SUBADR1        0xE2  // Default I2C subaddress 1
#define PCA9685_I2C_SUBADR2        0xE4  // Default I2C subaddress 2
#define PCA9685_I2C_SUBADR3        0xE8  // Default I2C subaddress 3
#define PCA9685_I2C_SWRST          0x00  // General Call address used for software reset
#define PCA9685_SWRST_DATA         0x06  // Specific data byte required to trigger SWRST

/**
 * @brief Core configuration and address pointer registers
 */
#define PCA9685_REG_MODE1          0x00  // Mode register 1
#define PCA9685_REG_MODE2          0x01  // Mode register 2
#define PCA9685_REG_SUBADR1        0x02  // I2C-bus subaddress 1
#define PCA9685_REG_SUBADR2        0x03  // I2C-bus subaddress 2
#define PCA9685_REG_SUBADR3        0x04  // I2C-bus subaddress 3
#define PCA9685_REG_ALLCALLADR     0x05  // LED All Call I2C-bus address register

/**
 * @brief PWM Channel Control Registers (Channels 0 to 15)
 * Each of the 16 channels features 4 control bytes: ON_L, ON_H, OFF_L, OFF_H.
 * The register address offset between consecutive channels is exactly 4.
 */
#define PCA9685_CHANNEL_OFFSET     4

#define PCA9685_REG_LED0_ON_L      0x06  // LED0 output and brightness control byte 0
#define PCA9685_REG_LED0_ON_H      0x07  // LED0 output and brightness control byte 1
#define PCA9685_REG_LED0_OFF_L     0x08  // LED0 output and brightness control byte 2
#define PCA9685_REG_LED0_OFF_H     0x09  // LED0 output and brightness control byte 3

// Helper macros to calculate register addresses dynamically for any channel (0-15)
#define PCA9685_GET_REG_LED_ON_L(ch)  (PCA9685_REG_LED0_ON_L  + ((ch) * PCA9685_CHANNEL_OFFSET))
#define PCA9685_GET_REG_LED_ON_H(ch)  (PCA9685_REG_LED0_ON_H  + ((ch) * PCA9685_CHANNEL_OFFSET))
#define PCA9685_GET_REG_LED_OFF_L(ch) (PCA9685_REG_LED0_OFF_L + ((ch) * PCA9685_CHANNEL_OFFSET))
#define PCA9685_GET_REG_LED_OFF_H(ch) (PCA9685_REG_LED0_OFF_H + ((ch) * PCA9685_CHANNEL_OFFSET))

/**
 * @brief Global Control Registers (ALL LED) and Clock Prescaler
 */
#define PCA9685_REG_ALL_LED_ON_L   0xFA  // Load all LEDn_ON registers, byte 0
#define PCA9685_REG_ALL_LED_ON_H   0xFB  // Load all LEDn_ON registers, byte 1
#define PCA9685_REG_ALL_LED_OFF_L  0xFC  // Load all LEDn_OFF registers, byte 0
#define PCA9685_REG_ALL_LED_OFF_H  0xFD  // Load all LEDn_OFF registers, byte 1
#define PCA9685_REG_PRE_SCALE      0xFE  // Prescaler for output modulation frequency

/**
 * @brief Bitmasks for MODE1 register (Address 00h)
 */
#define PCA9685_MODE1_RESTART_BIT  (1 << 7)  // 1: Restart enabled, 0: Restart disabled
#define PCA9685_MODE1_EXTCLK_BIT   (1 << 6)  // 1: Use EXTCLK pin clock, 0: Use internal clock
#define PCA9685_MODE1_AI_BIT       (1 << 5)  // 1: Register Auto-Increment enabled, 0: Disabled
#define PCA9685_MODE1_SLEEP_BIT    (1 << 4)  // 1: Low power mode / Oscillator off, 0: Normal mode
#define PCA9685_MODE1_SUB1_BIT     (1 << 3)  // 1: PCA9685 responds to I2C-bus subaddress 1
#define PCA9685_MODE1_SUB2_BIT     (1 << 2)  // 1: PCA9685 responds to I2C-bus subaddress 2
#define PCA9685_MODE1_SUB3_BIT     (1 << 1)  // 1: PCA9685 responds to I2C-bus subaddress 3
#define PCA9685_MODE1_ALLCALL_BIT  (1 << 0)  // 1: PCA9685 responds to LED All Call I2C address

/**
 * @brief Bitmasks for MODE2 register (Address 01h)
 */
#define PCA9685_MODE2_INVRT_BIT    (1 << 4)  // 1: Output logic state inverted, 0: Not inverted (when OE = 0)
#define PCA9685_MODE2_OCH_BIT      (1 << 3)  // 1: Outputs change on ACK, 0: Change on STOP command
#define PCA9685_MODE2_OUTDRV_BIT   (1 << 2)  // 1: Totem pole output configuration, 0: Open-drain structure

// Output structure control definitions when OE pin is HIGH (output drivers not enabled)
#define PCA9685_MODE2_OUTNE_GND    (0x00 << 0) // When OE = 1, LEDn = 0
#define PCA9685_MODE2_OUTNE_ACTIVE (0x01 << 0) // When OE = 1, LEDn = 1 (if OUTDRV=1) or High-Z (if OUTDRV=0)
#define PCA9685_MODE2_OUTNE_HIGHZ  (0x02 << 0) // When OE = 1, LEDn = high-impedance
#define PCA9685_MODE2_OUTNE_MASK   (0x03 << 0)

/**
 * @brief Special bitmasks and constants for LEDn_ON_H and LEDn_OFF_H registers
 */
#define PCA9685_LED_FULL_ON        (1 << 4)  // Set in LEDn_ON_H to force the channel permanently ON
#define PCA9685_LED_FULL_OFF       (1 << 4)  // Set in LEDn_OFF_H to force the channel permanently OFF
#define PCA9685_LED_VALUE_MASK     0x0FFF    // Bitmask extracting the 12-bit PWM counter value (0 - 4095)

/**
 * @brief Hardware timing and scaling constants
 */
#define PCA9685_INTERNAL_OSC_HZ    25000000UL // Typical frequency of the internal oscillator (25 MHz)
#define PCA9685_MIN_PRESCALE       3          // Minimum value allowed in the PRE_SCALE register
#define PCA9685_MAX_PRESCALE       255        // Maximum structural value of the PRE_SCALE register
#define PCA9685_COUNTER_STEPS      4096       // Full resolution steps of the internal PWM counter (12-bit)

#endif /* PCA9685_REGISTERS_H */