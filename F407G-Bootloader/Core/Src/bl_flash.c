#include "bl_flash.h"
#include "main.h"            /* HAL */
#include "flash_layout.h"    /* BOOTLOADER_END_ADDR, FLASH_END_ADDR */
#include <string.h>

/* Kiểm tra độ rộng hợp lệ và đổi sang tham số HAL */
static int width_ok(bl_flash_width_t width)
{
    return (width == BL_FLASH_X8) || (width == BL_FLASH_X16) || (width == BL_FLASH_X32);
}

static uint32_t to_type_program(bl_flash_width_t width)
{
    switch (width)
    {
        case BL_FLASH_X8:  return FLASH_TYPEPROGRAM_BYTE;
        case BL_FLASH_X16: return FLASH_TYPEPROGRAM_HALFWORD;
        default:           return FLASH_TYPEPROGRAM_WORD;
    }
}

/* Dùng cùng một độ rộng cho xoá và ghi để PSIZE nhất quán */
static uint32_t to_voltage_range(bl_flash_width_t width)
{
    switch (width)
    {
        case BL_FLASH_X8:  return FLASH_VOLTAGE_RANGE_1;
        case BL_FLASH_X16: return FLASH_VOLTAGE_RANGE_2;
        default:           return FLASH_VOLTAGE_RANGE_3;
    }
}

/* Vùng [addr, addr + size) phải nằm sau Bootloader và trong flash */
static int range_ok(uint32_t addr, uint32_t size)
{
    if (addr < BOOTLOADER_END_ADDR)       { return 0; }
    if (addr >= FLASH_END_ADDR)           { return 0; }
    if (size > (FLASH_END_ADDR - addr))   { return 0; }
    return 1;
}

bl_flash_status_t bl_flash_erase_sector(uint32_t sector, bl_flash_width_t width)
{
    FLASH_EraseInitTypeDef erase = {0};
    uint32_t sector_error = 0xFFFFFFFFUL;
    HAL_StatusTypeDef st;

    if (!width_ok(width))                                   { return BL_FLASH_ERR_PARAM; }
    if ((sector < FLASH_SECTOR_3) || (sector > FLASH_SECTOR_11)) { return BL_FLASH_ERR_PARAM; }

    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.Sector       = sector;
    erase.NbSectors    = 1U;
    erase.VoltageRange = to_voltage_range(width);

    HAL_FLASH_Unlock();
    st = HAL_FLASHEx_Erase(&erase, &sector_error);
    HAL_FLASH_Lock();

    if ((st != HAL_OK) || (sector_error != 0xFFFFFFFFUL))
    {
        return BL_FLASH_ERR_ERASE;
    }
    return BL_FLASH_OK;
}

bl_flash_status_t bl_flash_write(uint32_t size, const uint8_t *buffer,
                                 uint32_t start_addr, bl_flash_width_t width)
{
    bl_flash_status_t result = BL_FLASH_OK;
    uint32_t w;

    if (buffer == NULL)                { return BL_FLASH_ERR_PARAM; }
    if (!width_ok(width))              { return BL_FLASH_ERR_PARAM; }
    w = (uint32_t)width;
    if (size == 0U)                    { return BL_FLASH_ERR_PARAM; }
    if ((size % w) != 0U)              { return BL_FLASH_ERR_PARAM; }
    if ((start_addr % w) != 0U)        { return BL_FLASH_ERR_PARAM; }
    if (!range_ok(start_addr, size))   { return BL_FLASH_ERR_PARAM; }

    HAL_FLASH_Unlock();

    for (uint32_t off = 0U; off < size; off += w)
    {
        uint32_t data = 0U;
        memcpy(&data, buffer + off, w);   /* memcpy: buffer có thể không căn 4 byte */

        if (HAL_FLASH_Program(to_type_program(width), start_addr + off, (uint64_t)data) != HAL_OK)
        {
            result = BL_FLASH_ERR_WRITE;
            break;
        }
    }

    HAL_FLASH_Lock();
    return result;
}

bl_flash_status_t bl_flash_verify(uint32_t size, const uint8_t *buffer, uint32_t start_addr)
{
    if ((buffer == NULL) || (size == 0U) || !range_ok(start_addr, size))
    {
        return BL_FLASH_ERR_PARAM;
    }
    if (memcmp(BL_FLASH_PTR(start_addr), buffer, size) != 0)
    {
        return BL_FLASH_ERR_VERIFY;
    }
    return BL_FLASH_OK;
}
