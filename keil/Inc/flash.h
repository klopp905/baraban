#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

// Адреса для записи в последний сектор Flash
#define FLASH_USER_START_ADDR 0x08007000  // Начало последнего сектора
#define FLASH_USER_END_ADDR   0x08007FFF  // Конец Flash-памяти


HAL_StatusTypeDef WriteI16ToFlash(uint32_t address, int16_t data);
int16_t ReadI16FromFlash(uint32_t address);


#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H */
