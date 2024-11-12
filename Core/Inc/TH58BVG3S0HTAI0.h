#ifndef NAND_FLASH_H
#define NAND_FLASH_H

#include <stdint.h>
#include "main.h"  


// NAND flash commands Reference taken from the datasheet
#define CMD_READ                    0x00
#define CMD_READ_CONFIRM            0x30
#define CMD_PAGE_PROGRAM            0x80
#define CMD_PROGRAM_CONFIRM         0x10
#define PAGE_SIZE                   4224            // Total size per page, including main and spare area
#define DATA_PORT                   GPIOB           // Adjust to the actual port used for data
#define CLE_PORT                    GPIOC           // Command Latch Enable port
#define CLE_PIN                     GPIO_PIN_5
#define WE_PORT                     GPIOC           // Write Enable port
#define WE_PIN                      GPIO_PIN_6
#define RE_PORT                     GPIOC           // Read Enable port
#define RE_PIN                      GPIO_PIN_7


uint8_t data[PAGESIZE] = {0};
uint8_t storeData[PAGESIZE] = {0};
uint32_t page_address = (1 << 6) | 0; // this will write to block 0 page 1


void nand_read_page(uint32_t page_address, uint8_t *data_buffer);
void nand_write_page(uint32_t page_address, const uint8_t *data);


#endif // NAND_FLASH_H
