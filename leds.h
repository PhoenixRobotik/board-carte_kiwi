#pragma once

#include "definitions/gpio_cpp.h"

#ifdef __cplusplus

class Led {
public:
    Led(Pin _pin)
    : pin (_pin)
    {
        init();
    }
    ~Led() { }
    void init();

    void set(bool on);
    void setOn()    { set(true);  }
    void setOff()   { set(false); }
    void toggle();

private:
    const Pin  pin;
};

extern "C" {
#endif

struct Led;
void led_set    (Led* led, bool on);
void led_toggle (Led* led);

#ifdef __cplusplus
}
#endif
