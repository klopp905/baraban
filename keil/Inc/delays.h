#include "stm32f0xx_hal.h"



void Delays_init(TIM_HandleTypeDef *tim);
void Mks(uint32_t time);
//void Ms(uint16_t time);
void Ms(__IO uint32_t nTime);
//void nops(__IO uint32_t nTime);
void TimingDelay_Decrement(void);


