#include "pwm.h"
#include "leds.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

void PWM::init() {
    // Enable timer clock
    m_timer->enable();
    // Enable GPIO clock
    m_pin.port->enable();

    // Reset timer peripheral
    timer_set_mode (m_timer->Id,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_repetition_counter(m_timer->Id, 0);
    timer_enable_preload        (m_timer->Id);
    timer_continuous_mode       (m_timer->Id);
    timer_set_period            (m_timer->Id, PWM_PERIOD);
    timer_set_prescaler         (m_timer->Id, PWM_GRANUL_PERIOD);

    // Setup GPIO
    gpio_mode_setup(m_pin.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE,    m_pin.number);
    gpio_set_af(    m_pin.port->Id, m_out_af, m_pin.number);
    gpio_set_output_options(
                    m_pin.port->Id, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, m_pin.number);


    timer_enable_break_main_output(m_timer->Id); // That's specific to Timer1
    timer_disable_oc_output (m_timer->Id, m_channel);
    timer_set_oc_mode       (m_timer->Id, m_channel, TIM_OCM_PWM1);
}

void PWM::enable() {
    enabled = true;
    // start timer
    timer_enable_counter(m_timer->Id);

    timer_set_oc_value      (m_timer->Id, m_channel, duty);
    timer_enable_oc_output  (m_timer->Id, m_channel);
}

void PWM::disable(){
    enabled = false;
    timer_disable_counter   (m_timer->Id);
    timer_disable_oc_output (m_timer->Id, m_channel);
}

void PWM::setDuty(uint32_t _duty) {
    duty = _duty;
    if (enabled)
        enable();
}

void PWM::setMicrosec(uint64_t _microsec) {
    setDuty(_microsec * PWM_GRANUL_FREQ_HZ / 1'000'000);
}
void PWM::setMillisec(int _millisec) {
    setMicrosec((uint64_t)_millisec * 1000);
}


void PWM::setPercent(int _percent) {
    setDuty(_percent * PWM_PERIOD / 100);
}

extern "C" {

    void pwm_set_duty   (PWM* pwm, uint32_t duty)   { pwm->setDuty(duty); }
    void pwm_set_ms     (PWM* pwm, int _millisec)   { pwm->setMillisec(_millisec); }
    void pwm_set_percent(PWM* pwm, int _percent)    { pwm->setPercent(_percent); }

}
