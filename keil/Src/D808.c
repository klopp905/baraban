#include "d808.h"
#include "periph.h"
#include "delays.h"
#include "main.h"
#include "pi.h"
#include "protocol.h"
#include "stepdir.h"
#include "stm32f0xx_hal_tim.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

//========================================================================

//========================================================================


//===Private function prototypes==========================================
static	uint16_t MedianFilter_GetMedian(uint16_t *buffer, uint8_t size);

static	float normalizeAngle(float angle);
static	float angleDifference(float a, float b); 


//========================================================================
#define MEDIAN_FILTER_WINDOW_SIZE 5  // Размер окна медианного фильтра (должен быть нечетным)

// Буфер для хранения последних значений АЦП
uint16_t adcBuffer[MEDIAN_FILTER_WINDOW_SIZE];
uint8_t bufferIndex = 0;  // Индекс для циклического буфера



//============================================================================================
void D808_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  
	
	//---STEP-DIR линия Fault-------------------------------------
  //---клочим порт -------------------------------------------------------------------------- 
	GPIO_FLT_CLK_ENABLE();	
  //---конфигурируем порт--------------------------------------------------------------------
  GPIO_InitStruct.Pin				= GPIO_PIN_FLT;
  GPIO_InitStruct.Mode			= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull			= GPIO_PULLUP;
  GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init( GPIO_FLT, &GPIO_InitStruct );		
	
	//---STEP-DIR линия In Position-------------------------------
  //---клочим порт ---------------------------------------------
	GPIO_INP_CLK_ENABLE();	
  //---конфигурируем порт---------------------------------------
  GPIO_InitStruct.Pin				= GPIO_PIN_INP;
  GPIO_InitStruct.Mode			= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull			= GPIO_PULLUP;
  GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init( GPIO_INP, &GPIO_InitStruct );	
	
	//---STEP-DIR линия DIR---------------------------------------
  //---клочим порт ---------------------------------------------
	//GPIO_DIR_CLK_ENABLE();	
  //---конфигурируем порт---------------------------------------
  //GPIO_InitStruct.Pin				= GPIO_PIN_DIR;
  //GPIO_InitStruct.Mode			= GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull			= GPIO_NOPULL;
  //GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_HIGH;
  //HAL_GPIO_Init( GPIO_DIR, &GPIO_InitStruct );
	//DIR_OUT_OFF;
	
	//---STEP-DIR линия ENA---------------------------------------
  //---клочим порт --------------------------------------------- 
	GPIO_ENA_CLK_ENABLE();	
  //---конфигурируем порт---------------------------------------
  GPIO_InitStruct.Pin				= GPIO_PIN_ENA;
  GPIO_InitStruct.Mode			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull			= GPIO_NOPULL;
  GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( GPIO_ENA, &GPIO_InitStruct );	
	ENA_OUT_ON;
	
	//---STEP-DIR линия PUL---------------------------------------
  //---клочим порт --------------------------------------------- 
	//GPIO_PUL_CLK_ENABLE();	
  //---конфигурируем порт---------------------------------------
  //GPIO_InitStruct.Pin				= GPIO_PIN_PUL;
  //GPIO_InitStruct.Mode			= GPIO_MODE_AF_PP;//GPIO_MODE_OUTPUT_PP;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //GPIO_InitStruct.Pull			= GPIO_NOPULL;
  //GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_HIGH;
	//GPIO_InitStruct.Alternate	= GPIO_AF_PUL;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //HAL_GPIO_Init( GPIO_PUL, &GPIO_InitStruct );	
	
	//PUL_ENABLE;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//---конфигурируем таймер плюющий step-dir импульсы-----------
	//TIM_Pwm_Config(&TIM_PWM_HANDLER);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//TIM_Pwm_ENABLE();
	
	//---конфигурируем таймер микросекундных задержек-------------
	Delays_init(&htim17);

}
//============================================================================================



//============================================================================================
// Задание номера сектора
void D808_SetSector(Tdrv	*drv, uint16_t sector)
{
		drv->RefDrumAdr = sector;
}
//============================================================================================



