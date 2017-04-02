#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (64000000)
#define SYSTICK_FREQ_HZ     (1000)
#define SYSTICK_PERIOD      (RCC_CLOCK_FREQ_HZ/SYSTICK_FREQ_HZ)

#ifdef __cplusplus
extern "C" {
#endif

void init_clock();

int get_systick();
void delay_nop(uint64_t count);
void delay_ms(unsigned int ms);

// // Alarms (LEDs, Video/Photo)
// typedef struct {
//     int time_wakeup;
//     void (*callback) (void);
// } Alarm;


// void setupAlarm(Alarm* alarm, int time_millis);

#ifdef __cplusplus
}
#endif
