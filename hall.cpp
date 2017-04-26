#include "hall.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

Hall hallsensor1(Port::pA, Pin::p8,
            Port::pA, Pin::p9,
            Port::pA, Pin::p10,
            Timer1,
            AltFunction::f6);

Hall hallsensor2(Port::pA, Pin::p0,
            Port::pA, Pin::p1,
            Port::pA, Pin::p2,
            Timer2,
            AltFunction::f1);

uint32_t rotation_speed_sensor1 = 0;
uint32_t rotation_speed_sensor2 = 0;

// TODO : need to handle counter overflow

void Hall::init() {
    // Enable GPIO clock
    rcc_periph_clock_enable(RCC_GPIOA); // TODO pass this
    Hall::init_gpio(portH1, pinH1);
    Hall::init_gpio(portH2, pinH2);
    Hall::init_gpio(portH3, pinH3);

    // Enable timer clock
    rcc_periph_clock_enable(timer.ClockEnable);
    Hall::init_timer();
}

void Hall::init_timer() {
    // Reset timer peripheral
    timer_set_mode (timer.Peripheral,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_repetition_counter(timer.Peripheral, 0);
    timer_enable_preload        (timer.Peripheral);
    timer_continuous_mode       (timer.Peripheral);
    timer_set_period            (timer.Peripheral, HALL_COUNTER_PERIOD);
    timer_set_prescaler         (timer.Peripheral, HALL_PRESCALER);

    // That's specific to Timer1
    timer_enable_break_main_output(timer.Peripheral);

    // timer_set_clock_division(timer.Peripheral, TIM_CR1_CKD_CK_INT);
    timer_set_ti1_ch123_xor(timer.Peripheral);
    timer_slave_set_trigger(timer.Peripheral, TIM_SMCR_TS_TI1F_ED);
    timer_slave_set_mode(timer.Peripheral, TIM_SMCR_SMS_RM);

    timer_ic_disable(timer.Peripheral, TIM_IC1);
    timer_ic_set_polarity(timer.Peripheral, TIM_IC1, TIM_IC_BOTH); // or TIM_IC_BOTH ?
    timer_ic_set_input(timer.Peripheral, TIM_IC1, TIM_IC_IN_TRC);
    //timer_ic_set_prescaler(timer.Peripheral, TIM_IC1, TIM_IC_PSC_OFF);
    timer_ic_set_filter(timer.Peripheral, TIM_IC1, TIM_IC_DTF_DIV_32_N_8);

    timer_set_counter(timer.Peripheral, 0);
    
    nvic_set_priority(timer.InterruptId, 0);
}

void  Hall::init_gpio(Port::Number port, Pin::Number pin) {
    gpio_set_output_options(
                port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                pin);
    gpio_mode_setup(
                port, GPIO_MODE_AF, GPIO_PUPD_NONE,
                pin);
    gpio_set_af(port, altFunction,
                pin);
}

void Hall::enable() {
    enabled = true;
    // initialize stuff for direction dection
    last_gpio_arrangement = {-1, -1, -1};
    compute_and_get_direction();

    nvic_enable_irq(timer.InterruptId);
    timer_enable_irq(timer.Peripheral,TIM_DIER_CC1IE);
    timer_ic_enable(timer.Peripheral, TIM_IC1);
    timer_enable_counter(timer.Peripheral);
}

void Hall::disable() {
    enabled = false;
    timer_disable_counter(timer.Peripheral);
    timer_ic_disable(timer.Peripheral, TIM_IC1);
    timer_disable_irq(timer.Peripheral,TIM_DIER_CC1IE);
    // if timer is used for other stuff remove this
    nvic_disable_irq(timer.InterruptId);
}

float Hall::get_pulse_period_ms() {
    return 1000*pulse_time/HALL_SAMPLE_FREQ_HZ;
}

int32_t Hall::get_pulse_count() {
    return pulse_count;
}

int Hall::get_direction() {
    return direction;
}

int Hall::compute_and_get_direction() {
    std::vector<int> actual_gpio_arrangement = {
        gpio_get(portH1, pinH1),
        gpio_get(portH2, pinH2),
        gpio_get(portH3, pinH3)};

    int i = 0;
    while (actual_gpio_arrangement[i] == last_gpio_arrangement[i] && i != 4) {
        i++;
    }

    if (i == 4 or last_toggled_gpio == 4) {
        // should fall here only at enable time
        // TODO : if fall here on error ?
        direction = 0;
    } else if (i == last_toggled_gpio) {
        direction = - direction;
    } else {
        direction = i - last_toggled_gpio;
        if (direction == 2) {
            direction = -1;
        }
        if (direction == -2) {
            direction = 1;
        }
    }

    last_toggled_gpio = i;
    last_gpio_arrangement = actual_gpio_arrangement;

    return direction;
}

void Hall::CC_interrupt_handler(void) {
    //if (timer_get_flag(timer.Peripheral, TIM_SR_CC1IF) == true)
    if (timer_interrupt_source(timer.Peripheral, TIM_SR_CC1IF) == true)
    {
        timer_clear_flag(timer.Peripheral, TIM_SR_CC1IF);
        pulse_time   = TIM_CCR1(timer.Peripheral);
        pulse_count += compute_and_get_direction();
    } else {
        ; // should never fall here
    }
}

void tim1_cc_isr(void)
{
    hallsensor1.CC_interrupt_handler();
}

void tim2_isr(void)
{
    hallsensor2.CC_interrupt_handler();
}

extern "C" {


}
