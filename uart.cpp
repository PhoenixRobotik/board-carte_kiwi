#include "uart.h"

#include "leds.h"

#include <libopencm3/stm32/f3/nvic.h>
#include <libopencm3/stm32/usart.h>

USART kiwiUSART1(USART1, Port::pB, Pin::p7, Port::pB, Pin::p6);
USART kiwiUSART2(USART2, Port::pB, Pin::p4, Port::pB, Pin::p3);

extern "C"
void usart1_exti25_isr(void) {
    statusLed.toggle();
}


void USART::init() {
    // Enable interrupts
    nvic_enable_irq(NVIC_USART1_EXTI25_IRQ);

    // Setup GPIO TX
    gpio_mode_setup (m_portRX, GPIO_MODE_AF, GPIO_PUPD_NONE, m_pinRX);
    gpio_set_af     (m_portRX, GPIO_AF7, m_pinRX);

    gpio_mode_setup (m_portTX, GPIO_MODE_AF, GPIO_PUPD_NONE, m_pinTX);
    gpio_set_af     (m_portTX, GPIO_AF7, m_pinTX);
    gpio_set_output_options
                    (m_portTX, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, m_pinTX);

    usart_set_baudrate      (m_periphUSART, 38400);
    usart_set_databits      (m_periphUSART, 8);
    usart_set_stopbits      (m_periphUSART, USART_STOPBITS_1);
    usart_set_mode          (m_periphUSART, USART_MODE_TX_RX);
    usart_set_parity        (m_periphUSART, USART_PARITY_NONE);
    usart_set_flow_control  (m_periphUSART, USART_FLOWCONTROL_NONE);

    // Finally enable the USART.
    usart_enable(m_periphUSART);

    usart_enable_rx_interrupt(m_periphUSART);

}

void USART::sendNonBlocking(std::vector<uint8_t> const& data) {
    for (auto const& byte : data)
        usart_send(m_periphUSART, byte);
}
void USART::sendBlocking   (std::vector<uint8_t> const& data) {
    for (auto const& byte : data)
        usart_send_blocking(m_periphUSART, byte);
}

// Those return true if something was received
bool USART::receiveNonBlocking() {
    return false;
}
bool USART::receiveBlocking() {
    return false;
}

std::vector<uint8_t> USART::getReceptionBuffer() {
    std::vector<uint8_t> result = std::move(m_receptionBuffer);
    m_receptionBuffer.clear();
    return result;
}


