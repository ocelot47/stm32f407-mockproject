#ifndef INC_APP_HEADER_H_
#define INC_APP_HEADER_H_

#include <stdint.h>

/* Chữ ký nhận diện App do mình tạo.
 * Tránh 0x00000000 và 0xFFFFFFFF (flash trống đọc ra 0xFFFFFFFF). */
#define APP_MAGIC   0xA5A5C0DEUL

/* Application Header: 5 word x 4 byte = 20 byte */
typedef struct
{
    uint32_t ota_flag;   /* Part 4-5: chưa dùng, để 0 */
    uint32_t magic;      /* Part 2: phải bằng APP_MAGIC */
    uint32_t size;       /* Part 2: kích thước App (byte) */
    uint32_t crc;        /* Part 3: chưa làm, để 0 */
    uint32_t version;    /* Part 5: chưa dùng, để 0 */
} app_header_t;

/* Báo lỗi lúc build nếu struct không đúng 20 byte */
_Static_assert(sizeof(app_header_t) == 20U, "app_header_t must be 5 words");

#endif /* INC_APP_HEADER_H_ */
