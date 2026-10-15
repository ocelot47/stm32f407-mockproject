import struct
import sys
import zlib

APP_MAGIC = 0xA5A5C0DE
HDR_SIZE_OFF = 8             # size nằm ở offset 8 trong header
HDR_CRC_OFF = 12             # crc nằm ở offset 12 trong header
CODE_OFF = 0x08020000 - 0x08010000   # code nằm sau header 0x10000 byte

app_bin = sys.argv[1]        # application.bin (chỉ code)
full_bin = sys.argv[2]       # F407G-Application.bin (header + đệm + code)

with open(app_bin, "rb") as f:
    code = f.read()

with open(full_bin, "rb") as f:
    img = bytearray(f.read())

magic, size = struct.unpack_from("<II", img, 4)

# Kiểm tra 1: magic đúng
if magic != APP_MAGIC:
    sys.exit(f"LOI: magic = 0x{magic:08X}, mong doi 0x{APP_MAGIC:08X}")

# Kiểm tra 2: size trong header bằng độ dài application.bin
if size != len(code):
    sys.exit(f"LOI: size header = {size}, application.bin = {len(code)}")

# Kiểm tra 3: code trong file đầy đủ trùng application.bin
if img[CODE_OFF:CODE_OFF + size] != code:
    sys.exit("LOI: code trong file day du khac application.bin")

crc = zlib.crc32(code) & 0xFFFFFFFF
struct.pack_into("<I", img, HDR_CRC_OFF, crc)

with open(full_bin, "wb") as f:
    f.write(img)

print(f"Size : {size} bytes (0x{size:08X})")
print(f"CRC32: 0x{crc:08X}  -> da ghi vao offset {HDR_CRC_OFF}")