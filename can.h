#pragma once

#include <string>
#include <stdint.h>

#ifdef __cplusplus


class CANBus
{
public:
    void init();
    void deinit();

    bool send(uint8_t data[8], int retries = 0);
    bool receive();

    // should enable filtering
    void setId(uint32_t newId);

private:
    friend CANBus& theCANBus();
    CANBus()    { init(); }
    ~CANBus() { deinit(); }

    uint32_t m_id;
};

CANBus& theCANBus();

extern "C" {
#endif


#ifdef __cplusplus
}
#endif
