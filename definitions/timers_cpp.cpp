#include "timers_cpp.h"

// Others possible interrupts for Timer1 :
// NVIC_TIM1_BRK_TIM15_IRQ
// NVIC_TIM1_UP_TIM16_IRQ
// NVIC_TIM1_TRG_COM_TIM17_IRQ
// NVIC_TIM1_CC_IRQ
Timer
Timer1(TIM1,
    libopencm3::RCC_TIM1,
    libopencm3::RST_TIM1),

Timer2(TIM2,
    libopencm3::RCC_TIM2,
    libopencm3::RST_TIM2),

Timer3(TIM3,
    libopencm3::RCC_TIM3,
    libopencm3::RST_TIM3),

Timer4(TIM4,
    libopencm3::RCC_TIM4,
    libopencm3::RST_TIM4),

// Timer Timer5;
// Timer Timer6;

Timer7(TIM7,
    libopencm3::RCC_TIM7,
    libopencm3::RST_TIM7);

// Timer Timer8;
// Timer Timer15;
// Timer Timer16;
// Timer Timer17;
