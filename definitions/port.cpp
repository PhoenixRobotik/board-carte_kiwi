#include "gpio_cpp.h"

#include "peripheral.h"


Port
PortA(GPIOA,
    libopencm3::RCC_GPIOA,
    libopencm3::RST_GPIOA),

PortB(GPIOB,
    libopencm3::RCC_GPIOB,
    libopencm3::RST_GPIOB),

PortC(GPIOC,
    libopencm3::RCC_GPIOC,
    libopencm3::RST_GPIOC),

PortD(GPIOD,
    libopencm3::RCC_GPIOD,
    libopencm3::RST_GPIOD),

// PortE does not exist

PortF(GPIOF,
    libopencm3::RCC_GPIOF,
    libopencm3::RST_GPIOF);
