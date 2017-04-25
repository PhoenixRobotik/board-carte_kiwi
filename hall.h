#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"

#include <stdint.h>
#include <libopencm3/stm32/timer.h>

#ifdef __cplusplus

class Hall
{
public:
    Hall(Port::Number _portH1, Pin::Number _pinH1,
        Port::Number _portH2, Pin::Number _pinH2,
        Port::Number _portH3, Pin::Number _pinH3,
        Timer const& _timer,
        AltFunction::Number _altFunction)
    : portH1(_portH1)
    , pinH1(_pinH1)
    , portH2(_portH2)
    , pinH2(_pinH2)
    , portH3(_portH3)
    , pinH3(_pinH3)
    , timer(_timer)
    , altFunction(_altFunction)
    {
        init();
        enable(); // But with dutycycle = 0
    }

    ~Hall() {
        deinit();
    }

    void init();
    void deinit() { }

    void enable();
    void disable();

private:
    const Port::Number portH1;
    const Pin::Number  pinH1;
    const Port::Number portH2;
    const Pin::Number  pinH2;
    const Port::Number portH3;
    const Pin::Number  pinH3;

    const Timer& timer;
    const AltFunction::Number altFunction;

    bool enabled = false;
};

// Sensors
extern Hall hallsensor1;
extern Hall hallsensor2;

extern uint32_t rotation_speed_sensor1;
extern uint32_t rotation_speed_sensor2;

extern "C" {
#endif



#ifdef __cplusplus
}
#endif
