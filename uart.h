#pragma once


#ifdef __cplusplus

#include "definitions/gpio_cpp.h"

#include <queue>
#include <stdint.h>

class USART {
public:
    USART(uint32_t periphUSART, Port::Number portRX, Pin::Number pinRX, Port::Number portTX, Pin::Number pinTX)
    : m_periphUSART(periphUSART)
    , m_portRX(portRX)
    , m_portTX(portTX)
    , m_pinRX(pinRX)
    , m_pinTX(pinTX)
    { }
    ~USART() { }

    void init();

    void sendNonBlocking(std::vector<uint8_t> const& data);
    void sendBlocking   (std::vector<uint8_t> const& data);

    // Those return true if something was received
    bool receiveNonBlocking();
    bool receiveBlocking();

    // *Moves* the buffer into this vector.
    std::vector<uint8_t> getReceptionBuffer();

private:
    uint32_t m_periphUSART;
    Port::Number m_portRX;
    Port::Number m_portTX;
    Pin::Number m_pinRX;
    Pin::Number m_pinTX;

    volatile int m_emissionBufferPosition;
    volatile int m_receptionBufferPosition;

    std::vector<uint8_t> m_receptionBuffer;
};

extern USART kiwiUSART1;
extern USART kiwiUSART2;



extern "C" {
#endif


#ifdef __cplusplus
}
#endif
