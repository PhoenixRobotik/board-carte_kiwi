#pragma once

#include "definitions/gpio_cpp.h"

#include  <libopencm3/stm32/exti.h>

class GPIO {
public:
    enum IOMode {
        input  = GPIO_MODE_INPUT,
        output = GPIO_MODE_OUTPUT,
    };

    enum PullMode {
        none     = GPIO_PUPD_NONE,
        pullup   = GPIO_PUPD_PULLUP,
        pulldown = GPIO_PUPD_PULLDOWN,
    };

    GPIO(Pin _pin,
         IOMode _io_mode,
         PullMode _pull_mode = none)
    : pin         (_pin)
    , m_exti      (_pin.number) //TODO: this is really dirty, change this
    , m_io_mode   (_io_mode)
    , m_pull_mode (_pull_mode)
    {
        init();
    }
    ~GPIO() { }

    void init();

    void set_mode() {};
    void set_pupd() {};

    // should return false if in INPUT mode ?
    void set(bool on);
    void setOn()    { set(true);  }
    void setOff()   { set(false); }
    void toggle();

    uint8_t read();

    bool enable_irq(bool on_rising, bool on_falling);
    bool disable_irq();
    void ACK_irq();


private:
    const Pin  pin;
    const uint32_t m_exti;
    IOMode m_io_mode;
    PullMode m_pull_mode;
};
