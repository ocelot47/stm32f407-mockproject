#ifndef INC_APP_OTA_H_
#define INC_APP_OTA_H_

#include <stdint.h>

/* Đọc nội dung flash theo địa chỉ. Unit test trên máy tính có thể định nghĩa lại. */
#ifndef APP_FLASH_PTR
#define APP_FLASH_PTR(addr)  ((const void *)(uintptr_t)(addr))
#endif

#define APP_OTA_HEADER_ADDR   0x08010000UL   /* Sector 4: header App */

/* Ghi ota_flag = 1 vào header trong flash (đọc cả header vào RAM, xoá Sector 4,
 * ghi lại 5 word). Trả về 0 nếu OK, âm nếu lỗi.
 * Hàm này KHÔNG reset MCU; gọi NVIC_SystemReset() sau khi nhận 0. */
int app_ota_request(void);

#endif /* INC_APP_OTA_H_ */
