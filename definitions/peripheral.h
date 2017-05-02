#pragma once
#ifndef __cplusplus
#   error "This header should be included from C++ code."
#endif

#include <stdint.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>


class Peripheral {
    // No copy, no assign. Only globals.
    Peripheral              (const Peripheral&) = delete;
    Peripheral& operator=   (const Peripheral&) = delete;
public:
    Peripheral(
        uint32_t _id,
        rcc_periph_clken _enable,
        rcc_periph_rst _reset)
    : valid(true)
    , Id(_id)
    , Enable(_enable)
    , Reset(_reset)
    { }

    bool valid;
    uint32_t            Id;
    rcc_periph_clken    Enable;
    rcc_periph_rst      Reset;

    void enable() {
        rcc_periph_clock_enable(Enable);
    }

    void reset() {
        rcc_periph_reset_pulse  (Reset);
        rcc_periph_reset_hold   (Reset);
        rcc_periph_reset_release(Reset);
    }
};
extern Peripheral
    // periphCAN1,
    // periphCAN2,
    periphCAN;
