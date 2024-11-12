#include "TH58BVG3S0HTAI0.h"

/*
    The below function will read data from the NAND flash memory
*/
void nand_read_page(uint32_t page_address, uint8_t *data_buffer) {
    // Issue the read command (0x00) for the page read operation
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_SET);   // Command latch enable
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);   // Write enable low
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, CMD_READ);     // Issue READ command
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);     // Write enable high
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_RESET); // Command latch disable

    // below data is taken from datsheet

    // First cycle: Column address (CA0-CA7)
    uint8_t col_address1 = 0x00;  
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, col_address1);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Second cycle: Column address (CA8-CA12)
    uint8_t col_address2 = 0x00;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, col_address2);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Third cycle: Page address (PA0-PA7)
    uint8_t page_addr1 = (page_address & 0xFF);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr1);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Fourth cycle: Page address (PA8-PA15)
    uint8_t page_addr2 = (page_address >> 8) & 0xFF;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr2);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Fifth cycle: Page address (PA16-PA17, remaining bits)
    uint8_t page_addr3 = (page_address >> 16) & 0x03;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr3);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Issue read confirm command (0x30) if required
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, CMD_READ_CONFIRM);  
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_RESET);

    // Start reading data into the buffer by toggling RE (Read Enable)
    for (int i = 0; i < PAGE_SIZE; i++) {
        HAL_GPIO_WritePin(RE_PORT, RE_PIN, GPIO_PIN_RESET);  
        data_buffer[i] = HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN);  
        HAL_GPIO_WritePin(RE_PORT, RE_PIN, GPIO_PIN_SET);    
    }
}



/*
    The below function will write data to the NAND flash memory
*/
void nand_write_page(uint32_t page_address, uint8_t *data_buffer) {
    uint8_t data[PAGE_SIZE];

    // Step 1: Issue the page program command (0x80)
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_SET);    // Command latch enable
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);    // Write enable low
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, CMD_PAGE_PROGRAM);  // Send program command (0x80)
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);      // Write enable high
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_RESET);  // Command latch disable

    // Step 2: Send address cycles (five cycles as per the datasheet)

    // First cycle: Column address (CA0-CA7)
    uint8_t col_address1 = 0x00;  // Assuming starting from byte 0 in the page
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, col_address1);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Second cycle: Column address (CA8-CA12)
    uint8_t col_address2 = 0x00;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, col_address2);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Third cycle: Page address (PA0-PA7)
    uint8_t page_addr1 = (page_address & 0xFF);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr1);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Fourth cycle: Page address (PA8-PA15)
    uint8_t page_addr2 = (page_address >> 8) & 0xFF;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr2);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Fifth cycle: Page address (PA16-PA17, remaining bits)
    uint8_t page_addr3 = (page_address >> 16) & 0x03;
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, page_addr3);
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);

    // Step 3: Send the data
    for (int i = 0; i < PAGE_SIZE; i++) {
        HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);  
        HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, data[i]);     
        HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);   
    }

    // Step 4: Issue the program confirm command (0x10)
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_SET);    
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_RESET);    
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, CMD_PROGRAM_CONFIRM);  
    HAL_GPIO_WritePin(WE_PORT, WE_PIN, GPIO_PIN_SET);      
    HAL_GPIO_WritePin(CLE_PORT, CLE_PIN, GPIO_PIN_RESET);  
}
