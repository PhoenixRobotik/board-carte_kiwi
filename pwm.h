#pragma once

#include <stdint.h>
#include "gpio_cpp.h"
#include <libopencm3/stm32/timer.h>

#ifdef __cplusplus

class PWM
{
public:
    PWM(Port::Number _port, Pin::Number _pin,
        volatile uint32_t* _reg, uint32_t _en,//RCC_APB2ENR, RCC_APB2ENR_TIM1EN
        uint32_t _timer,            // TIM1
        tim_oc_id _oc_id,           // TIM_OC1
        uint8_t _altFunction)       // GPIO_AF2
    : port(_port)
    , pin(_pin)
    , reg(_reg)
    , en(_en)
    , timer(_timer)
    , oc_id(_oc_id)
    , altFunction(_altFunction)
    {
        init();
    }

    ~PWM() {
        deinit();
    }

    void init();
    void deinit() { }

    void enable() { }
    void disable(){ }

    void setPeriod  (uint32_t period) { }
    void setPrescale(uint32_t prescale) { }
    void setDuty    (uint32_t duty);

    // More high level sets
    void setPercent (int percent) { }
    void setMillisec(int millisec){ }


    // Getters (conversion between different sets)
    uint32_t getPeriod();
    uint32_t getPrescale();
    uint32_t getDuty();
    int setPercent ();
    int setMillisec();

private:
    const Port::Number port;
    const Pin::Number  pin;

    // Low-level config
    volatile uint32_t* reg;
    uint32_t en;
    uint32_t timer;
    tim_oc_id oc_id;
    uint8_t altFunction;

    static const uint32_t PWM_PRESCALE = 1;
    static const uint32_t PWM_PERIOD   = 1024;



    uint32_t duty;

};


// LEDs PWM
extern PWM activeLedFloat;



extern "C" {
#endif

#ifdef __cplusplus
}
#endif
