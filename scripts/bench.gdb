# bench.gdb - xuat ket qua benchmark CRC ra man hinh Console va ra file
# Cach dung: dung o breakpoint sau bl_bench_run(), roi trong Debugger Console go:
#   source D:/TaiLieu/MCU/Mock_project/scripts/bench.gdb

set logging file D:/TaiLieu/MCU/Mock_project/bench/bench_result.txt
set logging overwrite on
set logging enabled on

printf "=== CRC benchmark ===\n"
printf "core_clk_hz = %u\n", g_bench.core_clk_hz
printf "size_bytes  = %u\n", g_bench.size_bytes
printf "\n"
printf "%-10s %-12s %-12s %-12s %-12s\n", "method", "crc", "cycles", "cycles/byte", "time_us"
printf "%-10s 0x%08x   %-12u %-12.2f %-12.2f\n", "bitwise", g_bench.crc_bitwise, g_bench.cyc_bitwise, (double)g_bench.cyc_bitwise/g_bench.size_bytes, (double)g_bench.cyc_bitwise*1000000/g_bench.core_clk_hz
printf "%-10s 0x%08x   %-12u %-12.2f %-12.2f\n", "lookup", g_bench.crc_lookup, g_bench.cyc_lookup, (double)g_bench.cyc_lookup/g_bench.size_bytes, (double)g_bench.cyc_lookup*1000000/g_bench.core_clk_hz
printf "%-10s 0x%08x   %-12u %-12.2f %-12.2f\n", "hardware", g_bench.crc_hw, g_bench.cyc_hw, (double)g_bench.cyc_hw/g_bench.size_bytes, (double)g_bench.cyc_hw*1000000/g_bench.core_clk_hz
printf "\n"
printf "speedup lookup/bitwise  = %.2fx\n", (double)g_bench.cyc_bitwise/g_bench.cyc_lookup
printf "speedup hardware/bitwise = %.2fx\n", (double)g_bench.cyc_bitwise/g_bench.cyc_hw
printf "speedup hardware/lookup  = %.2fx\n", (double)g_bench.cyc_lookup/g_bench.cyc_hw

set logging enabled off
echo \nDa luu vao D:/TaiLieu/MCU/Mock_project/bench/bench_result.txt\n
