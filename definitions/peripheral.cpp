#include "peripheral.h"

#include <libopencm3/stm32/can.h>

Peripheral
periphCAN(
    CAN,
    RCC_CAN,
    RST_CAN);
