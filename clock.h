#pragma once

#include <stdint.h>

#ifdef __cplusplus

extern "C" void sys_tick_handler();

class System
{
public:
    System();

    int32_t getSysTick();
    void    sleep_ms(int32_t ms);
    void    sleep_us(int32_t us);

private:
    friend void sys_tick_handler();
    void sysTick();

    volatile int32_t systick_count = 0;
};

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
