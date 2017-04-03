#pragma once

#include <stdint.h>

// Stm32f303k8 specific !
#define FLASH_BASE                  (0x08000000)
#define BASE_ADDRESS                (0x0800F000)
#define FLASH_PAGE_NUM_MAX          (16)
#define FLASH_PAGE_SIZE             (0x400)
#define FLASH_WRONG_DATA_WRITTEN    (0x80)

#ifdef __cplusplus
extern "C" {
#endif

uint32_t flash_write_data(uint32_t start_address, uint32_t *input_data, uint16_t num_elements);
void flash_read_data(uint32_t start_address, uint32_t *output_data, uint16_t num_elements);


#ifdef __cplusplus
}

template <typename DataStructure>
class DataOnEEPROM
{
public:
    // Private constructor because System is singleton.
    DataOnEEPROM()
    : readOnce(false)
    , m_dataSyncedWithEEPROM()
    { }
    ~DataOnEEPROM(){ }

    void store(DataStructure const& myStructure) {
        // Replace the current with the new (copy constructor)
        m_dataSyncedWithEEPROM = myStructure;
        // Flash the new !
        storeMe();
    }
    const DataStructure& load(bool force = false) {
        if (!readOnce || force) {
            readOnce = true;
            loadMe();
        }
        return m_dataSyncedWithEEPROM;
    }

    const DataStructure& get() {
        return m_dataSyncedWithEEPROM;
    }

private:
    bool readOnce;
    DataStructure m_dataSyncedWithEEPROM;

    void loadMe() {
        flash_read_data (BASE_ADDRESS, (uint32_t*) &m_dataSyncedWithEEPROM, (sizeof(DataStructure) + 4 - 1)/ 4);
    }
    void storeMe() {
        flash_write_data(BASE_ADDRESS, (uint32_t*) &m_dataSyncedWithEEPROM, (sizeof(DataStructure) + 4 - 1)/ 4);
    }
};

#endif
