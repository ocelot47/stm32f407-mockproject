#ifndef INC_BL_CRC_H_
#define INC_BL_CRC_H_

#include <stdint.h>

/* CRC32 chuẩn (giống zlib.crc32 của Python) */
uint32_t bl_crc32(const uint8_t *data, uint32_t length);

uint32_t bl_crc32_bitwise(const uint32_t *data, uint32_t len);

#endif /* INC_BL_CRC_H_ */
