#include "leds.h"

void Led::set(bool on) {
    on  ? gpio_set  (pin.port->Id, pin.number)
        : gpio_clear(pin.port->Id, pin.number);
}
void Led::toggle() {
    gpio_toggle(pin.port->Id, pin.number);
}

void Led::init() {
    pin.port->enable();
    gpio_mode_setup(
        pin.port->Id,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        pin.number);
}


extern "C" {

    void led_set    (Led* led, bool on) {   led->set(on);   }
    void led_toggle (Led* led)          {   led->toggle();  }

}
