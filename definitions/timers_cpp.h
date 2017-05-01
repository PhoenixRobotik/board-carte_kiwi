#pragma once

#include "peripheral.h"

class Timer
: public Peripheral {
public:
    Timer(
        uint32_t _id,
        libopencm3::rcc_periph_clken _enable,
        libopencm3::rcc_periph_rst _reset)
    : Peripheral(_id, _enable, _reset)
    { }



};

// Timer1 is special and not really handled here… I think
extern Timer
    Timer1,
    Timer2,
    Timer3,
    Timer4,
    // Timer5,
    // Timer6,
    Timer7;
    // Timer8;
    // Timer15;
    // Timer16;
    // Timer17;
