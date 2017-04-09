#pragma once

#include <libopencm3/stm32/rcc.h>
#include "definitions/gpio_cpp.h"

#ifdef __cplusplus

class Led {
public:
    Led(Port::Number _port, Pin::Number _pin)
    : port(_port)
    , pin (_pin)
    {
        init();
    }
    ~Led() { }
    void init();

    void set(bool on);
    void toggle();

private:
    const Port::Number port;
    const Pin::Number  pin;
};

extern Led activeLed;
extern Led statusLed;


extern "C" {
#endif

void led_active_set(bool on);
void led_active_toggle();

void led_status_set(bool on);
void led_status_toggle();

#ifdef __cplusplus
}
#endif
