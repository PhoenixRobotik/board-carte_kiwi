#include "can.h"
#include "clock.h"
#include "eeprom.h"
#include "leds.h"
#include "pwm.h"
#include "hall.h"
#include "motor.h"

#include <vector>

class BoardKiwi
: public System {
public:
    BoardKiwi()
    : System()
    , activeLed(Port::pF, Pin::p0)
    , statusLed(Port::pF, Pin::p1)
    , pwm_Mot1(Port::pB, Pin::p0,
            Timer3,
            AltFunction::f2,
            TIM_OC3)
    , pwm_Mot2(Port::pA, Pin::p6,
            Timer3,
            AltFunction::f2,
            TIM_OC1)
    , motor_right(pwm_Mot1, hallsensor1)
    , motor_left(pwm_Mot2, hallsensor2)
    // , usart1(USART1, Port::pB, Pin::p7, Port::pB, Pin::p6)
    // , usart2(USART2, Port::pB, Pin::p4, Port::pB, Pin::p3)
    { }

    Led activeLed, statusLed;

    PWM pwm_Mot1, pwm_Mot2;

    WheelHubMotor motor_right, motor_left;
    // USART usart1;
    // USART usart2;

} kiwi;

extern "C" {
    // LibOpenCm3 export
    void sys_tick_handler() {
        kiwi.systick_count++;
    }

    int get_systick() {
        return kiwi.getSysTick();
    }

    void delay_ms(unsigned int ms) {
        return kiwi.sleep_ms(ms);
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
    kiwi.pwm_Mot1.setDuty(150);
    kiwi.sleep_ms(1000);
    kiwi.pwm_Mot1.setDuty(110);
    kiwi.sleep_ms(500);
    kiwi.pwm_Mot1.setDuty(150);

    kiwi.pwm_Mot2.setDuty(150);
    kiwi.sleep_ms(1000);
    kiwi.pwm_Mot2.setDuty(110);
    kiwi.sleep_ms(500);
    kiwi.pwm_Mot2.setDuty(150);
}

int main(int argc, char const *argv[]) {
    // BoardKiwi kiwi;

    bool eepromStatus = false; // eepromTest();

    // Those are equivalent
    // kiwi.pwm_Mot1.setDuty     ( 150);
    // kiwi.pwm_Mot1.setPercent  (  75);
    // kiwi.pwm_Mot1.setMicrosec (1500);

    int percent = 0;
    int step = 5;
    kiwi.motor_right.enable();
    //kiwi.pwm_Mot1.setMicrosec (1500);
    kiwi.sleep_ms(200);
    // pwmTest();

    theCANBus().init();
    std::vector<uint8_t> data;

    bool ledsOn = true;
    int i = 0;
    while(true) {
        //kiwi.pwm_Mot1.setMicrosec (1520);
        kiwi.motor_right.set_percent_speed(percent);
        //kiwi.motor_right.set_rot_per_min_speed(percent*1180/100);
        //kiwi.motor_right.set_rot_per_sec_speed(percent*1180/100/60);
        kiwi.activeLed.set(eepromStatus ? true : ledsOn);

        data = std::vector<uint8_t>(4, i++);
        uint32_t rotation_speed_wheel1 = 1000*kiwi.motor_right.get_rot_per_sec_speed();
        // uint32_t rotation_speed_wheel2 = /*75*3.14159**/1000/(60*hallsensor2.get_pulse_period_ms());
        theCANBus().send(1, rotation_speed_wheel1);
        // theCANBus().send(2, rotation_speed_wheel2);
        // int32_t distance_wheel1 = /*75*3.14159/60**/hallsensor1.get_pulse_count();
        // int32_t distance_wheel2 = /*75*3.14159/60**/hallsensor2.get_pulse_count();
        //theCANBus().send(3, distance_wheel1);
        //theCANBus().send(4, distance_wheel2);
        //theCANBus().send(5, data.data(), 4);

        // kiwi.statusLed.set(eepromStatus);
        kiwi.sleep_ms(ledsOn ? 100 : 100);
        if (percent == 100 or percent == -100) {
            step = -step;
        }
        percent += step;
        ledsOn = !ledsOn;
    }

    while(1);
    return 0;
}


// static void clock_setup(void)
// {
//     /* Enable clocks for USART2. */
//     rcc_periph_clock_enable(RCC_USART2);
// }

// static void usart_setup(void)
// {
//     /* Setup USART2 parameters. */
//     usart_set_baudrate(USART2, 38400);
//     usart_set_databits(USART2, 8);
//     usart_set_stopbits(USART2, USART_STOPBITS_1);
//     usart_set_mode(USART2, USART_MODE_TX);
//     usart_set_parity(USART2, USART_PARITY_NONE);
//     usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

//     /* Finally enable the USART. */
//     usart_enable(USART2);
// }

// static void gpio_setup(void)
// {
//     /* Setup GPIO pins for USART2 transmit. */
//     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

//     /* Setup USART2 TX pin as alternate function. */
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
// }
