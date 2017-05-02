#include "timers_cpp.h"

// Others possible interrupts for Timer1 :
// NVIC_TIM1_BRK_TIM15_IRQ
// NVIC_TIM1_UP_TIM16_IRQ
// NVIC_TIM1_TRG_COM_TIM17_IRQ
Timer
Timer1(TIM1,
    libopencm3::RCC_TIM1,
    libopencm3::RST_TIM1,
    NVIC_TIM1_CC_IRQ),

Timer2(TIM2,
    libopencm3::RCC_TIM2,
    libopencm3::RST_TIM2,
    NVIC_TIM2_IRQ),

Timer3(TIM3,
    libopencm3::RCC_TIM3,
    libopencm3::RST_TIM3,
    NVIC_TIM3_IRQ),

Timer4(TIM4,
    libopencm3::RCC_TIM4,
    libopencm3::RST_TIM4,
    NVIC_TIM4_IRQ),

// Timer Timer5;
// Timer Timer6;

Timer7(TIM7,
    libopencm3::RCC_TIM7,
    libopencm3::RST_TIM7,
    NVIC_TIM7_IRQ);

// Timer Timer8;
// Timer Timer15;
// Timer Timer16;
// Timer Timer17;


void tim1_cc_isr(void)
{
    auto a = interruptSubscriptions.find(get_interrupt_flag(timer.Peripheral));
    if (a != interruptSubscriptions.end())
        a->second();


    hallsensor1.CC_interrupt_handler();
}

void tim2_isr(void)
{
    hallsensor2.CC_interrupt_handler();
}
