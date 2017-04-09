#include "leds.h"

#include <libopencm3/stm32/gpio.h>

Port::Number
    LedsPort = Port::_F;

Pin::Number
    LedActivePin = Pin::_0,
    LedStatusPin = Pin::_1;


Led activeLed(LedsPort, LedActivePin);
Led statusLed(LedsPort, LedStatusPin);

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
    void led_active_set(bool on){
        activeLed.set(on);
    }
    void led_active_toggle()    {
        activeLed.toggle();
    }

    void led_status_set(bool on){
        statusLed.set(on);
    }
    void led_status_toggle()    {
        statusLed.toggle();
    }
}
