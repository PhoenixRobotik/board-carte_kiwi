#pragma once

#include <libopencm3/stm32/gpio.h>

struct Port {
    enum Number {
        pA = GPIOA,
        pB = GPIOB,
        pC = GPIOC,
        pD = GPIOD,
        pE = GPIOE,
        pF = GPIOF,
    };
};

struct Pin {
    enum Number {
        p0   = GPIO0,
        p1   = GPIO1,
        p2   = GPIO2,
        p3   = GPIO3,
        p4   = GPIO4,
        p5   = GPIO5,
        p6   = GPIO6,
        p7   = GPIO7,
        p8   = GPIO8,
        p9   = GPIO9,
        p10  = GPIO10,
        p11  = GPIO11,
        p12  = GPIO12,
        p13  = GPIO13,
        p14  = GPIO14,
        p15  = GPIO15,
    };
};
