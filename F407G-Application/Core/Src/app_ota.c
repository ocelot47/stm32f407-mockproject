#include "app_ota.h"
#include "main.h"            /* HAL */
#include "app_header.h"      /* app_header_t, APP_MAGIC */
#include <string.h>

#define HDR_WORDS   (sizeof(app_header_t) / sizeof(uint32_t))   /* 5 word */

int app_ota_request(void)
{
    uint32_t hdr[HDR_WORDS];
    FLASH_EraseInitTypeDef erase = {0};
    uint32_t sector_error = 0xFFFFFFFFUL;
    int rc = 0;

    /* 1. Đọc cả header vào RAM. Header hỏng (magic sai) thì không đụng vào flash. */
    memcpy(hdr, APP_FLASH_PTR(APP_OTA_HEADER_ADDR), sizeof hdr);
    if (hdr[1] != APP_MAGIC)         /* word 1 = magic */
    {
        return -1;
    }

    /* 2. Sửa cờ trong RAM: word 0 = ota_flag */
    hdr[0] = 1U;

    /* 3. Xoá Sector 4 rồi ghi lại 5 word.
     *    Trong lúc xoá/ghi CPU đứng chờ, nên tắt ngắt cho chắc. */
    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.Sector       = FLASH_SECTOR_4;
    erase.NbSectors    = 1U;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;      /* 2.7 - 3.6 V, ghi word */

    __disable_irq();
    HAL_FLASH_Unlock();

    if ((HAL_FLASHEx_Erase(&erase, &sector_error) != HAL_OK) || (sector_error != 0xFFFFFFFFUL))
    {
        rc = -2;
    }
    else
    {
        for (uint32_t i = 0U; i < HDR_WORDS; i++)
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                  APP_OTA_HEADER_ADDR + (4UL * i), hdr[i]) != HAL_OK)
            {
                rc = -3;
                break;
            }
        }
    }

    HAL_FLASH_Lock();
    __enable_irq();
    return rc;
}
