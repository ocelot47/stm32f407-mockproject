#ifndef INC_BL_FLASH_H_
#define INC_BL_FLASH_H_

#include <stdint.h>

/* Đọc nội dung flash theo địa chỉ. Máy tính (unit test) có thể định nghĩa lại macro này. */
#ifndef BL_FLASH_PTR
#define BL_FLASH_PTR(addr)  ((const void *)(uintptr_t)(addr))
#endif

/* Độ rộng mỗi lần ghi = số byte. Cũng là PSIZE khi xoá (xem RM0090 mục 3.6.2). */
typedef enum
{
    BL_FLASH_X8  = 1U,   /* byte       - nguồn 1.8 - 3.6 V */
    BL_FLASH_X16 = 2U,   /* half word  - nguồn 2.1 - 3.6 V */
    BL_FLASH_X32 = 4U    /* word       - nguồn 2.7 - 3.6 V (board ~3.3 V dùng cái này) */
} bl_flash_width_t;

typedef enum
{
    BL_FLASH_OK = 0,
    BL_FLASH_ERR_PARAM,    /* tham số sai, KHÔNG đụng vào flash */
    BL_FLASH_ERR_ERASE,
    BL_FLASH_ERR_WRITE,
    BL_FLASH_ERR_VERIFY
} bl_flash_status_t;

/* Xoá 1 sector (FLASH_SECTOR_3 ... FLASH_SECTOR_11). Từ chối Sector 0-2 (Bootloader). */
bl_flash_status_t bl_flash_erase_sector(uint32_t sector, bl_flash_width_t width);

/* Ghi `size` byte từ buffer vào flash tại start_addr.
 * size và start_addr phải chia hết cho width. Vùng ghi phải nằm sau Bootloader.
 * Vùng ghi phải đã được xoá trước (chỉ đổi được bit 1 -> 0). */
bl_flash_status_t bl_flash_write(uint32_t size, const uint8_t *buffer,
                                 uint32_t start_addr, bl_flash_width_t width);

/* So sánh `size` byte trong flash với buffer. */
bl_flash_status_t bl_flash_verify(uint32_t size, const uint8_t *buffer, uint32_t start_addr);

#endif /* INC_BL_FLASH_H_ */
