#include "hall.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

// TODO : need to handle counter overflow

void Hall::init() {
    // Enable GPIO clock
    init_gpio(m_h1_in);
    init_gpio(m_h2_in);
    init_gpio(m_h3_in);

    // Enable timer clock
    init_timer();
}

void Hall::init_timer() {
    m_timer->enable();

    // Reset timer peripheral
    timer_set_mode (m_timer->Id,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_repetition_counter(m_timer->Id, 0);
    timer_enable_preload        (m_timer->Id);
    timer_continuous_mode       (m_timer->Id);
    timer_set_period            (m_timer->Id, HALL_COUNTER_PERIOD);
    timer_set_prescaler         (m_timer->Id, HALL_PRESCALER);

    // That's specific to Timer1
    timer_enable_break_main_output(m_timer->Id);

    // timer_set_clock_division(m_timer->Id, TIM_CR1_CKD_CK_INT);
    timer_set_ti1_ch123_xor(m_timer->Id);
    timer_slave_set_trigger(m_timer->Id, TIM_SMCR_TS_TI1F_ED);
    timer_slave_set_mode(m_timer->Id, TIM_SMCR_SMS_RM);

    timer_ic_disable(m_timer->Id, TIM_IC1);
    timer_ic_set_polarity(m_timer->Id, TIM_IC1, TIM_IC_BOTH); // or TIM_IC_BOTH ?
    timer_ic_set_input(m_timer->Id, TIM_IC1, TIM_IC_IN_TRC);
    //timer_ic_set_prescaler(m_timer->Id, TIM_IC1, TIM_IC_PSC_OFF);
    timer_ic_set_filter(m_timer->Id, TIM_IC1, TIM_IC_DTF_DIV_32_N_8);

    timer_set_counter(m_timer->Id, 0);
}

void Hall::init_gpio(Pin pin) {
    pin.port->enable();
    gpio_set_output_options(
                pin.port->Id, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pin.number);
    gpio_mode_setup(
                pin.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE, pin.number);
    gpio_set_af(pin.port->Id, m_hX_af, pin.number);
}

void Hall::enable() {
    enabled = true;
    // initialize stuff for direction dection
    last_hall_gpios_states = {
        gpio_get(m_h1_in.port->Id, m_h1_in.number),
        gpio_get(m_h2_in.port->Id, m_h2_in.number),
        gpio_get(m_h3_in.port->Id, m_h3_in.number)
    };


    m_timerInterrupt.provider->setPriority(0);
    m_timerInterrupt.subscribe();

    timer_enable_irq(m_timer->Id, TIM_DIER_CC1IE);
    timer_ic_enable(m_timer->Id, TIM_IC1);
    timer_enable_counter(m_timer->Id);
}

void Hall::disable() {
    enabled = false;
    timer_disable_counter(m_timer->Id);
    timer_ic_disable(m_timer->Id, TIM_IC1);
    timer_disable_irq(m_timer->Id,TIM_DIER_CC1IE);

    m_timerInterrupt.unsubscribe();
}

double Hall::get_pulse_period_ms() {
    return (1000. * pulse_time) / HALL_SAMPLE_FREQ_HZ;
}

int32_t Hall::get_pulse_count() {
    return pulse_count;
}

int Hall::get_direction() {
    return direction;
}

int Hall::compute_and_get_direction() {
    std::vector<int> current_hall_gpios_states = {
        gpio_get(m_h1_in.port->Id, m_h1_in.number),
        gpio_get(m_h2_in.port->Id, m_h2_in.number),
        gpio_get(m_h3_in.port->Id, m_h3_in.number)
    };

    int toggled_gpio = 0;
    int edge_direction = 0;
    int still_down_gpio = 0;

    for (int i = 0; i < 3; i++) {

        int gpio_state = current_hall_gpios_states[i];

        if (gpio_state != last_hall_gpios_states[i]) {
            toggled_gpio = i;
            edge_direction = ((gpio_state == 0) ? -1 : 1);
        }
        else if (gpio_state == 0) {
            still_down_gpio = i;
        }
    }

    // +1 and -1 before and after the modulo operation
    // in order to obtain -1 when we should obtain 2
    direction  = still_down_gpio - toggled_gpio + 1;
    // this modulo implementation gives a strict positive result
    direction  = (direction < 0 ? (direction % 3) + 3 : direction % 3 );
    direction -= 1;
    direction *= edge_direction;

    last_hall_gpios_states = current_hall_gpios_states;

    return direction;
}

void Hall::CC_interrupt_handler(void) {
    //if (timer_get_flag(m_timer->Id, TIM_SR_CC1IF) == true)
    if (timer_interrupt_source(m_timer->Id, TIM_SR_CC1IF) == true)
    {
        timer_clear_flag(m_timer->Id, TIM_SR_CC1IF);
        pulse_time   = TIM_CCR1(m_timer->Id);
        pulse_count += compute_and_get_direction();
    } else {
        ; // should never fall here
    }
}
