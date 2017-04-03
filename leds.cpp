#include "leds.h"

#include <libopencm3/stm32/gpio.h>
void* __dso_handle;




Port LedsPort = GPIOF;

Port    LedActivePin = GPIO0,
        LedStatusPin = GPIO1;


Led activeLed(LedsPort, LedActivePin);
Led statusLed(LedsPort, LedStatusPin);

extern "C" {
    void led_active_set(bool on){   activeLed.set(on);  }
    void led_active_toggle()    {   activeLed.toggle(); }

    void led_status_set(bool on){   statusLed.set(on);  }
    void led_status_toggle()    {   statusLed.toggle(); }
}
