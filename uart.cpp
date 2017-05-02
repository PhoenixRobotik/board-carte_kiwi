#include "uart.h"

#include "leds.h"

namespace libopencm3 {
    #include <libopencm3/stm32/f3/nvic.h>
    #include <libopencm3/stm32/usart.h>
}

using namespace libopencm3;

extern "C"
void usart1_exti25_isr(void) {

}


void USART::init() {
    // // Enable interrupts
    // nvic_enable_irq(NVIC_USART1_EXTI25_IRQ);

    // Setup GPIO TX
    gpio_mode_setup(m_rx.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE, m_rx.number);
    gpio_mode_setup(m_tx.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE, m_tx.number);

    gpio_set_af(    m_rx.port->Id, m_rx_af, m_rx.number);
    gpio_set_af(    m_tx.port->Id, m_tx_af, m_tx.number);

    gpio_set_output_options(
                    m_tx.port->Id, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, m_tx.number);

    usart_set_baudrate      (m_UARTPeriph->Id, m_baudrate);
    usart_set_databits      (m_UARTPeriph->Id, 8);
    usart_set_stopbits      (m_UARTPeriph->Id, USART_STOPBITS_1);
    usart_set_mode          (m_UARTPeriph->Id, USART_MODE_TX_RX);
    usart_set_parity        (m_UARTPeriph->Id, USART_PARITY_NONE);
    usart_set_flow_control  (m_UARTPeriph->Id, USART_FLOWCONTROL_NONE);

    // Finally enable the USART.
    usart_enable(m_UARTPeriph->Id);

    usart_enable_rx_interrupt(m_UARTPeriph->Id);

}

void USART::deinit() {

}

void USART::sendNonBlocking(std::vector<uint8_t> const& data) {
    for (auto const& byte : data)
        usart_send(m_UARTPeriph->Id, byte);
}
void USART::sendBlocking   (std::vector<uint8_t> const& data) {
    for (auto const& byte : data)
        usart_send_blocking(m_UARTPeriph->Id, byte);
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


