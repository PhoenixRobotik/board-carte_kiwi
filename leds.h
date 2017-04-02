#pragma once

#include <libopencm3/stm32/gpio.h>

typedef uint32_t Port;
typedef uint16_t Pin;

#ifdef __cplusplus

class Led {
public:
    Led(Port _port, Pin _pin)
    : port(_port)
    , pin (_pin)
    { }

    ~Led() { }

    void set(bool on) {
        on  ? gpio_set  (port, pin)
            : gpio_clear(port, pin);
    }
    void toggle() {
        gpio_toggle(port, pin);
    }

    void init() {
        gpio_mode_setup(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    }

    Port port;
    Pin  pin;
};

extern Led activeLed;
extern Led statusLed;


extern "C" {
#endif

void init_leds();


void led_active_set(bool on);
void led_active_toggle();

void led_status_set(bool on);
void led_status_toggle();

#ifdef __cplusplus
}

#endif
