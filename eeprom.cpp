#include "eeprom.h"

// #include "hardware.h"
#include "leds.h"

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/f3/memorymap.h>

extern "C" {



// Currently only works if all the data is in the same page
uint32_t flash_write_data(uint32_t start_address, uint32_t *input_data, uint16_t num_elements) {
    // Check if addresses are in the flash
    if (start_address                   <   FLASH_BASE
    && (start_address + num_elements/4) >= (FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NUM_MAX) )
        return 1;

    // calculate the page address
    uint32_t page_address = start_address - (start_address % FLASH_PAGE_SIZE);

    // Status flags
    volatile uint32_t flash_status = 0;

    // Ask the STM32 to unlock the flash for write
    flash_unlock();

    // Erasing the page is needed
    flash_erase_page(page_address);

    if((flash_status = flash_get_status_flags()) != FLASH_SR_EOP)
        return flash_status;

    // programming flash memory
    for (int i = 0; i < num_elements; ++i) {
        flash_program_word(start_address + i*4, *((uint32_t*)input_data + i));

        if((flash_status = flash_get_status_flags()) != FLASH_SR_EOP)
            return flash_status;

#       ifdef DEBUG_EEPROM
        // verify if correct data is programmed
        if (*((uint32_t*)start_address + i)
         != *((uint32_t*)input_data    + i) )
            return FLASH_WRONG_DATA_WRITTEN;
#       endif
    }
    return 0;
}

void flash_read_data(uint32_t start_address, uint32_t *output_data, uint16_t num_elements) {
    for (uint16_t i = 0; i < num_elements; ++i)
        *((uint32_t*)output_data + i) = *((uint32_t*)start_address + i);
}

}
