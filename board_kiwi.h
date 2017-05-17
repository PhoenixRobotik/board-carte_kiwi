#pragma once

#include "can.h"
#include "clock.h"
#include "eeprom.h"
#include "leds.h"
#include "pwm.h"
#include "hall.h"
#include "motor.h"
#include "definitions/interruptions.h"
#include "definitions/peripheral.h"

class BoardKiwi
: public System {
public:
    BoardKiwi()
    : System()
    , activeLed(Pin(PortF, Pin::p0))
    , statusLed(Pin(PortF, Pin::p1))
    , canBus(&periphCAN, // &InterruptCANRx1,
            Pin(PortA, Pin::p11), AltFunction::f9,
            Pin(PortA, Pin::p12), AltFunction::f9)
    , pwmMotor1(new PWM(&Timer3, TIM_OC3,
            Pin(PortB, Pin::p0), AltFunction::f2))
    , pwmMotor2(new PWM(&Timer3, TIM_OC1,
            Pin(PortA, Pin::p6), AltFunction::f2))
    , hallSensor1(new Hall(&Timer1, &InterruptTimer1_CC,
            Pin(PortA, Pin::p8),
            Pin(PortA, Pin::p9),
            Pin(PortA, Pin::p10),
            AltFunction::f6))
    , hallSensor2(new Hall(&Timer2, &InterruptTimer2,
            Pin(PortA, Pin::p0),
            Pin(PortA, Pin::p1),
            Pin(PortA, Pin::p2),
            AltFunction::f1))
    , motorRight(pwmMotor1, hallSensor1, true)
    , motorLeft(pwmMotor2, hallSensor2)
    // , usart1(USART1,
    //         Pin(PortB, Pin::p7),
    //         Pin(PortB, Pin::p6))
    // , usart2(USART2,
    //         Pin(PortB, Pin::p4),
    //         Pin(PortB, Pin::p3))
    { }

    Led activeLed, statusLed;
    CANBus canBus;

    std::shared_ptr<PWM> pwmMotor1, pwmMotor2;

    std::shared_ptr<Hall> hallSensor1, hallSensor2;

    WheelHubMotor motorRight, motorLeft;
    // USART usart1, usart2;
};

extern
std::unique_ptr<BoardKiwi> kiwi;

