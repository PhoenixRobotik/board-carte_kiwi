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


void Hall::init() {
    // Enable timer clock
    rcc_periph_clock_enable(timer.ClockEnable);
    Hall::init_timer();

    // Enable GPIO clock
    rcc_periph_clock_enable(RCC_GPIOA); // TODO pass this
    Hall::init_gpio(portH1, pinH1);
    Hall::init_gpio(portH2, pinH2);
    Hall::init_gpio(portH3, pinH3);
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
    timer_set_period            (timer.Peripheral, 65535); //need to be changed
    timer_set_prescaler         (timer.Peripheral, 126); //need to be changed

    // That's specific to Timer1
    timer_enable_break_main_output(timer.Peripheral);

    // timer_set_clock_division(timer.Peripheral, TIM_CR1_CKD_CK_INT);
    timer_set_ti1_ch123_xor(timer.Peripheral);
    timer_slave_set_trigger(timer.Peripheral, TIM_SMCR_TS_TI1F_ED);
    timer_slave_set_mode(timer.Peripheral, TIM_SMCR_SMS_RM);

    timer_ic_disable(timer.Peripheral, TIM_IC1);
    timer_ic_set_polarity(timer.Peripheral, TIM_IC1, TIM_IC_RISING); // or TIM_IC_BOTH ?
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
    nvic_enable_irq(timer.InterruptId);
    timer_enable_irq(timer.Peripheral,TIM_DIER_CC1IE);
    timer_ic_enable(timer.Peripheral, TIM_IC1);
    timer_enable_counter(timer.Peripheral);
}

void Hall::disable(){
    enabled = false;
    timer_disable_counter(timer.Peripheral);
    timer_ic_disable(timer.Peripheral, TIM_IC1);
    timer_disable_irq(timer.Peripheral,TIM_DIER_CC1IE);
    // if timer is used for other stuff remove this
    nvic_disable_irq(timer.InterruptId);
}

void tim1_cc_isr(void)
{
  //if (timer_get_flag(TIM2, TIM_SR_CC1IF) == true)
  if (timer_interrupt_source(TIM1, TIM_SR_CC1IF) == true)
  {
    timer_clear_flag(TIM1, TIM_SR_CC1IF);
    // don't trust this equation, it's here only for test
    // maths will come…
    rotation_speed_sensor1 = 100000/TIM_CCR1(TIM1)*20*3;
    //rotation_speed_sensor1 += 1;

  } else {
    ; // should never fall here
  }
}

void tim2_isr(void)
{

  //if (timer_get_flag(TIM2, TIM_SR_CC1IF) == true)
  if (timer_interrupt_source(TIM2, TIM_SR_CC1IF) == true)
  {
    timer_clear_flag(TIM2, TIM_SR_CC1IF);
    // don't trust this equation, it's here only for test
    // maths will come…
    rotation_speed_sensor2 = 100000/TIM_CCR1(TIM2)*20*3;
    //rotation_speed_sensor2 += 1;

  } else {
    ; // should never fall here
  }
}

extern "C" {


}
