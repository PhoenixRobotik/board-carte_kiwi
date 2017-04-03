#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (64000000)
#define SYSTICK_FREQ_HZ     (1000)
#define SYSTICK_PERIOD      (RCC_CLOCK_FREQ_HZ/SYSTICK_FREQ_HZ)

#ifdef __cplusplus

extern "C" void sys_tick_handler();

class System
{
public:
    int32_t getSysTick();
    void    sleep_ms(int32_t ms);

private:
    // Private constructor because System is singleton.
    System();
    ~System() { }

    friend System& theSystem();
    friend void sys_tick_handler();

    volatile int32_t systick_count = 0;
};

System& theSystem();



extern "C" {
#endif

int get_systick();
void delay_nop(uint64_t count);
void delay_ms(unsigned int ms);

#ifdef __cplusplus
}
#endif
