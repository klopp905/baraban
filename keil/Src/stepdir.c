#include "stepdir.h"
#include "periph.h"
#include "d808.h"
#include <math.h>
#include "main.h"


// Переменные для хранения текущей частоты и направления
//static uint32_t current_freq = 0;
//static uint8_t current_dir = 0;
//uint8_t		flagON = 0;


//==============================================================================
// Инициализация таймера TIM1 и GPIO
void StepDir_Init(void) {
    // Включаем тактирование GPIO и TIM1
    __HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    // Настройка пина Step (TIM1_CH1)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = STEP_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;  // TIM1_CH1 на PA8
    HAL_GPIO_Init(STEP_GPIO_PORT, &GPIO_InitStruct);

    // Настройка пина Dir
    GPIO_InitStruct.Pin = DIR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIR_GPIO_PORT, &GPIO_InitStruct);

    // Настройка таймера TIM1
    //TIM_HandleTypeDef htim1;
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = (SystemCoreClock / 1000000) - 1;  // Предделитель для тактовой частоты 1 МГц
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = (1000000 / MAX_STEP_FREQ) - 1;       // Начальная частота
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
        Error_Handler();  // Обработка ошибки инициализации
    }

    // Настройка канала PWM (TIM1_CH1)
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = (htim1.Init.Period + 1) / 2;  // Скважность 50%
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();  // Обработка ошибки настройки канала
    }

    // Запуск таймера
    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
//==============================================================================



//==============================================================================
// Функция для управления сервоприводом
void StepDir_Control(float speed) {
    // Ограничение входного значения от -1 до 1
    if (speed < -1.0f) speed = -1.0f;
    if (speed > 1.0f) speed = 1.0f;

    // Определение направления
    if (speed < 0) {
        //HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, GPIO_PIN_SET);  // Направление в одну сторону
				HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, GPIO_PIN_RESET);  // на шкафу
        speed = fabs(speed);  // Берем модуль скорости
    } else {
        //HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, GPIO_PIN_RESET);    // Направление в другую сторону
				HAL_GPIO_WritePin(DIR_GPIO_PORT, DIR_PIN, GPIO_PIN_SET);  // на шкафу
    }

    // Если скорость равна нулю, останавливаем таймер
    if ( (fabs(speed) < SPEED_CUTOFF) /*|| (Drive1.RawADC < 100) || (Drive1.RawADC > 4000)*/ ) {
				HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);  // Останавливаем генерацию импульсов
        //ENA_OUT_OFF;
				//flagON = 0;
        return;
    }		
		
    // Преобразование скорости в частоту Step
    uint32_t freq = MIN_STEP_FREQ + (uint32_t)(speed * (MAX_STEP_FREQ - MIN_STEP_FREQ));

    // Обновление частоты таймера
    //TIM1->ARR = (1000000 / freq) - 1;  // ARR = (тактов / частота) - 1
    //TIM1->CCR1 = (TIM1->ARR + 1) / 2;  // Скважность 50%
    __HAL_TIM_SET_AUTORELOAD(&htim1, (1000000 / freq) - 1);  										// ARR = (тактов / частота) - 1
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period + 1) / 2);	// Скважность 50%		
		
    // Если таймер был остановлен, запускаем его
    if (/*flagON == 0*/ !(htim1.Instance->CR1 & TIM_CR1_CEN)/*!__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE)*/) {
				HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        //ENA_OUT_ON;
				//flagON = 1;
    }		
}
//==============================================================================
