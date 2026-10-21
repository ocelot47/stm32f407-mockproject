# bench_ota.gdb - in ket qua benchmark OTA ra Console va ra file
# Dung o breakpoint ngay SAU run_ota() trong main() cua Bootloader, roi go:
#   source D:/TaiLieu/MCU/Mock_project/scripts/bench_ota.gdb
# Doi ten file ket qua truoc moi lan do (bench_result_ota.txt se bi ghi de).

set logging file D:/TaiLieu/MCU/Mock_project/bench/bench_result_ota.txt
set logging overwrite on
set logging enabled on

printf "=== OTA benchmark ===\n"
printf "core_clk_hz = %u\n", g_bench_ota.core_clk_hz
printf "size_bytes  = %u\n", g_bench_ota.size_bytes
printf "width       = x%u (byte moi lan ghi)\n", g_bench_ota.width_bytes * 8
printf "\n"
printf "%-14s %-12s %-12s\n", "phase", "cycles", "ms"
printf "%-14s %-12u %-12.3f\n", "clear_flag",  g_bench_ota.cyc_clear_flag, (double)g_bench_ota.cyc_clear_flag*1000/g_bench_ota.core_clk_hz
printf "%-14s %-12u %-12.3f\n", "erase_app",   g_bench_ota.cyc_erase_app,  (double)g_bench_ota.cyc_erase_app*1000/g_bench_ota.core_clk_hz
printf "%-14s %-12u %-12.3f\n", "write_app",   g_bench_ota.cyc_write_app,  (double)g_bench_ota.cyc_write_app*1000/g_bench_ota.core_clk_hz
printf "%-14s %-12u %-12.3f\n", "erase_header",g_bench_ota.cyc_erase_hdr,  (double)g_bench_ota.cyc_erase_hdr*1000/g_bench_ota.core_clk_hz
printf "%-14s %-12u %-12.3f\n", "write_header",g_bench_ota.cyc_write_hdr,  (double)g_bench_ota.cyc_write_hdr*1000/g_bench_ota.core_clk_hz
printf "%-14s %-12u %-12.3f\n", "TOTAL",       g_bench_ota.cyc_total,      (double)g_bench_ota.cyc_total*1000/g_bench_ota.core_clk_hz

set logging enabled off
echo \nDa luu vao D:/TaiLieu/MCU/Mock_project/bench/bench_result_ota.txt\n
