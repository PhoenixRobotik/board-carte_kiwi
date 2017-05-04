#include "board_kiwi.h"

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

int main(int argc, char const *argv[]) {
    kiwi = std::make_unique<BoardKiwi>();
    kiwi->statusLed.setOn();

    int percent = 0;
    int step = 5;
    kiwi->motorRight.enable();
    kiwi->motorLeft.enable();
    kiwi->sleep_ms(200);

    std::vector<uint8_t> data;

    bool ledsOn = true;
    int i = 0;
    while(true) {
        kiwi->motorRight.set_percent_speed(20);
        kiwi->motorLeft.set_percent_speed(20);
        // kiwi.motorRight.set_rot_per_min_speed(percent*1180/100);
        // kiwi.motorRight.set_rot_per_sec_speed(percent*1180/100/60);
        // kiwi->activeLed.set(eepromStatus ? true : ledsOn);

        data = std::vector<uint8_t>(4, i++);

        uint32_t rotation_speed_wheel1 = 1000 * kiwi->motorRight.get_rot_per_sec_speed();
        uint32_t rotation_speed_wheel2 = 1000 * kiwi->motorLeft.get_rot_per_sec_speed();
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
