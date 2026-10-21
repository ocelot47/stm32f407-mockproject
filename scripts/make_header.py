"""Tao file header 20 byte (5 word little-endian) tu file .bin chi co code App.
Thu tu: ota_flag, magic, size, crc, version  (offset 0, 4, 8, 12, 16).
Script tu chua ham CRC kieu STM32 nen KHONG can calc_crc.py.

Vi du:
  python scripts/make_header.py release/app_blue.bin --magic 0xA5A5C0DE --ota-flag 0 --version 1 -o ota_header.bin
"""
import argparse
import struct
import sys


def crc32_stm32(data):
    """Giong khoi CRC phan cung STM32F4: word 32 bit little-endian,
    poly 0x04C11DB7, init 0xFFFFFFFF, khong dao bit, khong XOR cuoi."""
    crc = 0xFFFFFFFF
    for (word,) in struct.iter_unpack("<I", data):
        crc ^= word
        for _ in range(32):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
    return crc


def main():
    p = argparse.ArgumentParser()
    p.add_argument("bin_file", help="file .bin chi co code App (da cat header)")
    p.add_argument("--magic", required=True, help="APP_MAGIC, vi du 0xA5A5C0DE")
    p.add_argument("--ota-flag", type=int, default=0, choices=[0, 1])
    p.add_argument("--version", type=int, default=1)
    p.add_argument("-o", "--output", default="header.bin")
    a = p.parse_args()

    with open(a.bin_file, "rb") as f:
        data = f.read()
    if len(data) % 4 != 0:
        sys.exit(f"Loi: size {len(data)} khong chia het cho 4")

    # self-test: ghi 1 word 0x12345678 vao CRC->DR sau reset -> 0xDF8A8A2B
    if crc32_stm32(struct.pack("<I", 0x12345678)) != 0xDF8A8A2B:
        sys.exit("Loi: self-test CRC that bai")

    size = len(data)
    crc = crc32_stm32(data)
    magic = int(a.magic, 0)
    blob = struct.pack("<5I", a.ota_flag, magic, size, crc, a.version)
    with open(a.output, "wb") as f:
        f.write(blob)

    print(f"File     : {a.bin_file}")
    print(f"ota_flag : {a.ota_flag}")
    print(f"magic    : 0x{magic:08X}")
    print(f"size     : {size} (0x{size:08X})")
    print(f"crc      : 0x{crc:08X}")
    print(f"version  : {a.version}")
    print(f"--> {a.output} ({len(blob)} byte)")
    print("Hex      :", blob.hex(" ").upper())


if __name__ == "__main__":
    main()
