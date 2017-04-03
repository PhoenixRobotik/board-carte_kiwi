#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (64000000)
#define SYSTICK_FREQ_HZ     (1000)
#define SYSTICK_PERIOD      (RCC_CLOCK_FREQ_HZ/SYSTICK_FREQ_HZ)

#define MILLIS_TO_SYSTICK(ms) (ms * SYSTICK_FREQ_HZ / 1000)
