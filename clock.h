#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <stdint.h>

extern "C" void sys_tick_handler();


class Alarm
{
public:
    // Takes a callback that takes a reference on self.
    Alarm(std::function<void(Alarm&)> _callback, int _time, int _now, bool _repeat)
    : callback(_callback)
    , enabled(true)
    , time(_time)
    , lastCallTime(_now)
    , repeat(_repeat)
    { }

    ~Alarm() 
    { }

    void enable() { enabled = true; }
    void disable(){ enabled = false;}
    void setTime(int _time) { time = _time; }
    void setRepeat(bool _repeat) { repeat = _repeat; }

    void call(int thisTime) {
        if (callback && enabled) {
            if (repeat && (lastCallTime - thisTime) >= time) {
                lastCallTime = thisTime;
                callback(*this);
            } else if (!repeat && thisTime >= time) {
                disable();
                lastCallTime = thisTime;
                callback(*this);
            }
        }
    }

private:
    std::function<void(Alarm&)> callback;

    bool enabled;
    int time;
    int lastCallTime;
    bool repeat;


};


class System
{
public:
    System();

    int32_t getSysTick();
    void    sleep_ms(int32_t ms);
    void    sleep_us(int32_t us);

    void addAlarm(std::shared_ptr<Alarm> alarm);

private:
    friend void sys_tick_handler();
    void sysTick();

    std::vector<std::shared_ptr<Alarm>> alarms;

    volatile int32_t systick_count = 0;
};

