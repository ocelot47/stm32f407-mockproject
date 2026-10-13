#ifndef INC_FLASH_LAYOUT_H_
#define INC_FLASH_LAYOUT_H_

#include <stdint.h>

/* =========================================================
 * STM32F407VG MEMORY LAYOUT
 * ========================================================= */

/* Tổng vùng Flash: 1 MB */
#define FLASH_START_ADDR          0x08000000UL
#define FLASH_END_ADDR            0x08100000UL
#define FLASH_SIZE                (1024UL * 1024UL)

/* =========================================================
 * BOOTLOADER REGION
 * Sector 0, 1, 2: 16 KB + 16 KB + 16 KB = 48 KB
 * ========================================================= */

#define BOOTLOADER_START_ADDR     0x08000000UL
#define BOOTLOADER_END_ADDR       0x0800C000UL
#define BOOTLOADER_SIZE           (48UL * 1024UL)

/* =========================================================
 * RESERVED REGION 1
 * Sector 3: 16 KB
 * ========================================================= */

#define RESERVED1_START_ADDR      0x0800C000UL
#define RESERVED1_END_ADDR        0x08010000UL
#define RESERVED1_SIZE            (16UL * 1024UL)

/* =========================================================
 * APPLICATION HEADER
 * Sector 4: 64 KB
 * ========================================================= */

#define APP_HEADER_START_ADDR     0x08010000UL
#define APP_HEADER_END_ADDR       0x08020000UL
#define APP_HEADER_SIZE           (64UL * 1024UL)

/* =========================================================
 * APPLICATION CODE
 * Sector 5, 6, 7: 128 KB x 3 = 384 KB
 * ========================================================= */

#define APP_START_ADDR            0x08020000UL
#define APP_END_ADDR              0x08080000UL
#define APP_SIZE                  (384UL * 1024UL)

/*
 * Application Vector Table
 *
 * APP_START_ADDR + 0:
 * Word 0 = Initial Main Stack Pointer
 *
 * APP_START_ADDR + 4:
 * Word 1 = Reset Handler
 */
#define MSP_OFFSET                0UL
#define RESET_HANDLER_OFFSET      4UL

/*
 * Có thể dùng các macro này để đọc trực tiếp địa chỉ:
 */
#define APP_MSP_ADDR              (APP_START_ADDR + MSP_OFFSET)
#define APP_RESET_HANDLER_ADDR    \
    (APP_START_ADDR + RESET_HANDLER_OFFSET)

/* =========================================================
 * RESERVED REGION 2
 * Sector 8: 128 KB
 * ========================================================= */

#define RESERVED2_START_ADDR      0x08080000UL
#define RESERVED2_END_ADDR        0x080A0000UL
#define RESERVED2_SIZE            (128UL * 1024UL)

/* =========================================================
 * OTA STORAGE
 * Sector 9, 10, 11: 128 KB x 3 = 384 KB
 * ========================================================= */

#define OTA_START_ADDR            0x080A0000UL
#define OTA_END_ADDR              0x08100000UL
#define OTA_SIZE                  (384UL * 1024UL)

/* =========================================================
 * SRAM
 * ========================================================= */

#define SRAM_START_ADDR           0x20000000UL
#define SRAM_END_ADDR             0x20020000UL
#define SRAM_SIZE                 (128UL * 1024UL)

#endif /* INC_FLASH_LAYOUT_H_ */
