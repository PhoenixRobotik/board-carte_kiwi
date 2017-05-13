#include "board_kiwi.h"
#include "leds.h"


#include <vector>

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

#define IDLE_STATE (1)
#define UNIT  (330)
#define SHORT (UNIT*2)
#define LONG  (UNIT*9)

void send_trame(Led *fake_remote, uint8_t trame)
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

void init_remote(Led * fake_remote)
{
    fake_remote->set(IDLE_STATE);
    kiwi->sleep_us(9300);
}

int main(int argc, char const *argv[]) {
    kiwi = std::make_unique<BoardKiwi>();
    kiwi->statusLed.setOn();

    Led fake_remote(Pin(PortB, Pin::p6));

    uint8_t droite  = 0x81; // 0b10000001;
    uint8_t avant   = 0x82; // 0b10000010;
    uint8_t speed   = 0x83; // 0b10000011;
    uint8_t gauche  = 0x84; // 0b10000100;
    uint8_t UV      = 0x85; // 0b10000101;
    uint8_t on_off  = 0x86; // 0b10000110; 
    uint8_t loop    = 0x87; // 0b10000111; 
    uint8_t arriere = 0x88; // 0b10001000;
    uint8_t spot    = 0x89; // 0b10001001;
    uint8_t docking = 0x8A; // 0b10001010;
    uint8_t _auto   = 0x8C; // 0b10001100;
    uint8_t pause   = 0x8D; // 0b10001101;

    init_remote(&fake_remote);
    while(1)
    {
        send_trame(&fake_remote, avant);
        kiwi->sleep_ms(1000);
        send_trame(&fake_remote, droite);
        kiwi->sleep_ms(1000);
        send_trame(&fake_remote, avant);
        kiwi->sleep_ms(1000);
        send_trame(&fake_remote, arriere);
        kiwi->sleep_ms(1000);
        send_trame(&fake_remote, gauche);
        kiwi->sleep_ms(1000);
        send_trame(&fake_remote, arriere);
        kiwi->sleep_ms(1000);

        kiwi->statusLed.toggle();
    }

    while(1);
    return 0;
}
