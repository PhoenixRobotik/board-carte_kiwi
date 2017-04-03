#pragma once

#include <libopencm3/stm32/rcc.h>
#include "gpio_cpp.h"

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

    void set(bool on) {
        on  ? gpio_set  (port, pin)
            : gpio_clear(port, pin);
    }
    void toggle() {
        gpio_toggle(port, pin);
    }

    void init() {
        rcc_periph_clock_enable(RCC_GPIOF);
        gpio_mode_setup(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    }

    Port::Number port;
    Pin::Number  pin;
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
