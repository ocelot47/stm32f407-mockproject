#ifndef INC_BL_BENCH_H_
#define INC_BL_BENCH_H_
/* #define BL_BENCHMARK */

#include <stdint.h>

typedef struct
{
    uint32_t core_clk_hz;      /* SystemCoreClock lúc đo */
    uint32_t size_bytes;       /* số byte đã tính CRC */
    uint32_t crc_bitwise;
    uint32_t crc_lookup;
    uint32_t crc_hw;
    uint32_t cyc_bitwise;      /* số chu kỳ CPU */
    uint32_t cyc_lookup;
    uint32_t cyc_hw;
} bl_bench_result_t;

extern volatile bl_bench_result_t g_bench;

void bl_bench_run(void);

#endif /* INC_BL_BENCH_H_ */
