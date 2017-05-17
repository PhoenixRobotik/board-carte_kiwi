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

#include "../canbus-driver/can_protocol.h"
#include "../canbus-driver/can_protocol_values.h"

void CANBus::init() {
    m_CANPeriph->enable();
    m_rx.port->enable();
    m_tx.port->enable();

    gpio_mode_setup(m_rx.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE, m_rx.number);
    gpio_mode_setup(m_tx.port->Id, GPIO_MODE_AF, GPIO_PUPD_NONE, m_tx.number);

    gpio_set_af(m_rx.port->Id, m_rx_af, m_rx.number);
    gpio_set_af(m_tx.port->Id, m_tx_af, m_tx.number);

    gpio_set_output_options(
                    m_tx.port->Id, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, m_tx.number);
    // gpio_set_output_options(
    //                 m_rx.port->Id, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, m_rx.number);


    // STM32F3 CAN on APB1 peripheral clock
    // APB1 clock period : 1/32MHz
    // baud rate prescaler = 2, so usefull clock : 16MHz
    // We want 1Mbit baud rate, so we need 16MHz/1MHz = 16tq
    // We want sample point at 87.5% (CANopen prefered value)
    // 87.5% * 16 = 14, so TS1 = 14 - 1 = 13 and TS2 = 16 - 14 = 2
    can_init(m_CANPeriph->Id,   // Interface
             false,             // Time triggered communication mode.
             true,              // Automatic bus-off management.
             false,             // Automatic wakeup mode.
             non_automatic_retransmit, // No automatic retransmission.
             false,             // Receive FIFO locked mode.
             true,              // Transmit FIFO priority.
             CAN_BTR_SJW_1TQ,   // Resynchronization time quanta jump width
             CAN_BTR_TS1_13TQ,  // Time segment 1 time quanta width
             CAN_BTR_TS2_2TQ,   // Time segment 2 time quanta width
             2,                 // Prescaler
             false,             // Loopback
             false);            // Silent


    // filter to match any standard id
    // mask bits: 0 = Don't care, 1 = mute match corresponding id bit
    can_filter_id_mask_32bit_init(
        CAN,
        0,                      // filter nr
        // id: only std id, no rtr
        FULL_IDENTIFIER(
            0,  //tx_board (don't care)
            BOARD_ID(TYPE_UNIQ, UNIQ_PILOT, ROBOT_BIGBOT), //rx_board
            0,  // msg_id (don't care)
            0), // priority (don't care)
        // mask: match only std id[10:8] = 0 (bootloader frames)
        FULL_IDENTIFIER(
            0,  //tx_board (don't care)
            BOARD_ID_MASK, //rx_board
            0,  // msg_id (don't care)
            0), // priority (don't care)
        1,                      // assign to fifo1
        true);                  // enable


    // // FIFO0 overrun interrupt enable
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_FOVIE0);
    // // FIFO0 full interrupt enable
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_FFIE0);
    // FIFO0 message pending interrupt enable
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_FMPIE0);

    // // FIFO1 overrun interrupt enable
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_FOVIE1);
    // // FIFO1 full interrupt enable
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_FFIE1);
    // FIFO1 message pending interrupt enable
    can_enable_irq(m_CANPeriph->Id, CAN_IER_FMPIE1);


    // // Transmit mailbox empty interrupt
    // can_enable_irq(m_CANPeriph->Id, CAN_IER_TMEIE);


    //m_Rx1_interrupt.provider->setPriority(0);
    //m_Rx1_interrupt.subscribe();
}

void CANBus::deinit() {

}


bool CANBus::send(uint32_t id, uint8_t* data, size_t dataSize) {
    // int32_t retries = maxSendRetries;
    int statusCAN;

    while (can_available_mailbox(m_CANPeriph->Id) == false);

    do {

        statusCAN = can_transmit(
            m_CANPeriph->Id, // canport
            id,       // can id
            false,    // extended id
            false,    // request transmit
            dataSize, // data length
            data);    // data

    } while (statusCAN < 0);


    switch(statusCAN)
    {
        case 0:
            while((CAN_TSR(m_CANPeriph->Id) & CAN_TSR_RQCP0) == 0);
            break;
        case 1:
            while((CAN_TSR(m_CANPeriph->Id) & CAN_TSR_RQCP1) == 0);
            break;
        case 2:
            while((CAN_TSR(m_CANPeriph->Id) & CAN_TSR_RQCP2) == 0);
            break;
    }

    return true;
}

bool CANBus::receive(uint32_t *id, uint8_t *message, uint8_t *length) {

    uint32_t fid;
    bool ext, rtr;

    if ((CAN_RF1R(CAN) & CAN_RF1R_FMP1_MASK) == 0) {
        return false;
    }

    can_receive(
        m_CANPeriph->Id, // canport
        1,               // fifo
        true,            // release
        id,              // can id
        &ext,            // extended id
        &rtr,            // transmission request
        &fid,            // filter id
        length,          // length
        message
    );
    return true;
}

void CANBus::CAN_Rx1_interrupt_handler(void) {

}

// // error interrupts
// void can1_sce_isr(void)
// {

// }
