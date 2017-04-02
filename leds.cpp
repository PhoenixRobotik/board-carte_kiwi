#include "leds.h"

#include <libopencm3/stm32/gpio.h>
void* __dso_handle;




Port LedsPort = GPIOF;

Port    LedActivePin = GPIO0,
        LedStatusPin = GPIO1;


Led activeLed(LedsPort, LedActivePin);
Led statusLed(LedsPort, LedStatusPin);

#ifdef __cplusplus
extern "C" {
#endif


void init_leds() {
    activeLed.init();
    statusLed.init();
}

void led_active_set(bool on){   activeLed.set(on);  }
void led_active_toggle()    {   activeLed.toggle(); }

void led_status_set(bool on){   statusLed.set(on);  }
void led_status_toggle()    {   statusLed.toggle(); }

#ifdef __cplusplus
}
#endif
