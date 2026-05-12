#include "buttons.h"
#include "flash.h"
#include "main.h"
#include "periph.h"


static	void OnButtonPressed(void);
static	void OnButtonReleased(void);


//============================================================================================
void ReadButton(Tbutton	*button)
{
				uint8_t reading;
	
				// Чтение состояния кнопки
        reading = HAL_GPIO_ReadPin(button->GPIOx, button->GPIO_Pin);

        // Проверка на дребезг
        if (reading != button->lastButtonState) {
            button->lastDebounceTime = HAL_GetTick(); // Запоминаем время изменения состояния
        }

        // Если состояние стабильно в течение debounceDelay, считаем его действительным
        if ((HAL_GetTick() - button->lastDebounceTime) > button->debounceDelay) {
            // Если состояние изменилось
            if (reading != button->buttonState) {
                button->buttonState = reading;

                // Обработка нажатия кнопки
                if (button->buttonState == GPIO_PIN_SET) { // Или GPIO_PIN_RESET, в зависимости от схемы
                    // Кнопка нажата
                    OnButtonPressed();
                } else {
                    // Кнопка отпущена
                    OnButtonReleased();
                }
            }
        }

        // Сохраняем текущее состояние для следующего цикла
        button->lastButtonState = reading;

        // Небольшая задержка для снижения нагрузки на процессор
        //HAL_Delay(10);
}




//============================================================================================
// Функция, вызываемая при нажатии кнопки
void OnButtonPressed(void)
{
    // Ваш код для обработки нажатия
    int16_t dataToWrite; // Данные для записи
    int16_t dataRead = 0;	
	
		//dataToWrite = Drive1.CalADC;

        // Рассчитайте калибровочное значение
		Drive1.PosDrumZeroAng = Drive1.PosADCFiltered;
    dataToWrite = Drive1.PosDrumZeroAng;

    // Запись данных в Flash
    if (WriteI16ToFlash(FLASH_USER_START_ADDR, dataToWrite) == HAL_OK)
    {
        // Чтение данных из Flash
        dataRead = ReadI16FromFlash(FLASH_USER_START_ADDR);

        // Проверка корректности записи
        if (dataRead == dataToWrite)
        {
            // Данные записаны и прочитаны успешно
						Drive1.CalADC = dataRead;
						
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;
        }
        else
        {
            // Ошибка: данные не совпадают
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;
						HAL_Delay(500);
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;					
        }
    }
    else
    {
        // Ошибка записи
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;
						HAL_Delay(500);
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;	
						HAL_Delay(500);
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;	
						HAL_Delay(500);
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;	
						HAL_Delay(500);
						LED_ENABLE;
						HAL_Delay(500);
						LED_DISABLE;				
    }	
}
//============================================================================================



//============================================================================================
// Функция, вызываемая при отпускании кнопки
void OnButtonReleased(void)
{
    // Ваш код для обработки отпускания
}
//============================================================================================
