#include "flash.h"
//#include "periph.h"
//#include "stm32f0xx_hal.h"




//============================================================================================
// Функция для записи int16_t в Flash
HAL_StatusTypeDef WriteI16ToFlash(uint32_t address, int16_t data)
{
    HAL_StatusTypeDef status;

    // Отключение прерываний
    __disable_irq();	
	
    // Разблокировка Flash
    HAL_FLASH_Unlock();

    // Очистка флагов ошибок
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

    // Настройка структуры для стирания страницы
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = FLASH_USER_START_ADDR; // Адрес начала сектора
    eraseInit.NbPages = 1; // Стираем 1 страницу (1 КБ)

    // Стирание страницы
    status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);
    if (status != HAL_OK)
    {
        // Обработка ошибки стирания
        HAL_FLASH_Lock();
				__enable_irq(); // Включение прерываний перед выходом
        return status;
    }

    // Запись данных
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, (uint16_t)data);
    if (status != HAL_OK)
    {
        // Обработка ошибки записи
        HAL_FLASH_Lock();
				__enable_irq(); // Включение прерываний перед выходом
        return status;
    }

    // Блокировка Flash
    HAL_FLASH_Lock();
		
    // Включение прерываний
    __enable_irq();

    return HAL_OK;
}
//============================================================================================



//============================================================================================
// Функция для чтения int16_t из Flash
int16_t ReadI16FromFlash(uint32_t address)
{
    return *(__IO int16_t*)address; // Чтение данных по адресу
}
//============================================================================================
