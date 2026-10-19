import sys
import struct


def crc32_stm32(data):
    """Mo phong khoi CRC phan cung STM32F4:
    - doc tung word 32 bit little-endian
    - da thuc 0x04C11DB7, dich TRAI (MSB truoc)
    - init 0xFFFFFFFF, khong dao bit, khong XOR cuoi
    """
    crc = 0xFFFFFFFF
    for (word,) in struct.iter_unpack("<I", data):
        crc ^= word
        for _ in range(32):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
    return crc


def self_test():
    # Ghi 1 word 0x12345678 vao CRC->DR sau reset -> 0xDF8A8A2B
    got = crc32_stm32(struct.pack("<I", 0x12345678))
    return got == 0xDF8A8A2B, got


if __name__ == "__main__":
    ok, got = self_test()
    print(f"Self-test: {'OK' if ok else 'FAIL'} (0x{got:08X}, mong doi 0xDF8A8A2B)")
    if not ok:
        sys.exit(1)

    path = sys.argv[1] if len(sys.argv) > 1 else "application.bin"
    with open(path, "rb") as f:
        data = f.read()

    if len(data) % 4 != 0:
        print(f"Loi: size {len(data)} khong chia het cho 4")
        sys.exit(1)

    size = len(data)
    crc = crc32_stm32(data)

    print()
    print(f"File : {path}")
    print(f"Size : {size} bytes (0x{size:08X})")
    print(f"CRC  : 0x{crc:08X}  (kieu STM32 hardware)")
    print()
    print("Copy 2 dong nay vao header trong main.c:")
    print(f"    .size    = 0x{size:08X}UL,")
    print(f"    .crc     = 0x{crc:08X}UL,")