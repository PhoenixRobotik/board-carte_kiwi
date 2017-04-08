#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (   64'000'000)

// Interruptions = 10kHz = 100us (beaucoup ?)
#define SYSTICK_FREQ_HZ     (       10'000)
#define SYSTICK_PERIOD      (RCC_CLOCK_FREQ_HZ / SYSTICK_FREQ_HZ)

// Granularité des PWM = prescaler = 100kHz = 10us
#define PWM_GRANUL_FREQ_HZ  (      100'000)
#define PWM_GRANUL_PERIOD   (RCC_CLOCK_FREQ_HZ / PWM_GRANUL_FREQ_HZ)
// Fréquence des PWM = 250Hz = 4ms
#define PWM_FREQ_HZ         (          250)
#define PWM_PERIOD          (PWM_GRANUL_FREQ_HZ / PWM_FREQ_HZ)


#define MILLIS_TO_SYSTICK(ms) (ms * SYSTICK_FREQ_HZ /     1'000)
#define MICROS_TO_SYSTICK(us) (us * SYSTICK_FREQ_HZ / 1'000'000)



// Stm32f303k8 specific !
#define FLASH_BASE                  (0x08000000)
#define BASE_ADDRESS                (0x0800F000)
#define FLASH_PAGE_NUM_MAX          (16)
#define FLASH_PAGE_SIZE             (0x400)
#define FLASH_WRONG_DATA_WRITTEN    (0x80)
