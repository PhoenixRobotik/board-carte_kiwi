#include "pwm.h"
#include "leds.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

PWM moteur1(Port::_B, Pin::_0,
            Timer3,
            AltFunction::_2,
            TIM_OC3);


void PWM::init() {
    // Enable timer clock
    rcc_periph_clock_enable(timer.ClockEnable);
    // Enable GPIO clock
    rcc_periph_clock_enable(RCC_GPIOB); // TODO pass this
    // Enable timer interrupt
    nvic_enable_irq(timer.InterruptId);
    // Reset timer peripheral to defaults
    rcc_periph_reset_pulse(timer.Reset);

    // Reset timer peripheral
    timer_set_mode (timer.Peripheral,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_repetition_counter(timer.Peripheral, 0);
    timer_enable_preload        (timer.Peripheral);
    timer_continuous_mode       (timer.Peripheral);
    timer_set_period            (timer.Peripheral, PWM_PERIOD);
    timer_set_prescaler         (timer.Peripheral, PWM_GRANUL_PERIOD);

    // Setup GPIO
    gpio_set_output_options(
                port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                pin);
    gpio_mode_setup(
                port, GPIO_MODE_AF, GPIO_PUPD_NONE,
                pin);
    gpio_set_af(port, altFunction,
                pin);


    timer_enable_break_main_output(timer.Peripheral); // That's specific to Timer1
    timer_disable_oc_output (timer.Peripheral, channel);
    timer_set_oc_mode       (timer.Peripheral, channel, TIM_OCM_PWM1);
}

void PWM::enable() {
    enabled = true;
    // start timer
    timer_enable_counter(timer.Peripheral);

    timer_set_oc_value      (timer.Peripheral, channel, duty);
    timer_enable_oc_output  (timer.Peripheral, channel);
}

void PWM::disable(){
    enabled = false;
    timer_disable_counter   (timer.Peripheral);
    timer_disable_oc_output (timer.Peripheral, channel);
}

void PWM::setDuty(uint32_t _duty) {
    duty = _duty;
    if (enabled)
        enable();
}

