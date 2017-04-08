#include "timers_cpp.h"

// Timer Timer1;
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
