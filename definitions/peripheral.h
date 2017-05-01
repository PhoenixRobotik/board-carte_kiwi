#pragma once
#ifndef __cplusplus
#   error "This header should be included from C++ code."
#endif

#include <stdint.h>

namespace libopencm3 {
    #include <libopencm3/cm3/nvic.h>
    #include <libopencm3/stm32/rcc.h>
    #include <libopencm3/stm32/gpio.h>
    #include <libopencm3/stm32/timer.h>
}


class Peripheral {
    // No copy, no assign. Only globals.
    Peripheral              (const Peripheral&) = delete;
    Peripheral& operator=   (const Peripheral&) = delete;
public:
    Peripheral(
        uint32_t _id,
        libopencm3::rcc_periph_clken _enable,
        libopencm3::rcc_periph_rst _reset)
    : valid(true)
    , Id(_id)
    , Enable(_enable)
    , Reset(_reset)
    { }

    bool valid;
    uint32_t                        Id;
    libopencm3::rcc_periph_clken    Enable;
    libopencm3::rcc_periph_rst      Reset;
    // uint8_t                         InterruptId;

    void enable() {
        rcc_periph_clock_enable(Enable);
    }

    void reset() {
        rcc_periph_reset_pulse  (Reset);
        rcc_periph_reset_hold   (Reset);
        rcc_periph_reset_release(Reset);
    }
};
