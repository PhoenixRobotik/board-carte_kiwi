#include "can.h"

#include "leds.h"
#include "clock.h"
#include "config_macros.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>


CANBus& theCANBus() {
    static CANBus theOneAndOnlyCANBus;
    return theOneAndOnlyCANBus;
}

const Port::Number CANPort  = Port::pA;
const Pin ::Number CANPinRx = Pin ::p11;
const Pin ::Number CANPinTx = Pin ::p12;

void CANBus::init() {
    rcc_periph_clock_enable(RCC_CAN);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(CANPort, GPIO_MODE_AF, GPIO_PUPD_NONE,
                    CANPinRx | CANPinTx);
    gpio_set_output_options(
                    CANPort, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ,
                    CANPinRx | CANPinTx);
    gpio_set_af(    CANPort, GPIO_AF9,
                    CANPinRx | CANPinTx);

    // STM32F3 CAN on 36MHz configured APB1 peripheral clock
    // 36MHz / 2 -> 18MHz
    // 18MHz / (1tq + 10tq + 7tq) = 1MHz => 1Mbit
    can_init(CAN,               // Interface
             false,             // Time triggered communication mode.
             true,              // Automatic bus-off management.
             false,             // Automatic wakeup mode.
             true,              // No automatic retransmission.
             false,             // Receive FIFO locked mode.
             false,             // Transmit FIFO priority.
             CAN_BTR_SJW_1TQ,   // Resynchronization time quanta jump width
             CAN_BTR_TS1_10TQ,  // Time segment 1 time quanta width
             CAN_BTR_TS2_7TQ,   // Time segment 2 time quanta width
             2,                 // Prescaler
             false,             // Loopback
             false);            // Silent


    // filter to match any standard id
    // mask bits: 0 = Don't care, 1 = mute match corresponding id bit
    can_filter_id_mask_32bit_init(
        CAN,
        0,                      // filter nr
        0,                      // id: only std id, no rtr
        6 | (7<<29),            // mask: match only std id[10:8] = 0 (bootloader frames)
        0,                      // assign to fifo0
        true);                  // enable

}
void CANBus::deinit() {

}


bool CANBus::send(uint8_t data[8], int retries) {
    m_id = 0;

    do {
        volatile int statusCAN = can_transmit(
            CAN,    // canport
            m_id,   // can id
            false,  // extended id
            false,  // request transmit
            8,      // data length
            data);  // data

        while((CAN_TSR(CAN) & CAN_TSR_RQCP0) == 0);

        statusLed.toggle();
        return true;



        // while((CAN_TSR(CAN) & CAN_TSR_RQCP0) == 0 && false) {
        //     statusLed.toggle();
        //     theSystem().sleep_ms(20);
        // }
        // activeLed.set(true);

        // if ((CAN_TSR(CAN) & CAN_TSR_TXOK0)) {
        //     return true;    // can ok
        // }
    } while (retries-- > 0);

    return false;
}
bool CANBus::receive() {
    return false;
}


bool can_interface_read_message(uint32_t *id, uint8_t *message, uint8_t *length, uint32_t retries)
{
    uint32_t fid;
    uint8_t len;
    bool ext, rtr;

    while(retries-- != 0 && (CAN_RF0R(CAN) & CAN_RF0R_FMP0_MASK) == 0);

    if ((CAN_RF0R(CAN) & CAN_RF0R_FMP0_MASK) == 0) {
        return false;
    }

    can_receive(
        CAN,        // canport
        0,          // fifo
        true,       // release
        id,         // can id
        &ext,       // extended id
        &rtr,       // transmission request
        &fid,       // filter id
        &len,       // length
        message
    );

    *length = len;

    return true;
}