//============================================================================================
// Получаем угол барабана (-540; 540) в градусах из значения ADC
void D808_GetDrumAngle(Tdrv	*drv)
{
		drv->PosDrumAng = -((1080.0f * drv->PosADCFiltered )/ 4095.0f - 540.0f);
}
//============================================================================================



//============================================================================================
// Получаем угол барабана (0; 1080) в градусах из значения ADC
float D808_GetDrumLinAngle(float	adcVal)
{
		//return (adcVal * 1080.0f) / 4095.0f;
		//return (((4095.0f - adcVal) * 1080.0f) / 4095.0f ) - 540.0f;
		return -540.0f + (1080.0f / 4095.0f) * adcVal;
}
//============================================================================================



//============================================================================================
// Определение адреса сектора
uint8_t Drum_GetSector(Tdrum* drum) {
    uint16_t angle = (drum->current_angle - drum->zero_angle + MAX_ANGLE) % MAX_ANGLE;
    return (angle * Drive1.SectorsCount/*NUM_SECTORS*/) / MAX_ANGLE;
}
//============================================================================================



//============================================================================================
// Регулятор
void D808_Reg(void)
{
	
					//---регулятор положения---------------------------------------------------------------
					Pi_Calc(&Drive1.pP, Drive1.RefDrumAng, Drive1.PosDrumAng );

		
					//---загрузим в таймер-----------------------------------------------------------------
					StepDir_Control(Drive1.pP.Output);
					
		
}
//============================================================================================


//============================================================================================
// проверка попадания в уставку
uint8_t D808_CheckInpos(void) 
{
			if( fabs(Drive1.pP.Err) < POS_CUTOFF )
			{
					//   положение отработано
					return 0;
			}	
			else
			{
					//   положение еще отрабатывается	
					return 1;
			}	
}
//============================================================================================



//============================================================================================
// Вспомогательная функция для вычисления медианы
uint16_t MedianFilter_GetMedian(uint16_t *buffer, uint8_t size)
{
    uint16_t tempBuffer[size];
    memcpy(tempBuffer, buffer, size * sizeof(uint16_t));  // Копируем данные во временный буфер

    // Сортировка пузырьком (можно заменить на более эффективный алгоритм)
    for (uint8_t i = 0; i < size - 1; i++)
    {
        for (uint8_t j = 0; j < size - i - 1; j++)
        {
            if (tempBuffer[j] > tempBuffer[j + 1])
            {
                uint16_t temp = tempBuffer[j];
                tempBuffer[j] = tempBuffer[j + 1];
                tempBuffer[j + 1] = temp;
            }
        }
    }

    // Возвращаем медианное значение
    return tempBuffer[size / 2];
}
//============================================================================================


/*
//============================================================================================
// чтение положения с фильтрованием
void D808_ReadPosition(void) 
{
    int32_t compensatedValue = 0;
		uint16_t medianFilteredValue = 0;

    //--- Старт АЦП ---------------------------------------------------------------
    HAL_ADC_Start(&ADC_POT_HANDLER);

    //--- Ожидание завершения преобразования --------------------------------------
    if (HAL_ADC_PollForConversion(&ADC_POT_HANDLER, HAL_MAX_DELAY) == HAL_OK)
    {
        //--- Чтение сырого значения АЦП ------------------------------------------
        Drive1.RawADC = (uint16_t)HAL_ADC_GetValue(&ADC_POT_HANDLER);

        //--- Компенсация и смещение нуля -----------------------------------------
        compensatedValue = (int32_t)Drive1.RawADC + Drive1.CalADC; // Добавление калибровочного значения
        //compensatedValue -= 2048; // Смещение нуля в середину диапазона (2048)

        //--- Проверка на выход за пределы 12-битного диапазона (0–4095) ----------
        if (compensatedValue < 0) { compensatedValue = 0; } 
        else if (compensatedValue > 4095) { compensatedValue = 4095; }

        //--- Запись результата --------------------------------------------------
        Drive1.PosADC = (uint16_t)compensatedValue;

        //--- Обновление буфера для медианного фильтра ---------------------------
        adcBuffer[bufferIndex] = Drive1.PosADC;
        bufferIndex = (bufferIndex + 1) % MEDIAN_FILTER_WINDOW_SIZE;  // Циклический буфер

        //--- Применение медианного фильтра --------------------------------------
        medianFilteredValue = MedianFilter_GetMedian(adcBuffer, MEDIAN_FILTER_WINDOW_SIZE);

        //--- Инициализация фильтра, если это первый вызов -----------------------
        if (Drive1.flagInit == 0)
        {
            Drive1.PosADCFiltered = medianFilteredValue;
            Drive1.flagInit = 1;
        }

        //--- Применение фильтра низких частот -----------------------------------
        Drive1.PosADCFiltered += (medianFilteredValue - Drive1.PosADCFiltered) * SPEED_FILTER_COEFF;
    }
    else
    {
        // Обработка ошибки преобразования АЦП
        // Например, можно установить флаг ошибки или записать лог
    }
}
//============================================================================================
*/



