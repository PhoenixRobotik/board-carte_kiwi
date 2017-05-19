#include "gpio.h"

void GPIO::set(bool on) {
    on  ? gpio_set  (pin.port->Id, pin.number)
        : gpio_clear(pin.port->Id, pin.number);
}
void GPIO::toggle() {
    gpio_toggle(pin.port->Id, pin.number);
}

void GPIO::init() {
    pin.port->enable();
    gpio_mode_setup(
        pin.port->Id,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        pin.number);
}


extern "C" {

    void GPIO_set    (GPIO* gpio, bool on) {   gpio->set(on);   }
    void GPIO_toggle (GPIO* gpio)          {   gpio->toggle();  }

}
