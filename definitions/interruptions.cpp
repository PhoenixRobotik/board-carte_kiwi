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
    InterruptTimer2     (NVIC_TIM2_IRQ),
    InterruptCANRx1     (NVIC_CAN1_RX1_IRQ),
    InterruptEXTI1      (NVIC_EXTI1_IRQ),
    InterruptEXTI2_TSC  (NVIC_EXTI2_TSC_IRQ);


// Systick handling
std::function<void(void)> sysTickHandler = {};

void setSysTickHandler(std::function<void(void)> externalHandler) {
    sysTickHandler = externalHandler;
}

// LibOpenCm3 export
extern "C"

void sys_tick_handler() {
    if (sysTickHandler) {
        sysTickHandler();
    }
}

void tim1_cc_isr(void)
{
    InterruptTimer1_CC.interrupt();
}

void tim2_isr(void)
{
    InterruptTimer2.interrupt();
}

void can1_rx1_isr(void)
{
    InterruptCANRx1.interrupt();
}

void exti1_isr(void)     { InterruptEXTI1.interrupt();     }
void exti2_tsc_isr(void) { InterruptEXTI2_TSC.interrupt(); }

// void exti0_isr(void) { }
// void exti3_isr(void) { }
// void exti4_isr(void) { }
// void exti15_10_isr(void) { }
// void exti9_5_isr(void) { }
