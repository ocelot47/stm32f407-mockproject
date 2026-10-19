#include "bl_bench.h"
#include "bl_crc.h"
#include "flash_layout.h"
#include "main.h"

volatile bl_bench_result_t g_bench;

static void dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;   /* bật khối DWT */
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;             /* bắt đầu đếm */
}

void bl_bench_run(void)
{
    const uint32_t *app = (const uint32_t *)APP_START_ADDR;
    const volatile uint32_t *hdr_size = (const volatile uint32_t *)(APP_HEADER_START_ADDR + 8U);
    uint32_t size = *hdr_size;                         /* size trong header (offset 8) */
    uint32_t t0;

    dwt_init();

    g_bench.core_clk_hz = SystemCoreClock;
    g_bench.size_bytes  = size;

    t0 = DWT->CYCCNT;
    g_bench.crc_bitwise = bl_crc32_bitwise(app, size);
    g_bench.cyc_bitwise = DWT->CYCCNT - t0;

    t0 = DWT->CYCCNT;
    g_bench.crc_lookup = bl_crc32_lookup(app, size);
    g_bench.cyc_lookup = DWT->CYCCNT - t0;

    t0 = DWT->CYCCNT;
    g_bench.crc_hw = bl_crc32_hw(app, size);
    g_bench.cyc_hw = DWT->CYCCNT - t0;
}
