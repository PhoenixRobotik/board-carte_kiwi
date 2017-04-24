#include "hall.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

// Hall hallsensor1(Port::pA, Pin::p8,
//             Port::pA, Pin::p9,
//             Port::pA, Pin::p10,
//             Timer1,
//             AltFunction::f6);

Hall hallsensor2(Port::pA, Pin::p0,
            Port::pA, Pin::p1,
            Port::pA, Pin::p2,
            Timer2,
            AltFunction::f1);


void Hall::init() {
    // Enable timer clock
    rcc_periph_clock_enable(timer.ClockEnable);
    // Enable GPIO clock
    rcc_periph_clock_enable(RCC_GPIOA); // TODO pass this
    // Enable timer interrupt
    nvic_enable_irq(timer.InterruptId);
    // Reset timer peripheral to defaults
    // rcc_periph_reset_pulse(timer.Reset);

    // Reset timer peripheral
    timer_set_mode (timer.Peripheral,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_repetition_counter(timer.Peripheral, 0);
    timer_enable_preload        (timer.Peripheral);
    timer_continuous_mode       (timer.Peripheral);
    timer_set_period            (timer.Peripheral, 65535); //need to be changed
    timer_set_prescaler         (timer.Peripheral, 126); //need to be changed

    // Setup GPIO H1
    gpio_set_output_options(
                portH1, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                pinH1);
    gpio_mode_setup(
                portH1, GPIO_MODE_AF, GPIO_PUPD_NONE,
                pinH1);
    gpio_set_af(portH1, altFunction,
                pinH1);

    // Setup GPIO H2
    gpio_set_output_options(
                portH2, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                pinH2);
    gpio_mode_setup(
                portH2, GPIO_MODE_AF, GPIO_PUPD_NONE,
                pinH2);
    gpio_set_af(portH2, altFunction,
                pinH2);

    // Setup GPIO H3
    gpio_set_output_options(
                portH3, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                pinH3);
    gpio_mode_setup(
                portH3, GPIO_MODE_AF, GPIO_PUPD_NONE,
                pinH3);
    gpio_set_af(portH3, altFunction,
                pinH3);

    timer_enable_break_main_output(timer.Peripheral); // That's specific to Timer1
    timer_set_ti1_ch123_xor(timer.Peripheral);
    timer_slave_set_trigger(timer.Peripheral, TIM_SMCR_TS_TI1F_ED);
    timer_slave_set_mode(timer.Peripheral, TIM_SMCR_SMS_RM);

    timer_ic_disable(timer.Peripheral, TIM_IC1);
    timer_ic_set_polarity(timer.Peripheral, TIM_IC1, TIM_IC_RISING); // or TIM_IC_BOTH ?
    timer_ic_set_input(timer.Peripheral, TIM_IC1, TIM_IC_IN_TRC);
    timer_ic_set_prescaler(timer.Peripheral, TIM_IC1, TIM_IC_PSC_OFF);
    timer_ic_set_filter(timer.Peripheral, TIM_IC1, TIM_IC_DTF_DIV_32_N_8);

}

void Hall::enable() {
    enabled = true;
    // code here
}

void Hall::disable(){
    enabled = false;
    // code here
}

extern "C" {


}
