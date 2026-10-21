#include "bl_ota.h"
#include "app_header.h"      /* app_header_t, APP_MAGIC */
#include "flash_layout.h"    /* APP_HEADER_START_ADDR, APP_START_ADDR, APP_SIZE */
#include <string.h>

#define APP_FIRST_SECTOR    FLASH_SECTOR_5            /* Sector 5-7: code App */
#define APP_SECTOR_BYTES    (128UL * 1024UL)
#define HEADER_SECTOR       FLASH_SECTOR_4            /* Sector 4: header App */

/* ---------- đo thời gian (chỉ bật khi định nghĩa BL_BENCHMARK_OTA) ---------- */
#ifdef BL_BENCHMARK_OTA
volatile bl_ota_bench_t g_bench_ota;
static uint32_t s_t_begin;
static uint32_t s_t_mark;
#define BENCH_BEGIN()                                                         \
    do {                                                                      \
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;                       \
        DWT->CYCCNT = 0U;                                                     \
        DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;                                 \
        s_t_begin = DWT->CYCCNT;                                              \
        s_t_mark  = s_t_begin;                                                \
        g_bench_ota.core_clk_hz = SystemCoreClock;                            \
        g_bench_ota.width_bytes = (uint32_t)OTA_FLASH_WIDTH;                  \
    } while (0)
#define BENCH_LAP(field)                                                      \
    do { uint32_t n_ = DWT->CYCCNT; g_bench_ota.field = n_ - s_t_mark; s_t_mark = n_; } while (0)
#define BENCH_END()   do { g_bench_ota.cyc_total = DWT->CYCCNT - s_t_begin; } while (0)
#else
#define BENCH_BEGIN()   do { } while (0)
#define BENCH_LAP(f)    do { } while (0)
#define BENCH_END()     do { } while (0)
#endif

bl_flash_status_t run_ota(void)
{
    bl_flash_status_t st;
    app_header_t cur;        /* header App hiện tại (đọc vào RAM trước khi xoá Sector 4) */
    app_header_t ota;        /* header ảnh OTA */
    app_header_t fresh;      /* header mới sẽ ghi */
    const uint32_t zero = 0U;
    const uint8_t *ota_code = (const uint8_t *)BL_FLASH_PTR(OTA_CODE_ADDR);
    uint32_t nsec;
    uint32_t i;

    OTA_LED_ON();
    BENCH_BEGIN();

    memcpy(&cur, BL_FLASH_PTR(APP_HEADER_START_ADDR), sizeof cur);
    memcpy(&ota, BL_FLASH_PTR(OTA_HEADER_ADDR),       sizeof ota);

    /* 1. Clear ota_flag: ghi word 0 đè lên (bit 1 -> 0 nên KHÔNG cần xoá sector) */
    st = bl_flash_write(sizeof zero, (const uint8_t *)&zero, APP_HEADER_START_ADDR, OTA_FLASH_WIDTH);
    BENCH_LAP(cyc_clear_flag);
    if (st != BL_FLASH_OK) { goto out; }

    /* 2. Kiểm tra header OTA trước khi xoá bất cứ thứ gì của App */
    if ((ota.magic != APP_MAGIC) || (ota.size < 8U) || (ota.size > APP_SIZE) || ((ota.size & 3U) != 0U))
    {
        st = BL_FLASH_ERR_PARAM;
        goto out;
    }
#ifdef BL_BENCHMARK_OTA
    g_bench_ota.size_bytes = ota.size;
#endif

    /* 3. Dựng header mới trong RAM: giữ magic/size/crc của ảnh OTA,
     *    version = version App hiện tại + 1, ota_flag = 0 */
    fresh          = ota;
    fresh.ota_flag = 0U;
    fresh.version  = (cur.version == 0xFFFFFFFFUL) ? 1U : (cur.version + 1U);

    /* 4. Xoá đúng số sector App cần dùng, rồi chép đúng ota.size byte */
    nsec = (ota.size + APP_SECTOR_BYTES - 1U) / APP_SECTOR_BYTES;
    for (i = 0U; i < nsec; i++)
    {
        st = bl_flash_erase_sector(APP_FIRST_SECTOR + i, OTA_FLASH_WIDTH);
        if (st != BL_FLASH_OK) { goto out; }
    }
    BENCH_LAP(cyc_erase_app);

    st = bl_flash_write(ota.size, ota_code, APP_START_ADDR, OTA_FLASH_WIDTH);
    if (st == BL_FLASH_OK) { st = bl_flash_verify(ota.size, ota_code, APP_START_ADDR); }
    BENCH_LAP(cyc_write_app);
    if (st != BL_FLASH_OK) { goto out; }

    /* 5. Xoá Sector 4 rồi ghi header mới */
    st = bl_flash_erase_sector(HEADER_SECTOR, OTA_FLASH_WIDTH);
    BENCH_LAP(cyc_erase_hdr);
    if (st != BL_FLASH_OK) { goto out; }

    st = bl_flash_write(sizeof fresh, (const uint8_t *)&fresh, APP_HEADER_START_ADDR, OTA_FLASH_WIDTH);
    if (st == BL_FLASH_OK) { st = bl_flash_verify(sizeof fresh, (const uint8_t *)&fresh, APP_HEADER_START_ADDR); }
    BENCH_LAP(cyc_write_hdr);

out:
    BENCH_END();
    OTA_LED_OFF();           /* điểm thoát duy nhất: LED cam luôn được tắt */
    return st;
}
