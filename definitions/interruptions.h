#pragma once

#include <memory>
#include <set>

class InterruptProvider;

class InterruptSubscriber
{
public:
    InterruptSubscriber(InterruptProvider* _provider, std::function<void(void)> _callback)
    : provider(_provider)
    , callback(_callback)
    { }
    ~InterruptSubscriber()
    { unsubscribe(); }

    void subscribe();
    void unsubscribe();

    InterruptProvider* const provider;
    std::function<void(void)> const callback;
};


class InterruptProvider {
public:
    InterruptProvider(uint8_t _id)
    : Id(_id)
    { }

    void subscribe  (InterruptSubscriber* subscriber);
    void unsubscribe(InterruptSubscriber* subscriber);

    void enable();
    void disable();
    void setPriority(int priority);

    // Cleanup subscribers on the fly
    void interrupt();

private:
    uint8_t const Id;

    std::set<InterruptSubscriber*> subscribers;
};

// Special, simpler interruption provider for systick. Should be called by class System only.
void setSysTickHandler(std::function<void(void)>);

extern InterruptProvider
    InterruptTimer1_CC,
    InterruptTimer2,
    InterruptCANRx1,
    InterruptEXTI0,
    InterruptEXTI1,
    InterruptEXTI2_TSC,
    InterruptEXTI5_to_9,
    InterruptEXTI10_to_15;
