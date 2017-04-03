#include "leds.h"

#include <libopencm3/stm32/gpio.h>

Port::Number
    LedsPort = Port::_F;

Pin::Number
    LedActivePin = Pin::_0,
    LedStatusPin = Pin::_1;


Led activeLed(LedsPort, LedActivePin);
Led statusLed(LedsPort, LedStatusPin);

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
