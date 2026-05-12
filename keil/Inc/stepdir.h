#ifndef __STEPDIR_H
#define __STEPDIR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

// Определяем пины для Step и Dir
#define STEP_GPIO_PORT GPIOA
#define STEP_PIN GPIO_PIN_8  // Пример: PA8 (TIM1_CH1)

#define DIR_GPIO_PORT GPIOB
#define DIR_PIN GPIO_PIN_14  // Пример: PB14

// Максимальная и минимальная частота Step (в Гц)
#define MAX_STEP_FREQ 2222   // Максимальная частота импульсов
#define MIN_STEP_FREQ 222    // Минимальная частота импульсов

void StepDir_Init(void);
void StepDir_Control(float speed);


#ifdef __cplusplus
}
#endif

#endif /* __STEPDIR_H */


