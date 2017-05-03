#include "can.h"
#include "clock.h"
#include "eeprom.h"
#include "leds.h"
#include "pwm.h"
#include "hall.h"
#include "motor.h"
#include "definitions/interruptions.h"
#include "definitions/peripheral.h"

#include <vector>

class BoardKiwi
: public System {
public:
    BoardKiwi()
    : System()
    , activeLed(Pin(PortF, Pin::p0))
    , statusLed(Pin(PortF, Pin::p1))
    , canBus(&periphCAN,
            Pin(PortA, Pin::p11), AltFunction::f9,
            Pin(PortA, Pin::p12), AltFunction::f9)
    , pwm_Mot1(&Timer3, TIM_OC3,
            Pin(PortB, Pin::p0), AltFunction::f2)
    , pwm_Mot2(&Timer3, TIM_OC1,
            Pin(PortA, Pin::p6), AltFunction::f2)
    , hallsensor1(&Timer1, &InterruptTimer1_CC,
            Pin(PortA, Pin::p8),
            Pin(PortA, Pin::p9),
            Pin(PortA, Pin::p10),
            AltFunction::f6)
    , hallsensor2(&Timer2, &InterruptTimer2,
            Pin(PortA, Pin::p0),
            Pin(PortA, Pin::p1),
            Pin(PortA, Pin::p2),
            AltFunction::f1)
    , motor_right(pwm_Mot2, hallsensor2)
    , motor_left(pwm_Mot1, hallsensor1, true)
    // , usart1(USART1,
    //         Pin(PortB, Pin::p7),
    //         Pin(PortB, Pin::p6))
    // , usart2(USART2,
    //         Pin(PortB, Pin::p4),
    //         Pin(PortB, Pin::p3))
    { }

    Led activeLed, statusLed;
    CANBus canBus;

    PWM pwm_Mot1, pwm_Mot2;

    Hall hallsensor1, hallsensor2;

    WheelHubMotor motor_right, motor_left;
    // USART usart1, usart2;
};

std::unique_ptr<BoardKiwi> kiwi = 0;


extern "C" {
    // LibOpenCm3 export
    void sys_tick_handler() {
        if (kiwi)
            kiwi->systick_count++;
    }

    int get_systick() {
        return kiwi
            ? kiwi->getSysTick()
            : 0;
    }

    void delay_ms(unsigned int ms) {
        if (kiwi)
            kiwi->sleep_ms(ms);
    }
}


struct TestEepromData {
    bool on;
};
bool eepromTest() {
    DataOnEEPROM<TestEepromData> testData;
    TestEepromData data = testData.load(true);
    // Bool trim (stm32 silicon bug ?)
    data.on = (data.on & 0b1);

    TestEepromData newData;
    newData.on = !data.on;
    testData.store(newData);
    return newData.on;
}


void pwmTest() {
    // kiwi.pwm_Mot1.setDuty(150);
    // kiwi.sleep_ms(1000);
    // kiwi.pwm_Mot1.setDuty(110);
    // kiwi.sleep_ms(500);
    // kiwi.pwm_Mot1.setDuty(150);

    // kiwi.pwm_Mot2.setDuty(150);
    // kiwi.sleep_ms(1000);
    // kiwi.pwm_Mot2.setDuty(110);
    // kiwi.sleep_ms(500);
    // kiwi.pwm_Mot2.setDuty(150);
}

int main(int argc, char const *argv[]) {
    kiwi = std::make_unique<BoardKiwi>();
    kiwi->statusLed.setOn();

    int percent = 0;
    int step = 5;
    kiwi->motor_right.enable();
    kiwi->motor_left.enable();
    kiwi->sleep_ms(200);

    std::vector<uint8_t> data;

    bool ledsOn = true;
    int i = 0;
    while(true) {
        kiwi->motor_right.set_percent_speed(20);
        kiwi->motor_left.set_percent_speed(20);
        // kiwi.motor_right.set_rot_per_min_speed(percent*1180/100);
        // kiwi.motor_right.set_rot_per_sec_speed(percent*1180/100/60);
        // kiwi->activeLed.set(eepromStatus ? true : ledsOn);

        data = std::vector<uint8_t>(4, i++);

        uint32_t rotation_speed_wheel1 = 1000 * 1000/60/kiwi->hallsensor1.get_pulse_period_ms();
        uint32_t rotation_speed_wheel2 = 1000 * kiwi->motor_left.get_rot_per_sec_speed();
        kiwi->canBus.send(1, rotation_speed_wheel1);
        kiwi->canBus.send(2, rotation_speed_wheel2);

        // int32_t distance_wheel1 =
        //     // 75*3.14159/60*
        //     kiwi->hallsensor1.get_pulse_count();
        // int32_t distance_wheel2 =
        //     // 75*3.14159/60*
        //     kiwi->hallsensor2.get_pulse_count();
        // kiwi->canBus.send(3, distance_wheel1);
        // kiwi->canBus.send(4, distance_wheel2);
        // kiwi->canBus.send(5, data.data(), 4);

        kiwi->activeLed.toggle();
        kiwi->sleep_ms(ledsOn ? 100 : 100);
        if (percent == 100 or percent == -100) {
            step = -step;
        }
        percent += step;
        ledsOn = !ledsOn;
    }

    while(1);
    return 0;
}
