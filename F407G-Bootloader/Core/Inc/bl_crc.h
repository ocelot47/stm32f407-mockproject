#ifndef INC_BL_CRC_H_
#define INC_BL_CRC_H_

#include <stdint.h>

/*
 * CRC32 kiểu khối CRC phần cứng STM32F4 (khớp scripts/calc_crc.py):
 *   - đa thức 0x04C11DB7, dịch TRÁI (MSB trước)
 *   - init 0xFFFFFFFF, không đảo bit, không XOR cuối
 *   - xử lý từng word 32 bit
 *
 * data : địa chỉ bắt đầu, phải căn 4 byte
 * len  : số BYTE (phải chia hết cho 4)
 */
uint32_t bl_crc32_bitwise(const uint32_t *data, uint32_t len);
uint32_t bl_crc32_lookup(const uint32_t *data, uint32_t len);
uint32_t bl_crc32_hw(const uint32_t *data, uint32_t len);

#endif /* INC_BL_CRC_H_ */
