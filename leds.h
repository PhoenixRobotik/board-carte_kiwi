#pragma once

#include "definitions/gpio_cpp.h"

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
