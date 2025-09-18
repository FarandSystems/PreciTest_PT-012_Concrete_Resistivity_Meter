#pragma once
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BL_REQUEST_MAGIC  0xB007B007u  // any unique value

	void Bootloader_RequestAndReset(void);

#ifdef __cplusplus
}
#endif