#pragma once

#include "definitions/peripheral.h"
#include "definitions/gpio_cpp.h"

#include <queue>
#include <stdint.h>

class USART {
public:
    USART(Peripheral* UARTPeriph,
        Pin rx, AltFunction::Number rx_af,
        Pin tx, AltFunction::Number tx_af,
        int baudrate = 115200)
    : m_UARTPeriph(UARTPeriph)
    , m_rx(rx)
    , m_tx(tx)
    , m_rx_af(rx_af)
    , m_tx_af(tx_af)
    , m_baudrate(baudrate)
    { }

    ~USART() { deinit(); }

    void init();
    void deinit();

    void sendNonBlocking(std::vector<uint8_t> const& data);
    void sendBlocking   (std::vector<uint8_t> const& data);

    // Those return true if something was received
    bool receiveNonBlocking();
    bool receiveBlocking();

    // *Moves* the buffer into this vector.
    std::vector<uint8_t> getReceptionBuffer();

private:
    Peripheral* m_UARTPeriph;
    Pin m_rx, m_tx;
    AltFunction::Number m_rx_af, m_tx_af;

    int m_baudrate;


    volatile int m_emissionBufferPosition;
    volatile int m_receptionBufferPosition;

    std::vector<uint8_t> m_receptionBuffer;
};
