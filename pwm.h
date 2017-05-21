#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"

#include <stdint.h>

#include <libopencm3/stm32/timer.h>

class PWM
{
public:
    PWM(Peripheral* timer, tim_oc_id channel,
        Pin outPin, AltFunction::Number altFunction,
        uint32_t prescaler = PWM_GRANUL_PERIOD, 
        uint32_t period = PWM_PERIOD)
    : m_timer(timer)
    , m_channel(channel)
    , m_pin(outPin)
    , m_out_af(altFunction)
    {
        init(prescaler, period);
        enable(); // But with dutycycle = 0
    }

    ~PWM() {
        deinit();
    }

    void init(uint32_t prescaler, uint32_t period);
    void deinit() { }

    void enable();
    void disable();

    void setDuty    (uint32_t _duty);

    // More high level sets
    void setPercent (int _percent);
    void setMillisec(int _millisec);
    void setMicrosec(uint64_t _microsec);
    // void setPeriod  (uint32_t _period) { }


    // Getters (conversion between different sets)
    uint32_t getPeriod();
    uint32_t getPrescale();
    uint32_t getDuty();

private:
    Peripheral* const m_timer;
    tim_oc_id const m_channel;

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
