#pragma once

#include "peripheral.h"

// Minimal classes to encapsulate all the useful
// defines/values/addresses from libopencm3


class Port
: public Peripheral {
public:
    Port(
        uint32_t _id,
        libopencm3::rcc_periph_clken _enable,
        libopencm3::rcc_periph_rst _reset)
    : Peripheral(_id, _enable, _reset)
    { }


    // May be useful
    enum Number {
        pA = GPIOA,
        pB = GPIOB,
        pC = GPIOC,
        pD = GPIOD,
        pE = GPIOE,
        pF = GPIOF,
    };
};

extern Port
    PortA,
    PortB,
    PortC,
    PortD,
    // PortE,
    PortF;



class Pin {
public:
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

    Pin(Port& _port, Number _number)
    : port(&_port)
    , number(_number)
    { }

    Port*   port;
    Number  number;
};



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
