#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

#include "clock.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_clock() {
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

volatile int systick_count = 0;
#define MILLIS_TO_SYSTICK(ms) (ms * SYSTICK_FREQ_HZ / 1000)


void sys_tick_handler() {
    systick_count++;

    // Alarms
    // alarmCall(&LedsBleuesAlarm);
    // alarmCall(&LedsRougesAlarm);
    // alarmCall(&PhotoVideoAlarm);
}

int get_systick() {
    return systick_count;
}


// Should not be used with FreeRTOS !!
void delay_nop(uint64_t count) {
    for (uint64_t i = 0; i < count; ++i)
        __asm__("nop");
}

void delay_ms(unsigned int ms) {
    int count_max = systick_count + MILLIS_TO_SYSTICK(ms);
    while(systick_count < count_max) {}
}

#ifdef __cplusplus
}
#endif
