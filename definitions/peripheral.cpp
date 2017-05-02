#include "peripheral.h"

namespace libopencm3 {
    #include <libopencm3/stm32/can.h>
}

Peripheral periphCAN = { CAN, libopencm3::RCC_CAN, libopencm3::RST_CAN };
