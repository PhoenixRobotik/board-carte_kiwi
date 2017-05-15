#include "interruptions.h"

#include <libopencm3/cm3/nvic.h>


void InterruptSubscriber::subscribe() {
    provider->  subscribe(this);
}
void InterruptSubscriber::unsubscribe() {
    provider->unsubscribe(this);
}

void InterruptProvider::subscribe(InterruptSubscriber* subscriber) {
    if (subscribers.empty())
        enable();
    subscribers.insert(subscriber);
}
void InterruptProvider::unsubscribe(InterruptSubscriber* subscriber) {
    subscribers.erase(subscriber);
    if (subscribers.empty())
        disable();
}
void InterruptProvider::enable() {
    nvic_enable_irq(Id);
}
void InterruptProvider::disable() {
    nvic_disable_irq(Id);
}
void InterruptProvider::setPriority(int priority) {
    nvic_set_priority(Id, priority);
}

// Cleanup subscribers on the fly
void InterruptProvider::interrupt() {
    for (auto subscriber : subscribers)
        subscriber->callback();
}


InterruptProvider
    InterruptTimer1_CC  (NVIC_TIM1_CC_IRQ),
    InterruptTimer2     (NVIC_TIM2_IRQ);


void tim1_cc_isr(void)
{
    InterruptTimer1_CC.interrupt();
}
void tim2_isr(void)
{
    InterruptTimer2.interrupt();
}


// Systick handling
std::function<void(void)> sysTickHandler = {};

void setSysTickHandler(std::function<void(void)> externalHandler) {
    sysTickHandler = externalHandler;
}
// LibOpenCm3 export
extern "C"
void sys_tick_handler() {
    if (sysTickHandler)
        sysTickHandler();
}