//============================================================================================
// чтение положения с фильтрованием
void D808_ReadPosition(void) 
{
    //int32_t compensatedValue = 0;
		uint16_t medianFilteredValue = 0;


        //--- Чтение сырого значения АЦП ------------------------------------------
        //Drive1.RawADC = (uint16_t)AS5045_ReadAngle();

        //--- Компенсация и смещение нуля -----------------------------------------
        //compensatedValue = (int32_t)Drive1.RawADC + Drive1.CalADC; // Добавление калибровочного значения

        //--- Проверка на выход за пределы 12-битного диапазона (0–4095) ----------
        //if (compensatedValue < 0) { compensatedValue = 0; } 
        //else if (compensatedValue > 4095) { compensatedValue = 4095; }

        //--- Запись результата --------------------------------------------------
        Drive1.PosADC = (uint16_t)AS5045_ReadAngle();

        //--- Обновление буфера для медианного фильтра ---------------------------
        adcBuffer[bufferIndex] = Drive1.PosADC;
        bufferIndex = (bufferIndex + 1) % MEDIAN_FILTER_WINDOW_SIZE;  // Циклический буфер

        //--- Применение медианного фильтра --------------------------------------
        medianFilteredValue = MedianFilter_GetMedian(adcBuffer, MEDIAN_FILTER_WINDOW_SIZE);

        //--- Инициализация фильтра, если это первый вызов -----------------------
        if (Drive1.flagInit == 0)
        {
            Drive1.PosADCFiltered = medianFilteredValue;
            Drive1.flagInit = 1;
        }

        //--- Применение фильтра низких частот -----------------------------------
        Drive1.PosADCFiltered += ((float)medianFilteredValue - Drive1.PosADCFiltered) * SPEED_FILTER_COEFF;

}
//============================================================================================



#define ANGLE_OFFSET 540.0f          // Смещение угла
#define ANGLE_RANGE 1080.0f          // Полный диапазон углов (от -540° до +540°)
#define HYSTERESIS_THRESHOLD 10.0f    // Порог гистерезиса (в градусах)

//============================================================================================
// Функция для нормализации угла в диапазон [-540°, 540°]
float normalizeAngle(float angle) {
    while (angle > ANGLE_OFFSET) angle -= ANGLE_RANGE;
    while (angle < -ANGLE_OFFSET) angle += ANGLE_RANGE;
    return angle;
}
//============================================================================================



//============================================================================================
// Функция для вычисления минимальной разницы между двумя углами
float angleDifference(float a, float b) {
    float diff = normalizeAngle(a - b);
    if (diff > ANGLE_OFFSET) {
        diff -= ANGLE_RANGE; // Корректируем разницу для отрицательных значений
    }
    return diff;
}
//============================================================================================



//============================================================================================
// Функция для поиска ближайшего к текущему положению угла
float findNearestAngle(uint16_t adr) {

    float target = (float)adr * Drive1.SectorSize;
    float angles[3];
    angles[0] = target - 360.0f;
    angles[1] = target;
    angles[2] = target + 360.0f;

    int minIndex = 0;
    float minDistance = fabs(angles[0] - Drive1.PosDrumAng);
    for (int i = 1; i < 3; i++) {
        float distance = fabs(angles[i] - Drive1.PosDrumAng);
        if (distance < minDistance) {
            minDistance = distance;
            minIndex = i;
        }
    }

    return angles[minIndex];
}
//============================================================================================



