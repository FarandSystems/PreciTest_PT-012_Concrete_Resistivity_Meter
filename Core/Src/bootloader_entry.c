#include "bootloader_entry.h"

void Bootloader_RequestAndReset(void)
{
    // Enable backup domain and write the magic flag
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_RTC_ENABLE();  // harmless if already enabled by your Initialize_RTC()

    RTC->BKP0R = BL_REQUEST_MAGIC;    // set flag for bootloader

    __DSB(); __ISB();
    __disable_irq();

    // (Optional) stop SysTick to avoid surprises during reset
    SysTick->CTRL = 0; SysTick->LOAD = 0; SysTick->VAL = 0;

    NVIC_SystemReset();
}