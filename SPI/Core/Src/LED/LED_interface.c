#include "LED\LED_interface.h"
#include "main.h"
#include <math.h>
#include <stdlib.h> // Required for fabsf()

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


// --- STRUCTURES ---
typedef struct {
    float pitch; /**< Pitch angle (forward/backward) in degrees */
    float roll;  /**< Roll angle (left/right) in degrees */
} LIS3DSH_Angles_t;

// --- INITIALIZATION ---
HAL_StatusTypeDef LED_Init(TIM_HandleTypeDef *htim) {
    uint32_t desired_timer_clock = 1000000; 

    uint32_t prescaler_value = (CLOCK_FREQUENCY / desired_timer_clock) - 1;
    uint32_t arr_value = (desired_timer_clock / LED_PWM_FREQUENCY) - 1;

    htim->Init.Prescaler         = (uint16_t)prescaler_value;
    htim->Init.Period            = arr_value; // Period maps directly to the ARR register
    htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;

    if (HAL_TIM_PWM_Init(htim) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4) != HAL_OK) return HAL_ERROR;

    return HAL_OK;
}

// --- MANUAL DUTY CYCLE CONTROL (%) ---
HAL_StatusTypeDef LED_PWM_DutyCycle(TIM_HandleTypeDef *htim,LED_Color_t color, uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        duty_cycle = 100; // Cap the duty cycle at 100%
    }

    // Calculate pulse length based on htim4.Init.Period (ARR)
    uint32_t pulse_length = (htim->Init.Period + 1) * duty_cycle / 100;

    switch (color) {
        case LED_GREEN:
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pulse_length);
            break;
        case LED_ORANGE:
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, pulse_length);
            break;
        case LED_BLUE:
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, pulse_length);
            break;
        case LED_RED:
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, pulse_length);
            break;
        default:
            return HAL_ERROR;
    }

    return HAL_OK;
}

// --- TILT MATH ---
void CalculateAngles(int16_t ax, int16_t ay, int16_t az, LIS3DSH_Angles_t *angles) {
    // Calculate roll angle (left/right tilt)
    float roll_rad = atan2f((float)ay, (float)az);
    angles->roll = roll_rad * (180.0f / M_PI);

    // Calculate pitch angle (forward/backward tilt) using Y and Z hypotenuse for stability
    float denominator = sqrtf((float)ay * (float)ay + (float)az * (float)az);
    float pitch_rad = atan2f(-(float)ax, denominator);
    angles->pitch = pitch_rad * (180.0f / M_PI);
}

// --- UNIVERSAL RANGE MAPPING ---
uint32_t map_value(float x, float in_min, float in_max, uint32_t out_min, uint32_t out_max) {
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    return (uint32_t)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

// --- AUTOMATIC LED UPDATE BASED ON ACCELEROMETER RAW DATA ---
HAL_StatusTypeDef LED_Angles_Update(int16_t ax, int16_t ay, int16_t az,TIM_HandleTypeDef *htim) {
    LIS3DSH_Angles_t angles;
    
    // 1. Calculate current tilt angles in degrees (-90 to 90)
    CalculateAngles(ax, ay, az, &angles);
    
    uint32_t max_ccr = htim->Init.Period; // Maximum allowed CCR value (ARR value)

    // 2. Process PITCH axis (Forward / Backward)
    if (angles.pitch > 5.0f) {
        // Tilted forward -> Fade in ORANGE LED (Channel 2)
        uint32_t ccr_orange = map_value(angles.pitch, 0.0f, 90.0f, 0, max_ccr);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_orange);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, 0); // Turn off Blue LED
    } 
    else if (angles.pitch < -5.0f) {
        // Tilted backward -> Fade in BLUE LED (Channel 3)
        uint32_t ccr_blue = map_value(fabsf(angles.pitch), 0.0f, 90.0f, 0, max_ccr);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_blue);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0); // Turn off Orange LED
    } 
    else {
        // Balanced pitch -> Turn off both pitch LEDs (deadband zone)
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, 0);
    }

    // 3. Process ROLL axis (Left / Right)
    if (angles.roll > 5.0f) {
        // Tilted right -> Fade in RED LED (Channel 4)
        uint32_t ccr_red = map_value(angles.roll, 0.0f, 90.0f, 0, max_ccr);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_red);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, 0); // Turn off Green LED
    } 
    else if (angles.roll < -5.0f) {
        // Tilted left -> Fade in GREEN LED (Channel 1)
        uint32_t ccr_green = map_value(fabsf(angles.roll), 0.0f, 90.0f, 0, max_ccr);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_green);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, 0); // Turn off Red LED
    } 
    else {
        // Balanced roll -> Turn off both roll LEDs (deadband zone)
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, 0);
    }

    return HAL_OK;
}