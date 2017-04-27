#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"

#include <stdint.h>
#include <vector>
#include <libopencm3/stm32/timer.h>

#ifdef __cplusplus

class Hall
{
public:
    Hall(Port::Number _portH1, Pin::Number _pinH1,
        Port::Number _portH2, Pin::Number _pinH2,
        Port::Number _portH3, Pin::Number _pinH3,
        Timer const& _timer,
        AltFunction::Number _altFunction)
    : portH1(_portH1)
    , pinH1(_pinH1)
    , portH2(_portH2)
    , pinH2(_pinH2)
    , portH3(_portH3)
    , pinH3(_pinH3)
    , timer(_timer)
    , altFunction(_altFunction)
    {
        init();
        enable();
    }

    ~Hall() {
        deinit();
    }

    void init();
    void deinit() { }

    void enable();
    void disable();

    float get_pulse_period_ms();
    int32_t get_pulse_count();
    int get_direction();

    void CC_interrupt_handler(void);

private:
    void init_timer();
    void init_gpio(Port::Number port, Pin::Number pin);
    int compute_and_get_direction();
    
    const Port::Number portH1;
    const Pin::Number  pinH1;
    const Port::Number portH2;
    const Pin::Number  pinH2;
    const Port::Number portH3;
    const Pin::Number  pinH3;

    const Timer& timer;
    const AltFunction::Number altFunction;

    bool enabled = false;
    uint16_t pulse_time = 0;
    int32_t pulse_count = 0;
    std::vector<int> last_hall_gpios_states;
    int direction = 0;
};

// Sensors
extern Hall hallsensor1;
extern Hall hallsensor2;

extern uint32_t rotation_speed_sensor1;
extern uint32_t rotation_speed_sensor2;

extern "C" {
#endif



#ifdef __cplusplus
}
#endif
