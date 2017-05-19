#pragma once

#include "definitions/gpio_cpp.h"

class GPIO {
public:
    GPIO(Pin _pin)
    : pin (_pin)
    {
        init();
    }
    ~GPIO() { }
    void init();

    void set(bool on);
    void setOn()    { set(true);  }
    void setOff()   { set(false); }
    void toggle();

private:
    const Pin  pin;
};
