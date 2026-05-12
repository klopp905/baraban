#ifndef __PERIPH_H
#define __PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

//============================================================================================

//---Mks timer------------------------------------------------
#define   TIMMKS                          TIM17
//#define   RCC_TIMMKS_PeriphClockCmd(a)    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, (a))
#define		TIM_MKS_CLK_ENABLE()						__HAL_RCC_TIM17_CLK_ENABLE()

//---step-dir-------------------------------------------------
//#define		TIM_PWM									TIM1
//#define		TIM_PWM_ARR							4800//(800)//19200//(200)//76800//9600(400)//2400//240//1920(1600steps)//
//#define		TIM_PWM_CCR							2400//(800)//9600//(200)//38400//4800(400)//1200//180//960(1600steps)//
#define		TIM_PWM_HANDLER					htim1
//#define		TIM_PWM_CLK_ENABLE()		__HAL_RCC_TIM1_CLK_ENABLE()

//---лини€ TX-------------------------------------------------
#define		GPIO_UARTTX 							GPIOA
#define		GPIO_PIN_UARTTX						GPIO_PIN_2
#define		GPIO_UARTTX_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()

//---лини€ RX-------------------------------------------------
#define		GPIO_UARTRX 							GPIOA
#define		GPIO_PIN_UARTRX						GPIO_PIN_3
#define		GPIO_UARTRX_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()

//---uart-----------------------------------------------------
#define		UART										USART1
#define		UART_HANDLER						huart1
#define		UART_CLK_ENABLE()				__HAL_RCC_USART1_CLK_ENABLE()
#define		UART_IRQ								USART1_IRQn

//+++лини€ TX freemaster+++++++++++++++++++++++++++++++++++++++++++++
#define		GPIO_UARTFM_TX 								GPIOA
#define		GPIO_PIN_UARTFM_TX						GPIO_PIN_2
#define		GPIO_UARTFM_TX_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

//+++лини€ RX freemaster+++++++++++++++++++++++++++++++++++++++++++++
#define		GPIO_UARTFM_RX 								GPIOA
#define		GPIO_PIN_UARTFM_RX						GPIO_PIN_3
#define		GPIO_UARTFM_RX_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

//+++uart freemaster+++++++++++++++++++++++++++++++++++++++++++++++++
#define		UARTFM												USART1
#define		UARTFM_HANDLER								huart1
#define		UARTFM_CLK_ENABLE()						__HAL_RCC_USART1_CLK_ENABLE()
#define		UARTFM_IRQ										USART1_IRQn

//---потенциометр---------------------------------------------
//#define		GPIO_POT								GPIOA
//#define		GPIO_PIN_POT						GPIO_PIN_0
//#define		GPIO_POT_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
//#define		ADC_POT									ADC1
//#define		ADC_POT_HANDLER					hadc1
//#define		ADC_POT_CLK_ENABLE()		__HAL_RCC_ADC1_CLK_ENABLE()
//#define		ADC_IRQn								ADC1_IRQn

//---энкодер AS5045-------------------------------------------
#define		GPIO_ENC_SCK										GPIOA
#define		GPIO_PIN_ENC_SCK								GPIO_PIN_5
#define 	GPIO_AF_ENC_SCK                 GPIO_AF0_SPI1
#define		GPIO_ENC_SCK_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()

#define		GPIO_ENC_MISO										GPIOA
#define		GPIO_PIN_ENC_MISO								GPIO_PIN_6
#define 	GPIO_AF_ENC_MISO                GPIO_AF0_SPI1
#define		GPIO_ENC_MISO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()

#define		GPIO_ENC_CS											GPIOA
#define		GPIO_PIN_ENC_CS									GPIO_PIN_4
#define		GPIO_ENC_CS_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()

#define		SPI_ENC									SPI1
#define		SPI_ENC_HANDLER					hspi1
#define		SPI_ENC_CLK_ENABLE()		__HAL_RCC_SPI1_CLK_ENABLE()
#define		SPI_IRQn								SPI1_IRQn

#define		ENC_CS_HIGH		 					GPIO_ENC_CS->BSRR = GPIO_PIN_ENC_CS
#define		ENC_CS_LOW			 				GPIO_ENC_CS->BSRR = (uint32_t)GPIO_PIN_ENC_CS << 16U
//---светодиод Fault------------------------------------------
#define		GPIO_LED								GPIOA
#define		GPIO_PIN_LED						GPIO_PIN_9
#define		GPIO_LED_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

#define		LED_DISABLE						  GPIO_LED->BRR   = GPIO_PIN_LED
#define		LED_ENABLE						  GPIO_LED->BSRR  = GPIO_PIN_LED

//--- нопка 1-------------------------------------------------
#define		GPIO_BUT1								GPIOC
#define		GPIO_PIN_BUT1						GPIO_PIN_13
#define		GPIO_BUT1_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

//--- нопка 2-------------------------------------------------
#define		GPIO_BUT2								GPIOC
#define		GPIO_PIN_BUT2						GPIO_PIN_14
#define		GPIO_BUT2_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

//--- нопка 3-------------------------------------------------
#define		GPIO_BUT3								GPIOC
#define		GPIO_PIN_BUT3						GPIO_PIN_15
#define		GPIO_BUT3_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

//---¬ход FLT-------------------------------------------------
#define		GPIO_FLT								GPIOB
#define		GPIO_PIN_FLT						GPIO_PIN_12
#define		GPIO_FLT_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()

//============================================================================================


//===протокол=================================================================================

//---прием комманд--------------------------------------------
#define		PROTOCOL_COMM_ROTATE		0x10
#define		PROTOCOL_DATA_DIRCW			0x00
#define		PROTOCOL_DATA_DIRCCW		0x01


//---отправка ответа------------------------------------------
#define		PROTOCOL_ANS_POTH				0x08
#define		PROTOCOL_ANS_POTL				0xAD
#define		PROTOCOL_ANS_STATE			0x01
#define		PROTOCOL_ANS_CRCH				0x00
#define		PROTOCOL_ANS_CRCL				0x00

//============================================================================================

/*struct  Tanswer{

	uint16_t pot; 	// данные с ADC, 12бит
	uint8_t state; 	// состо€ние
	uint16_t crc; 	// контрольна€ суммат
  
};*/


//============================================================================================
extern	TIM_HandleTypeDef htim1;
extern  TIM_HandleTypeDef htim17;
extern	ADC_HandleTypeDef hadc1;
extern	UART_HandleTypeDef huart1;
extern	SPI_HandleTypeDef hspi1;
//============================================================================================



//============================================================================================
void GPIO_Config(void);
void UART_Config(UART_HandleTypeDef *uart);
void USART_FM_Config(void);
void USART_FM_ENABLE_IT(void);

void AS5045_Config(SPI_HandleTypeDef *spi);
uint16_t AS5045_ReadAngle(void);
uint16_t Dpr_Read( void );
//void ADC_Config(ADC_HandleTypeDef *adc);

//void TIM_Pwm_Config(TIM_HandleTypeDef *tim);
//void TIM_Pwm_Start(void);
//void TIM_Pwm_ENABLE(void);
//void TIM_Pwm_DISABLE(void);
//void TIM_Load_RCR(uint8_t cnt);
//============================================================================================



#ifdef __cplusplus
}
#endif

#endif /* __PERIPH_H */
