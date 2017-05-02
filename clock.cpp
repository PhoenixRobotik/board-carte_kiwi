#include "clock.h"

#include "config_macros.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

void* __dso_handle;

System::System() {
    rcc_clock_setup_hsi(&rcc_hsi_8mhz[RCC_CLOCK_64MHZ]);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // Interrupts each millisec
    systick_set_reload(SYSTICK_PERIOD - 1);
    // clear counter so it starts right away
    systick_clear();
    systick_counter_enable();
    systick_interrupt_enable();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
}

int32_t System::getSysTick() {
    return systick_count;
}

void System::sleep_ms(int32_t ms) {
    int count_max = systick_count + MILLIS_TO_SYSTICK(ms);
    while(systick_count < count_max) {}
}

void System::sleep_us(int32_t us) {
    int count_max = systick_count + MICROS_TO_SYSTICK(us);
    while(systick_count < count_max) {}
}
