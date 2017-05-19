#include "board_kiwi.h"
#include "gpio.h"


#include <vector>

std::unique_ptr<BoardKiwi> kiwi = 0;




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

#define IDLE_STATE (1)
#define UNIT  (330)
#define SHORT (UNIT*2)
#define LONG  (UNIT*9)

void send_trame(GPIO *fake_remote, uint8_t trame)
{
    for (int i = 8; i > 0; --i)
    {
        uint8_t state = (trame >> (i-1)) & 0x01;
        fake_remote->set(false);
        kiwi->sleep_us(state == 1 ? LONG : SHORT);
        fake_remote->set(true);
        kiwi->sleep_us(state == 1 ? SHORT : LONG);
    }
    fake_remote->set(IDLE_STATE);
    kiwi->sleep_us(9300);
}

void init_remote(GPIO * fake_remote)
{
    fake_remote->set(IDLE_STATE);
    kiwi->sleep_us(9300);
}

void CAN_Rx1_interrupt_handler(void) {
    //kiwi->activeLed.toggle();
    uint32_t id;
    uint8_t message[8];
    uint8_t length;
    kiwi->canBus.receive(&id, message, &length);
}


void EXTI1_interrupt_handler(void) {
    kiwi->activeLed.set((gpio_get(PortA.Id, 1<<1) == 0));
    exti_reset_request(1<<1);
}

void EXTI0_interrupt_handler(void) {
    kiwi->activeLed.set((gpio_get(PortA.Id, 1<<0) == 0));
    exti_reset_request(1<<0);
}

// void EXTI2_interrupt_handler(void) {
//     if (exti_get_flag_status(1<<2) == 0)
//     {
//         return;
//     }
//     kiwi->activeLed.set((gpio_get(PortA.Id, 1<<2) == 0));
//     exti_reset_request(1<<2);
// }


int main(int argc, char const *argv[]) {
    kiwi = std::make_unique<BoardKiwi>();
    kiwi->statusLed.setOn();

    GPIO sensor2_p3(Pin(PortA, Pin::p1), GPIO::IOMode::input);
    GPIO sensor2_p4(Pin(PortA, Pin::p0), GPIO::IOMode::input);

    InterruptSubscriber CAN_Rx1_interrupt(&InterruptCANRx1,
        &CAN_Rx1_interrupt_handler);

    InterruptSubscriber EXTI0_interrupt(&InterruptEXTI0,
        &EXTI0_interrupt_handler);

    InterruptSubscriber EXTI1_interrupt(&InterruptEXTI1,
        &EXTI1_interrupt_handler);

    CAN_Rx1_interrupt.subscribe();
    EXTI0_interrupt.subscribe();
    EXTI1_interrupt.subscribe();

    GPIO fake_remote(Pin(PortB, Pin::p6), GPIO::IOMode::output);


    sensor2_p3.enable_irq(true, true);
    sensor2_p4.enable_irq(true, true);

    // uint8_t droite  = 0x81; // 0b10000001;
    // uint8_t avant   = 0x82; // 0b10000010;
    // uint8_t speed   = 0x83; // 0b10000011;
    // uint8_t gauche  = 0x84; // 0b10000100;
    // uint8_t UV      = 0x85; // 0b10000101;
    // uint8_t on_off  = 0x86; // 0b10000110; 
    // uint8_t loop    = 0x87; // 0b10000111; 
    uint8_t arriere = 0x88; // 0b10001000;
    // uint8_t spot    = 0x89; // 0b10001001;
    // uint8_t docking = 0x8A; // 0b10001010;
    // uint8_t _auto   = 0x8C; // 0b10001100;
    // uint8_t pause   = 0x8D; // 0b10001101;

    init_remote(&fake_remote);
    uint8_t data[] = {0,0};
    while(1)
    {
        send_trame(&fake_remote, arriere);
        kiwi->sleep_ms(200);
        data[0] = sensor2_p3.read();
        data[1] = sensor2_p4.read();
        kiwi->canBus.send(1,data,2);
        kiwi->statusLed.toggle();
    }

    while(1);
    return 0;
}
