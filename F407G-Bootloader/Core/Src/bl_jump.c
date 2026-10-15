#include "bl_jump.h"
#include "bl_crc.h"
#include "app_header.h"
#include "flash_layout.h"
#include "stm32f4xx_hal.h"

app_status_t validate_app(void)
{
    const volatile app_header_t *hdr =
        (const volatile app_header_t *)APP_HEADER_START_ADDR;

    uint32_t app_msp           = *(__IO uint32_t *)(APP_START_ADDR + MSP_OFFSET);
    uint32_t app_reset_handler = *(__IO uint32_t *)(APP_START_ADDR + RESET_HANDLER_OFFSET);

    /* 1. Magic */
    if (hdr->magic != APP_MAGIC)
    {
        return APP_MAGIC_INVALID;
    }

    /* 2. Size: đủ chứa Vector Table (8 byte) và không vượt vùng App */
    if ((hdr->size < 8U) || (hdr->size > APP_SIZE))
    {
        return APP_SIZE_INVALID;
    }

    /* 3. CRC: tính trên [APP_START_ADDR, APP_START_ADDR + size) */
    uint32_t crc = bl_crc32((const uint8_t *)APP_START_ADDR, hdr->size);
    if (crc != hdr->crc)
    {
        return APP_CRC_INVALID;
    }

    /* 4. Initial MSP phải nằm trong SRAM */
    if ((app_msp < SRAM_START_ADDR) || (app_msp > SRAM_END_ADDR))
    {
        return APP_MSP_INVALID;
    }

    /* 5. Reset Handler: Thumb bit = 1 */
    if ((app_reset_handler & 0x1UL) == 0UL)
    {
        return APP_RESET_HANDLER_THUMB_INVALID;
    }

    /* 6. Reset Handler (bỏ Thumb bit) nằm trong vùng App */
    uint32_t reset_addr = app_reset_handler & ~0x1UL;
    if ((reset_addr < APP_START_ADDR) || (reset_addr >= APP_END_ADDR))
    {
        return APP_RESET_HANDLER_RANGE_INVALID;
    }

    return APP_VALID;
}

void jump_to_app(void)
{
    /* 1. Đọc MSP và Reset Handler của App */
    uint32_t app_msp           = *(__IO uint32_t *)(APP_START_ADDR + MSP_OFFSET);
    uint32_t app_reset_handler = *(__IO uint32_t *)(APP_START_ADDR + RESET_HANDLER_OFFSET);

    void (*app_entry)(void) = (void (*)(void))app_reset_handler;

    /* 2. Đưa clock về mặc định HSI */
    HAL_RCC_DeInit();

    /* 3. Tắt interrupt toàn cục */
    __disable_irq();

    /* 4. Dừng SysTick */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* 5. Disable và clear pending toàn bộ NVIC interrupt */
    for (uint32_t i = 0; i < 8U; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    /* 6. Clear pending SysTick và PendSV */
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk | SCB_ICSR_PENDSVCLR_Msk;

    /* 7. Relocate Vector Table sang App */
    SCB->VTOR = APP_START_ADDR;
    __DSB();
    __ISB();

    /* 8. Nạp MSP của App */
    __set_MSP(app_msp);

    /* 9. Nhảy vào Reset Handler của App */
    app_entry();

    while (1) { }   /* không bao giờ tới đây */
}
