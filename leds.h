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

extern "C" {
#endif

struct Led;
void led_set    (Led* led, bool on);
void led_toggle (Led* led);

#ifdef __cplusplus
}
#endif
