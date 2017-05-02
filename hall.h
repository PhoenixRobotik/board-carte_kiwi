#pragma once

#include "config_macros.h"

#include "definitions/gpio_cpp.h"
#include "definitions/timers_cpp.h"
#include "definitions/interruptions.h"

#include <stdint.h>
#include <vector>

class Hall
{
public:
    Hall(Timer* _timer, InterruptProvider* interrupt,
        Pin h1_in, Pin h2_in, Pin h3_in,
        AltFunction::Number _altFunction)
    : m_timer(_timer)
    , m_timerInterrupt(interrupt, std::bind(&Hall::CC_interrupt_handler, this))
    , m_h1_in(h1_in)
    , m_h2_in(h2_in)
    , m_h3_in(h3_in)
    , m_hX_af(_altFunction)
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

    double get_pulse_period_ms();
    int32_t get_pulse_count();
    int get_direction();

    void CC_interrupt_handler(void);

private:
    void init_timer();
    void init_gpio(Pin pin);
    int compute_and_get_direction();

    Timer* const m_timer;
    InterruptSubscriber m_timerInterrupt;

    Pin const m_h1_in, m_h2_in, m_h3_in;
    // Same alternate function for all pinouts (?)
    AltFunction::Number const m_hX_af;

    bool enabled = false;
    uint16_t pulse_time = 0;
    int32_t pulse_count = 0;
    std::vector<int> last_hall_gpios_states;
    int direction = 0;
};
