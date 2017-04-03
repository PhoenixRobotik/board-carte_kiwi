#pragma once

#include <stdint.h>
#include "gpio_cpp.h"

#ifdef __cplusplus

class PWM
{
public:
    PWM(Port::Number _port, Pin::Number _pin)
    : port(_port)
    , pin(_pin)
    { }
    ~PWM() {
        deinit();
    }

    void init();
    void deinit();

    void enable();
    void disable();

    void setPeriod  (uint32_t period);
    void setPrescale(uint32_t prescale);
    void setDuty    (uint32_t duty);

    // More high level sets
    void setPercent (int percent);
    void setMillisec(int millisec);


    // Getters (conversion between different sets)
    uint32_t getPeriod();
    uint32_t getPrescale();
    uint32_t getDuty();
    int setPercent ();
    int setMillisec();

private:
    Port::Number port;
    Pin::Number  pin;

    uint32_t duty;
};




extern "C" {
#endif

#ifdef __cplusplus
}
#endif
