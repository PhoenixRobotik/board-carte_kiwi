#include "gpio.h"


void GPIO::set(bool on) {
    on  ? gpio_set  (pin.port->Id, pin.number)
        : gpio_clear(pin.port->Id, pin.number);
}
void GPIO::toggle() {
    gpio_toggle(pin.port->Id, pin.number);
}

void GPIO::init() {
    pin.port->enable();
    gpio_mode_setup(
        pin.port->Id,
        m_io_mode,
        m_pull_mode,
        pin.number);
}

bool GPIO::enable_irq(bool on_rising, bool on_falling) {
    if (m_io_mode != input) {
        return false;
    }
    // no event selected
    if (not on_rising and not on_falling) {
        return false;
    }

    // realy bad should change this
    uint32_t exti = pin.number;

    // EXTI already in use
    if (((EXTI_IMR | EXTI_EMR) & exti) != 0) {
        return false;
    }

    exti_set_trigger   (exti,
        (on_rising and on_falling) ? EXTI_TRIGGER_BOTH   :
         on_rising                 ? EXTI_TRIGGER_RISING :
                                     EXTI_TRIGGER_FALLING);
    exti_select_source (exti, pin.port->Id);
    exti_enable_request(exti);

    return true;
}

bool GPIO::disable_irq() {
    if (m_io_mode != input) {
        return false;
    }

    // realy bad should change this
    uint32_t exti = pin.number;

    exti_disable_request(exti);
    return true;
}

extern "C" {

    void GPIO_set    (GPIO* gpio, bool on) {   gpio->set(on);   }
    void GPIO_toggle (GPIO* gpio)          {   gpio->toggle();  }

}
