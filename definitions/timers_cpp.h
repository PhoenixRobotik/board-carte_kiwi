#pragma once

#include "peripheral.h"

class Timer
: public Peripheral {
public:
    Timer(
        uint32_t _id,
        rcc_periph_clken _enable,
        rcc_periph_rst _reset)
    : Peripheral(_id, _enable, _reset)
    { }



};


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
