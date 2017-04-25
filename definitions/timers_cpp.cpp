#include "timers_cpp.h"

// Others possible interrupts for Timer1 :
// NVIC_TIM1_BRK_TIM15_IRQ
// NVIC_TIM1_UP_TIM16_IRQ
// NVIC_TIM1_TRG_COM_TIM17_IRQ
Timer Timer1({ true, TIM1, RST_TIM1, RCC_TIM1, NVIC_TIM1_CC_IRQ});
Timer Timer2({ true, TIM2, RST_TIM2, RCC_TIM2, NVIC_TIM2_IRQ });
Timer Timer3({ true, TIM3, RST_TIM3, RCC_TIM3, NVIC_TIM3_IRQ });
Timer Timer4({ true, TIM4, RST_TIM4, RCC_TIM4, NVIC_TIM4_IRQ });
// Timer Timer5;
// Timer Timer6;
Timer Timer7({ true, TIM7, RST_TIM7, RCC_TIM7, NVIC_TIM7_IRQ });
// Timer Timer8;
// Timer Timer15;
// Timer Timer16;
// Timer Timer17;
