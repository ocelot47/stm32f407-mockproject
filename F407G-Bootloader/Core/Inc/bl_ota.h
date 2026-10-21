#ifndef INC_BL_OTA_H_
#define INC_BL_OTA_H_

#include <stdint.h>
#include "main.h"        /* HAL, LD3_Pin, LD3_GPIO_Port do CubeMX sinh */
#include "bl_flash.h"

/* LED cam LD3 (PD13): sáng liên tục khi Bootloader đang OTA */
#define OTA_LED_ON()    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET)
#define OTA_LED_OFF()   HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET)

/* Vùng OTA (nếu flash_layout.h của bạn đã có tên khác thì sửa ở đây) */
#ifndef OTA_HEADER_ADDR
#define OTA_HEADER_ADDR     0x08080000UL     /* Sector 8: header của ảnh OTA */
#endif
#ifndef OTA_CODE_ADDR
#define OTA_CODE_ADDR       0x080A0000UL     /* Sector 9-11: code của ảnh OTA */
#endif

/* Độ rộng mỗi lần ghi: BL_FLASH_X8 / BL_FLASH_X16 / BL_FLASH_X32 (board ~3.3 V dùng X32) */
#ifndef OTA_FLASH_WIDTH
#define OTA_FLASH_WIDTH     BL_FLASH_X32
#endif

/* Chạy OTA: copy code + header từ vùng OTA lên vùng App.
 * Trả về BL_FLASH_OK nếu thành công. Dù thành công hay lỗi, hàm LUÔN trả về
 * để main() đi tiếp xuống validate_app(). */
bl_flash_status_t run_ota(void);

#ifdef BL_BENCHMARK_OTA
typedef struct
{
    uint32_t core_clk_hz;
    uint32_t size_bytes;
    uint32_t width_bytes;       /* 1, 2 hoặc 4 */
    uint32_t cyc_clear_flag;    /* đọc header + clear cờ */
    uint32_t cyc_erase_app;     /* xoá sector vùng App */
    uint32_t cyc_write_app;     /* ghi code + verify */
    uint32_t cyc_erase_hdr;     /* xoá Sector 4 */
    uint32_t cyc_write_hdr;     /* ghi header + verify */
    uint32_t cyc_total;
} bl_ota_bench_t;

extern volatile bl_ota_bench_t g_bench_ota;
#endif

#endif /* INC_BL_OTA_H_ */
