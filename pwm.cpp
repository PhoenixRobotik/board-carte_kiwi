#include "pwm.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


PWM activeLedFloat(Port::_F, Pin::_0,
        &RCC_APB2ENR, RCC_APB2ENR_TIM1EN,
        TIM1_BASE,  // TIM1
        TIM_OC3N,
        GPIO_AF6);


void PWM::init() {
    // Enable timer clock
    rcc_peripheral_enable_clock(reg, en);
    // Enable GPIO clock
    // rcc_peripheral_enable_clock(&RCC_AHBENR, RCC_AHBENR_GPIOAEN);

    // Reset timer peripheral
    timer_reset   (timer);
    timer_set_mode(timer,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler         (timer, PWM_PRESCALE);
    timer_set_repetition_counter(timer, 0);
    timer_enable_preload        (timer);
    timer_continuous_mode       (timer);
    timer_set_period            (timer, PWM_PERIOD);

    // start timer
    timer_enable_break_main_output(timer);
    timer_enable_counter        (timer);



    // Set timer channel to output
    gpio_set_output_options(
                port,
                GPIO_OTYPE_PP,
                GPIO_OSPEED_100MHZ,
                pin);
    gpio_mode_setup(
                port,
                GPIO_MODE_AF,
                GPIO_PUPD_NONE,
                pin);
    gpio_set_af(port,
                GPIO_AF2,
                pin);

    timer_disable_oc_output (timer, oc_id);
    timer_set_oc_mode       (timer, oc_id, TIM_OCM_PWM1);
    timer_set_oc_value      (timer, oc_id, 0);
    timer_enable_oc_output  (timer, oc_id);
}

void PWM::setDuty(uint32_t duty) {
    timer_set_oc_value(timer, oc_id, duty);
}

