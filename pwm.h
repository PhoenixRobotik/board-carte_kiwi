#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"

#include <stdint.h>
#include <libopencm3/stm32/timer.h>

#ifdef __cplusplus

class PWM
{
public:
    PWM(Port::Number _port, Pin::Number _pin,
        Timer const& _timer,
        AltFunction::Number _altFunction, tim_oc_id _channel)
    : port(_port)
    , pin(_pin)
    , timer(_timer)
    , altFunction(_altFunction)
    , channel(_channel)
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

    void setPeriod  (uint32_t _period) { }
    void setDuty    (uint32_t _duty);//   { duty = _duty; }

    // More high level sets
    void setPercent (int _percent) { }
    void setMillisec(int _millisec){ }


    // Getters (conversion between different sets)
    uint32_t getPeriod();
    uint32_t getPrescale();
    uint32_t getDuty();
    int setPercent ();
    int setMillisec();

private:
    const Port::Number port;
    const Pin::Number  pin;

    const Timer& timer;
    const AltFunction::Number altFunction;
    const tim_oc_id channel;

    bool enabled = false;
    uint32_t duty = 0;
};

// Moteurs
extern PWM moteur1;
extern PWM moteur2;
// Led PWM
// extern PWM ledActFloat;

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
