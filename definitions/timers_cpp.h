#pragma once

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>


struct Timer {
    bool valid;
    uint32_t            Peripheral;
    rcc_periph_rst      Reset;
    rcc_periph_clken    ClockEnable;
    uint8_t             InterruptId;
};

// Timer1 is special and not really handled here… I think
// extern Timer Timer1;
extern Timer Timer2;
extern Timer Timer3;
extern Timer Timer4;
// extern Timer Timer5;
// extern Timer Timer6;
extern Timer Timer7;
// extern Timer Timer8;
// extern Timer Timer15;
// extern Timer Timer16;
// extern Timer Timer17;

// GPIO-specific, not Timer-specific
struct AltFunction {
    enum Number {
        f0   = GPIO_AF0,
        f1   = GPIO_AF1,
        f2   = GPIO_AF2,
        f3   = GPIO_AF3,
        f4   = GPIO_AF4,
        f5   = GPIO_AF5,
        f6   = GPIO_AF6,
        f7   = GPIO_AF7,
        f8   = GPIO_AF8,
        f9   = GPIO_AF9,
        f10  = GPIO_AF10,
        f11  = GPIO_AF11,
        f12  = GPIO_AF12,
        f13  = GPIO_AF13,
        f14  = GPIO_AF14,
        f15  = GPIO_AF15,
    };
};
