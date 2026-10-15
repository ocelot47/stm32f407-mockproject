#include "bl_crc.h"

#define CRC32_POLY_REFLECTED   0xEDB88320UL
#define CRC32_INIT             0xFFFFFFFFUL
#define CRC32_XOR_OUT          0xFFFFFFFFUL

uint32_t bl_crc32(const uint8_t *data, uint32_t length)
{
    uint32_t crc = CRC32_INIT;

    for (uint32_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint32_t bit = 0; bit < 8U; bit++)
        {
            if (crc & 1UL)
            {
                crc = (crc >> 1) ^ CRC32_POLY_REFLECTED;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc ^ CRC32_XOR_OUT;
}
