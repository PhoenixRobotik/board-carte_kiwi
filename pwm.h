#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"

#include <stdint.h>

namespace libopencm3 {
    #include <libopencm3/stm32/timer.h>
}

class PWM
{
public:
    PWM(Peripheral* timer, libopencm3::tim_oc_id channel,
        Pin outPin, AltFunction::Number altFunction)
    : m_timer(timer)
    , m_channel(channel)
    , m_pin(outPin)
    , m_out_af(altFunction)
    {
        init();
        enable(); // But with dutycycle = 0
    }

    ~PWM() {
        deinit();
    }

    void init();
    void deinit() { }

    void enable();
    void disable();

    void setDuty    (uint32_t _duty);//   { duty = _duty; }

    // More high level sets
    void setPercent (int _percent);
    void setMillisec(int _millisec);
    void setMicrosec(uint64_t _microsec);
    // void setPeriod  (uint32_t _period) { }


    // Getters (conversion between different sets)
    uint32_t getPeriod();
    uint32_t getPrescale();
    uint32_t getDuty();
    int setPercent ();
    int setMillisec();

private:
    Peripheral* const m_timer;
    libopencm3::tim_oc_id const m_channel;

    Pin const m_pin;
    AltFunction::Number const m_out_af;


    bool enabled = false;
    uint32_t duty = 0;
};


// extern "C" {
// #endif

// struct PWM;
// void pwm_set_duty   (PWM* pwm, uint32_t duty);
// void pwm_set_ms     (PWM* pwm, int _millisec);
// void pwm_set_percent(PWM* pwm, int _percent);

// #ifdef __cplusplus
// }
// #endif
