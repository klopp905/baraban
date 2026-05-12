// Includes ------------------------------------------------------------------*/
#include "main.h"
#include "periph.h"


//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



//========================================================================
void HAL_MspInit(void)
{

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

}
//========================================================================



//========================================================================
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    // Peripheral clock enable 
    __HAL_RCC_TIM1_CLK_ENABLE();

  }

}
//========================================================================

/*
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM1)
  {

    __HAL_RCC_GPIOA_CLK_ENABLE();

    //PA9     ------> TIM1_CH1

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //HAL_I2CEx_EnableFastModePlus(SYSCFG_CFGR1_I2C_FMP_PA9);

  }

}
*/

//========================================================================
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    // Peripheral clock disable 
    __HAL_RCC_TIM1_CLK_DISABLE();

  }

}
//========================================================================



//========================================================================
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC_POT)
  {

    // Peripheral clock enable 
    ADC_POT_CLK_ENABLE();
    //GPIO_POT_CLK_ENABLE();
		
    // ADC GPIO Configuration 
		// PA0     ------> ADC_IN0
    //GPIO_InitStruct.Pin = GPIO_PIN_POT;
    //GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
    //HAL_GPIO_Init(GPIO_POT, &GPIO_InitStruct);

  }

}
//========================================================================



//========================================================================
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    // Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    // ADC GPIO Configuration
    // PA0     ------> ADC_IN0
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

  }

}
//========================================================================



//========================================================================
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {

    // Peripheral clock enable 
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
    // USART1 GPIO Configuration
    // PA2     ------> USART1_TX
    // PA3     ------> USART1_RX
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }

}
//========================================================================



//========================================================================
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {

    // Peripheral clock disable 
    __HAL_RCC_USART1_CLK_DISABLE();

    // USART1 GPIO Configuration
    // PA2     ------> USART1_TX
    // PA3     ------> USART1_RX
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

  }

}
//========================================================================
