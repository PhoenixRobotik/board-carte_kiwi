#pragma once

#include <cstdint>

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
static const uint32_t RCC_CLOCK_FREQ_HZ (   64'000'000);

// Interruptions = 10kHz = 100us (beaucoup ?)
static const uint32_t SYSTICK_FREQ_HZ   (       100'000);
static const uint32_t SYSTICK_PERIOD    (RCC_CLOCK_FREQ_HZ / SYSTICK_FREQ_HZ);
static_assert(SYSTICK_PERIOD == 640);

inline constexpr uint32_t MILLIS_TO_SYSTICK(uint32_t ms) {
    return ms * SYSTICK_FREQ_HZ / 1'000;
}
inline constexpr uint32_t MICROS_TO_SYSTICK(uint32_t us) {
    return us * SYSTICK_FREQ_HZ / 1'000'000;
}
inline constexpr uint32_t SYSTICK_TO_MICROS(uint32_t tick) {
    return tick * 1'000'000 / SYSTICK_FREQ_HZ;
}
inline constexpr uint32_t SYSTICK_TO_MILLIS(uint32_t tick) {
    return tick * 1'000 / SYSTICK_FREQ_HZ;
}

// Granularité des PWM = prescaler = 100kHz = 10us
static const uint32_t PWM_GRANUL_FREQ_HZ(      100'000);
static const uint32_t PWM_GRANUL_PERIOD (RCC_CLOCK_FREQ_HZ / PWM_GRANUL_FREQ_HZ);
static_assert(PWM_GRANUL_PERIOD == 640);
// Fréquence des PWM = 50Hz = 20ms
static const uint32_t PWM_FREQ_HZ       (           50);
static const uint32_t PWM_PERIOD        (PWM_GRANUL_FREQ_HZ / PWM_FREQ_HZ);
static_assert(PWM_PERIOD == 2'000);

// 50kHz and a maximum period of (1<<16)-1 means a minimal detection
// speed of about 3mm/s and a resolution of 0.1mm with a wheel diameter
// of 75mm and 60 pulses per tour
// 64MHz/1280 = 50kHz
static const uint32_t HALL_PRESCALER    (         1280);
static const uint32_t HALL_COUNTER_PERIOD (  (1<<16)-1);
// APB1 and APB2 timer frequency are considered to be the same
static const uint32_t APBx_TIMER_FREQ_HZ(RCC_CLOCK_FREQ_HZ);
static const uint32_t HALL_SAMPLE_FREQ_HZ (APBx_TIMER_FREQ_HZ / HALL_PRESCALER);

// Stm32f303k8 specific !
static const uint32_t FLASH_BASE                (0x08000000);
static const uint32_t BASE_ADDRESS              (0x0800F000);
static const uint32_t FLASH_PAGE_NUM_MAX        (16);
static const uint32_t FLASH_PAGE_SIZE           (0x400);
static const uint32_t FLASH_WRONG_DATA_WRITTEN  (0x80);
