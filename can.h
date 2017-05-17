#pragma once

#include "definitions/peripheral.h"
#include "definitions/gpio_cpp.h"
#include "definitions/interruptions.h"

#include <string>
#include <stdint.h>

class CANBus
{
public:
    CANBus(Peripheral* CANPeriph, //InterruptProvider* interrupt,
        Pin rx, AltFunction::Number rx_af,
        Pin tx, AltFunction::Number tx_af,
        bool _non_automatic_retransmit = false)
    : m_CANPeriph(CANPeriph)
    //, m_Rx1_interrupt(interrupt, std::bind(&CANBus::CAN_Rx1_interrupt_handler, this))
    , m_rx(rx)
    , m_tx(tx)
    , m_rx_af(rx_af)
    , m_tx_af(tx_af)
    , non_automatic_retransmit(_non_automatic_retransmit)
    {
        init();
    }
    ~CANBus() { deinit(); }

    void init();
    void deinit();

    // should enable filtering
    void setId(uint32_t newId);

    // Send only one message

    bool send(uint32_t id, uint8_t* data, size_t dataSize);

    template<typename T>
    bool send(uint32_t id, T data) {
        static_assert(sizeof(T) <= 8, "Size of template data type");
        return send(id, (uint8_t*)&data, sizeof(T));
    }
    template<typename T>
    bool send(uint32_t id, T* data) {
        static_assert(sizeof(T) <= 8, "Size of template data type");
        return send(id, (uint8_t*)data, sizeof(T));
    }


    bool receive(uint32_t *id, uint8_t *message, uint8_t *length);

    void CAN_Rx1_interrupt_handler(void);



private:
    Peripheral* m_CANPeriph;
    // InterruptSubscriber m_Rx1_interrupt;
    Pin m_rx, m_tx;
    AltFunction::Number m_rx_af, m_tx_af;

    bool non_automatic_retransmit;


    static const int32_t maxSendRetries = 2500;

    uint32_t m_id;
};
