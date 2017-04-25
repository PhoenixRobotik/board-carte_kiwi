#pragma once

#include <string>
#include <stdint.h>

#ifdef __cplusplus


class CANBus
{
public:
    static const int32_t maxSendRetries = 2500;

    void init();
    void deinit();

    bool send(uint32_t id, uint8_t* data, size_t dataSize);

    template<typename T>
    bool send(uint32_t id, T data) {
        static_assert(sizeof(T) <= 8);
        return send(id, (uint8_t*)&data, sizeof(T));
    }
    template<typename T>
    bool send(uint32_t id, T* data) {
        static_assert(sizeof(T) <= 8);
        return send(id, (uint8_t*)data, sizeof(T));
    }

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
