#include "can.h"
#include "clock.h"
#include "eeprom.h"
#include "leds.h"
#include "pwm.h"

#include <vector>

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
    moteur1.setDuty(150);
    theSystem().sleep_ms(1000);
    moteur1.setDuty(110);
    theSystem().sleep_ms(500);
    moteur1.setDuty(150);

    moteur2.setDuty(150);
    theSystem().sleep_ms(1000);
    moteur2.setDuty(110);
    theSystem().sleep_ms(500);
    moteur2.setDuty(150);
}

int main(int argc, char const *argv[]) {
    bool eepromStatus = false; // eepromTest();

    // Those are equivalent
    moteur1.setDuty     ( 150);
    moteur1.setPercent  (  75);
    moteur1.setMicrosec (1500);

    moteur2.setMicrosec (1500);
    // pwmTest();

    theCANBus().init();
    std::vector<uint8_t> data;

    bool ledsOn = true;
    int i = 0;
    while(true) {
        activeLed.set(eepromStatus ? true : ledsOn);

        data = std::vector<uint8_t>(8, i++);
        theCANBus().send(data.data());

        // statusLed.set(eepromStatus);
        delay_ms(ledsOn ? 100 : 100);
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
