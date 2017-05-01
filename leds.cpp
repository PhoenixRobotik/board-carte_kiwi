#include "leds.h"

// #include <libopencm3/stm32/gpio.h>

void Led::set(bool on) {
    on  ? libopencm3::gpio_set  (pin.port->Id, pin.number)
        : libopencm3::gpio_clear(pin.port->Id, pin.number);
}
void Led::toggle() {
    libopencm3::gpio_toggle(pin.port->Id, pin.number);
}

void Led::init() {
    pin.port->enable();
    libopencm3::gpio_mode_setup(
        pin.port->Id,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        pin.number);
}


extern "C" {

    void led_set    (Led* led, bool on) {   led->set(on);   }
    void led_toggle (Led* led)          {   led->toggle();  }

}
