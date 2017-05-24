#include "clock.h"

#include "config_macros.h"

#include "definitions/interruptions.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

#include <functional>

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

    setSysTickHandler(std::bind(&System::sysTick, this));

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOF);
}

uint32_t System::getSysTick() {
    return systick_count;
}

void System::sysTick() {
    systick_count++;
    for (auto alarm_it = alarms.begin(); alarm_it != alarms.end(); ++alarm_it) {
        // Remove alarms that are not referenced anymore (functions may be invalid too ^^)
        if (alarm_it->unique())
            alarm_it = alarms.erase(alarm_it);
        else {
            (*alarm_it)->call(systick_count);
        }

        
    }

}
void System::sleep_ms(uint32_t ms) {
    unsigned int count_max = systick_count + MILLIS_TO_SYSTICK(ms);
    while(systick_count < count_max) {}
}

void System::sleep_us(uint32_t us) {
    unsigned int count_max = systick_count + MICROS_TO_SYSTICK(us);
    while(systick_count < count_max) {}
}


void System::addAlarm(std::shared_ptr<Alarm> alarm) {
    alarms.push_back(alarm);
}
