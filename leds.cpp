#include "leds.h"

#include <libopencm3/stm32/gpio.h>

void Led::set(bool on) {
    on  ? gpio_set  (port, pin)
        : gpio_clear(port, pin);
}
void Led::toggle() {
    gpio_toggle(port, pin);
}

void Led::init() {
    rcc_periph_clock_enable(RCC_GPIOF); // TODO F-specific
    gpio_mode_setup(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
}


extern "C" {

    void led_set    (Led* led, bool on) {   led->set(on);   }
    void led_toggle (Led* led)          {   led->toggle();  }

}
