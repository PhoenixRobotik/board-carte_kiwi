#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (   64'000'000)

// Interruptions = 10kHz = 100us (beaucoup ?)
#define SYSTICK_FREQ_HZ     (       100'000)
#define SYSTICK_PERIOD      (RCC_CLOCK_FREQ_HZ / SYSTICK_FREQ_HZ)

#define MILLIS_TO_SYSTICK(ms) (ms * SYSTICK_FREQ_HZ /     1'000)
#define MICROS_TO_SYSTICK(us) (us * SYSTICK_FREQ_HZ / 1'000'000)

// Granularité des PWM = prescaler = 100kHz = 10us
#define PWM_GRANUL_FREQ_HZ  (      100'000)
#define PWM_GRANUL_PERIOD   (RCC_CLOCK_FREQ_HZ / PWM_GRANUL_FREQ_HZ)
// Fréquence des PWM = 250Hz = 4ms
#define PWM_FREQ_HZ         (          400)
#define PWM_PERIOD          (PWM_GRANUL_FREQ_HZ / PWM_FREQ_HZ)

// 50kHz and a maximum period of (1<<16)-1 means a minimal detection
// speed of about 3mm/s and a resolution of 0.1mm with a wheel diameter
// of 75mm and 60 pulses per tour
// 64MHz/1280 = 50kHz
#define HALL_PRESCALER      (         1280)
#define HALL_COUNTER_PERIOD (    (1<<16)-1)
// APB1 and APB2 timer frequency are considered to be the same
#define APBx_TIMER_FREQ_HZ  (RCC_CLOCK_FREQ_HZ)
#define HALL_SAMPLE_FREQ_HZ (APBx_TIMER_FREQ_HZ / HALL_PRESCALER)

// Stm32f303k8 specific !
#define FLASH_BASE                  (0x08000000)
#define BASE_ADDRESS                (0x0800F000)
#define FLASH_PAGE_NUM_MAX          (16)
#define FLASH_PAGE_SIZE             (0x400)
#define FLASH_WRONG_DATA_WRITTEN    (0x80)
