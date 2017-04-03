#pragma once

#include <libopencm3/stm32/gpio.h>

struct Port {
    enum Number {
        _A = GPIOA,
        _B = GPIOB,
        _C = GPIOC,
        _D = GPIOD,
        _E = GPIOE,
        _F = GPIOF,
    };
};

struct Pin {
    enum Number {
        _0   = GPIO0,
        _1   = GPIO1,
        _2   = GPIO2,
        _3   = GPIO3,
        _4   = GPIO4,
        _5   = GPIO5,
        _6   = GPIO6,
        _7   = GPIO7,
        _8   = GPIO8,
        _9   = GPIO9,
        _10  = GPIO10,
        _11  = GPIO11,
        _12  = GPIO12,
        _13  = GPIO13,
        _14  = GPIO14,
        _15  = GPIO15,
    };
};
