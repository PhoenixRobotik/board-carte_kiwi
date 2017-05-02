#include "gpio_cpp.h"

#include "peripheral.h"


Port
PortA(GPIOA,
    RCC_GPIOA,
    RST_GPIOA),

PortB(GPIOB,
    RCC_GPIOB,
    RST_GPIOB),

PortC(GPIOC,
    RCC_GPIOC,
    RST_GPIOC),

PortD(GPIOD,
    RCC_GPIOD,
    RST_GPIOD),

// PortE does not exist

PortF(GPIOF,
    RCC_GPIOF,
    RST_GPIOF);
