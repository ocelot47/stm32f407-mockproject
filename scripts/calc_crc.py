import sys
import zlib

path = sys.argv[1] if len(sys.argv) > 1 else "application.bin"

with open(path, "rb") as f:
    data = f.read()

size = len(data)
crc = zlib.crc32(data) & 0xFFFFFFFF

print(f"File : {path}")
print(f"Size : {size} bytes (0x{size:08X})")
print(f"CRC32: 0x{crc:08X}")
print()
print("Copy dong nay vao header trong main.c:")
print(f"    .crc = 0x{crc:08X}UL,")