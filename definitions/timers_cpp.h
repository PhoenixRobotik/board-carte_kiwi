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
        _0   = GPIO_AF0,
        _1   = GPIO_AF1,
        _2   = GPIO_AF2,
        _3   = GPIO_AF3,
        _4   = GPIO_AF4,
        _5   = GPIO_AF5,
        _6   = GPIO_AF6,
        _7   = GPIO_AF7,
        _8   = GPIO_AF8,
        _9   = GPIO_AF9,
        _10  = GPIO_AF10,
        _11  = GPIO_AF11,
        _12  = GPIO_AF12,
        _13  = GPIO_AF13,
        _14  = GPIO_AF14,
        _15  = GPIO_AF15,
    };
};
