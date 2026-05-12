#include "periph.h"
#include "main.h"
#include "protocol.h"

	

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim17;
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;
SPI_HandleTypeDef hspi1; // Структура для управления SPI


//========================================================================
extern	uint8_t str[3];

extern  uint8_t dataReceived;
extern  uint8_t dataTransmitted;

//extern	uint8_t buf[6];
//extern	uint8_t buf[5];
extern	uint8_t firstByteWait;
//extern	struct answer;

extern	uint8_t	flagSetStep;

extern  uint16_t uRev;
extern	uint16_t	Steps;
extern	uint8_t	Dir;


//========================================================================

//===Private function prototypes==========================================

//========================================================================


//===========================================================================================
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

		// Настройка пина LED_FLT
		GPIO_LED_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_LED;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_LED, &GPIO_InitStruct);
	
		// Настройка пина BUT1
		GPIO_BUT1_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_BUT1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_BUT1, &GPIO_InitStruct);	
		
		// Настройка пина BUT2
		GPIO_BUT2_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_BUT2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_BUT2, &GPIO_InitStruct);	

		// Настройка пина BUT3
		GPIO_BUT3_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_BUT3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_BUT3, &GPIO_InitStruct);	
		
		// Настройка пина FLT
		GPIO_FLT_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_FLT;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_FLT, &GPIO_InitStruct);			
	
}
//===========================================================================================



//============================================================================================
void UART_Config(UART_HandleTypeDef *uart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//--------------------------------------------------------------------------------------
  // Peripheral clock enable 
  UART_CLK_ENABLE();
	
	// Config
  uart->Instance = UART;
  uart->Init.BaudRate = 9600;
  uart->Init.WordLength = UART_WORDLENGTH_8B;
  uart->Init.StopBits = UART_STOPBITS_1;
  uart->Init.Parity = UART_PARITY_NONE;
  uart->Init.Mode = UART_MODE_TX_RX;
  uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart->Init.OverSampling = UART_OVERSAMPLING_16;
  uart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	//uart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_DATAINVERT_INIT | UART_ADVFEATURE_MSBFIRST_INIT;
  //uart->AdvancedInit.DataInvert = UART_ADVFEATURE_DATAINV_ENABLE;
  //uart->AdvancedInit.MSBFirst = UART_ADVFEATURE_MSBFIRST_ENABLE;	
	
  uart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(uart) != HAL_OK)
  {
    Error_Handler();
  }
	//--------------------------------------------------------------------------------------


	
	
	//--------------------------------------------------------------------------------------
  if(uart->Instance == UART)
  {
		
		// USART GPIO Configuration
		
    // Peripheral clock enable 
    GPIO_UARTTX_CLK_ENABLE();
    // PA2     ------> USART1_TX
    GPIO_InitStruct.Pin = GPIO_PIN_UARTTX;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIO_UARTTX, &GPIO_InitStruct);
		
		
    // Peripheral clock enable 
    GPIO_UARTRX_CLK_ENABLE();
    // PA3     ------> USART1_RX
    GPIO_InitStruct.Pin = GPIO_PIN_UARTRX;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIO_UARTRX, &GPIO_InitStruct);		
		
    // USART interrupt Init 
    HAL_NVIC_SetPriority(UART_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(UART_IRQ);			

  }
	//--------------------------------------------------------------------------------------


}
//============================================================================================



//===========================================================================================
void USART_FM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Peripheral clock enable 
  UARTFM_CLK_ENABLE();
	GPIO_UARTFM_TX_CLK_ENABLE();
	GPIO_UARTFM_RX_CLK_ENABLE();
	
  // USART3 GPIO Configuration
  GPIO_InitStruct.Pin				= GPIO_PIN_UARTFM_TX;
  GPIO_InitStruct.Mode			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull			= GPIO_PULLUP;
  GPIO_InitStruct.Speed			= GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIO_UARTFM_TX, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin				= GPIO_PIN_UARTFM_RX;	
	GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
	HAL_GPIO_Init(GPIO_UARTFM_RX, &GPIO_InitStruct);

  // USART3 interrupt Init 
  HAL_NVIC_SetPriority(UARTFM_IRQ, 4, 1);
  HAL_NVIC_EnableIRQ(UARTFM_IRQ);	
	
	
  UARTFM_HANDLER.Instance						= UARTFM;
  UARTFM_HANDLER.Init.BaudRate			= 115200;
  UARTFM_HANDLER.Init.WordLength		= UART_WORDLENGTH_8B;
  UARTFM_HANDLER.Init.StopBits			= UART_STOPBITS_1;
  UARTFM_HANDLER.Init.Parity				= UART_PARITY_NONE;
  UARTFM_HANDLER.Init.Mode					= UART_MODE_TX_RX;
  UARTFM_HANDLER.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  UARTFM_HANDLER.Init.OverSampling	= UART_OVERSAMPLING_8;
  if ( HAL_UART_Init( &UARTFM_HANDLER ) != HAL_OK ) { Error_Handler(); }
	
	__HAL_UART_ENABLE(&UARTFM_HANDLER);

}
//===========================================================================================



//===включение прерывания USART_FM===========================================================
void USART_FM_ENABLE_IT(void)
{
	__HAL_UART_ENABLE_IT(&UARTFM_HANDLER, UART_IT_RXNE);
}
//===========================================================================================



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



//============================================================================================
void AS5045_Config(SPI_HandleTypeDef *spi)
{

		GPIO_InitTypeDef GPIO_InitStruct = {0};
	
		//---клочим порты линий SCK, MISO, CS-----------------------------------------------------
		GPIO_ENC_SCK_CLK_ENABLE();
		GPIO_ENC_MISO_CLK_ENABLE();
		GPIO_ENC_CS_CLK_ENABLE();
	
		//---SPI_SCK и SPI_MISO-------------------------------------------------------------------
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;	//
		GPIO_InitStruct.Pull      = GPIO_PULLDOWN;		//
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;	//	
		GPIO_InitStruct.Alternate = GPIO_AF_ENC_SCK;		// !!!!!!!

		//---SPI_SCK-------------------------------------------------------------------------------
		GPIO_InitStruct.Pin       = GPIO_PIN_ENC_SCK;
		HAL_GPIO_Init(GPIO_ENC_SCK, &GPIO_InitStruct);	
	
		//---SPI_MISO------------------------------------------------------------------------------
		GPIO_InitStruct.Pin       =  GPIO_PIN_ENC_MISO;
		HAL_GPIO_Init(GPIO_ENC_MISO, &GPIO_InitStruct);	
	
    // Настройка PA4 как выход (CS для AS5045)
    GPIO_InitStruct.Pin = GPIO_PIN_ENC_CS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_ENC_CS, &GPIO_InitStruct);
    ENC_CS_HIGH;//HAL_GPIO_WritePin(GPIO_ENC_CS, GPIO_PIN_ENC_CS, GPIO_PIN_SET); // Деактивация CS
		
		
    SPI_ENC_CLK_ENABLE(); // Включение тактирования SPI1

    spi->Instance = SPI_ENC;
    spi->Init.Mode = SPI_MODE_MASTER;
    spi->Init.Direction = SPI_DIRECTION_2LINES;
    spi->Init.DataSize = SPI_DATASIZE_16BIT; // 16-битный режим
    spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi->Init.CLKPhase = SPI_PHASE_2EDGE;//SPI_PHASE_1EDGE;//
    spi->Init.NSS = SPI_NSS_SOFT;
    spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;//SPI_BAUDRATEPRESCALER_8;
    spi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi->Init.TIMode = SPI_TIMODE_DISABLE;
    spi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi->Init.CRCPolynomial = 7;
    //HAL_SPI_Init(spi);
		if(HAL_SPI_Init(spi) != HAL_OK)
		{
			// Initialization Error 
			Error_Handler();
		}			

    // Включение прерываний SPI
    //HAL_NVIC_SetPriority(SPI_IRQn, 1, 0);
    //HAL_NVIC_EnableIRQ(SPI_IRQn);		
	
	/*
    // Включение тактирования SPI1 и GPIO
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Включение тактирования GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Включение тактирования SPI1

    // Настройка пинов SPI1 (PA5 - SCK, PA6 - MISO, PA7 - MOSI)
    GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 ); // Сброс режима
    GPIOA->MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1); // Альтернативный режим
    GPIOA->AFR[0] |= (0x0 << (5 * 4)) | (0x0 << (6 * 4)); // AF0 для SPI1

    // Настройка SPI1
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA; // Режим Master, делитель частоты, режим SPI
    SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH; // 8-битный режим данных
    SPI1->CR1 |= SPI_CR1_SPE; // Включение SPI	
	*/
}
//============================================================================================



//============================================================================================
uint16_t AS5045_ReadAngle(void) 
{
		uint16_t data[2] = {0, 0};
		//uint16_t command = 0xFFFF;
	
    // Активация CS
    ENC_CS_LOW;//HAL_GPIO_WritePin(GPIO_ENC, GPIO_PIN_ENC_CS, GPIO_PIN_RESET);

	
		HAL_SPI_Receive(&SPI_ENC_HANDLER, (uint8_t*)&data[0], 1, HAL_MAX_DELAY);
		HAL_SPI_Receive(&SPI_ENC_HANDLER, (uint8_t*)&data[1], 1, HAL_MAX_DELAY);
    // Отправка команды чтения (0xFFFF для чтения угла)
    //HAL_SPI_Transmit_IT(&SPI_ENC_HANDLER, (uint8_t*)&command, 1); // Запуск передачи в неблокирующем режиме
		ENC_CS_HIGH;
		
		return (data[0]>>3) & 0xFFF;	//data & 0x3FFF;
}
//============================================================================================



//===========================================================================================
//---Чтение датчика--------------------------------------------------------------------------
uint16_t Dpr_Read( void ) {
	
	uint16_t val = 0;
	uint16_t temp[2] = {0, 0};

	//__disable_irq();	

	
					ENC_CS_LOW;//HAL_GPIO_WritePin(GPIO_ENC, GPIO_PIN_ENC_CS, GPIO_PIN_RESET);//SPI_DPR_CS1_LOW;

	
					while( ( ( SPI_ENC->SR & SPI_FLAG_TXE ) == (uint16_t)RESET) || \
								 ( ( SPI_ENC->SR & SPI_FLAG_BSY ) == (uint16_t)SET) );					
					SPI_ENC->DR = 0xAAAA;
					while( ( (SPI_ENC->SR & SPI_FLAG_TXE) == (uint16_t)RESET ) ||\
								 ( (SPI_ENC->SR & SPI_FLAG_RXNE) == (uint16_t)RESET )  );
					temp[0] = SPI_ENC->DR;
					
					//----16-31й биты---
					while( ( ( SPI_ENC->SR & SPI_FLAG_TXE ) == (uint16_t)RESET) || \
								 ( ( SPI_ENC->SR & SPI_FLAG_BSY ) == (uint16_t)SET) );						
					SPI_ENC->DR = 0xAAAA;
					while( ( (SPI_ENC->SR & SPI_FLAG_TXE) == (uint16_t)RESET ) ||\
								 ( (SPI_ENC->SR & SPI_FLAG_RXNE) == (uint16_t)RESET )  );
					temp[1] = SPI_ENC->DR;				
					
					// из вычитанных 32х бит, забираем 12
					val = (temp[0]>>3) & 0xFFF;					
					//__ASM("nop");



/*
					// Отправка команды чтения (0xFFFF для чтения угла)
					while (!(SPI1->SR & SPI_SR_TXE)); // Ожидание готовности передатчика
					SPI1->DR = 0xFFFF; // Отправка команды

					// Ожидание завершения передачи
					while (!(SPI1->SR & SPI_SR_RXNE)); // Ожидание данных
					val = SPI1->DR; // Чтение данных	
					*/
					
					
					ENC_CS_HIGH;//HAL_GPIO_WritePin(GPIO_ENC, GPIO_PIN_ENC_CS, GPIO_PIN_SET);//SPI_DPR_CS1_HIGH;		
				
	//__enable_irq();
	return val;
}
//===========================================================================================



//============================================================================================
//void ADC_Config(ADC_HandleTypeDef *adc)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	ADC_ChannelConfTypeDef sConfig = {0};

//	//--------------------------------------------------------------------------------------
//  // Peripheral clock enable 
//  ADC_POT_CLK_ENABLE();

//  // ADC1 interrupt Init 
//  HAL_NVIC_SetPriority(ADC_IRQn, 2, 2);
//  HAL_NVIC_EnableIRQ(ADC_IRQn);
//	
//  // Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//  adc->Instance = ADC_POT;
//  adc->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//  adc->Init.Resolution = ADC_RESOLUTION_12B;
//  adc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  adc->Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
//  adc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  adc->Init.LowPowerAutoWait = DISABLE;
//  adc->Init.LowPowerAutoPowerOff = DISABLE;
//  adc->Init.ContinuousConvMode = DISABLE;
//  adc->Init.DiscontinuousConvMode = DISABLE;
//  adc->Init.ExternalTrigConv = ADC_SOFTWARE_START;//ADC_EXTERNALTRIGCONV_T1_TRGO;//
//  adc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;//ADC_EXTERNALTRIGCONVEDGE_NONE;
//  adc->Init.DMAContinuousRequests = DISABLE;
//  adc->Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  if (HAL_ADC_Init(adc) != HAL_OK)
//  {
//    Error_Handler();
//  }
//	//--------------------------------------------------------------------------------------
//	
//	
//	
//	//--------------------------------------------------------------------------------------
//  // Configure for the selected ADC regular channel to be converted
//  sConfig.Channel = ADC_CHANNEL_0;
//  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
//  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_7CYCLES_5;//ADC_SAMPLETIME_13CYCLES_5;
//  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//	//--------------------------------------------------------------------------------------

//	HAL_ADCEx_Calibration_Start(adc);
//	
//	
//	//--------------------------------------------------------------------------------------
//  if(adc->Instance==ADC_POT)
//  {	
//		// Peripheral clock enable 
//		GPIO_POT_CLK_ENABLE();
//		
//		// ADC GPIO Configuration 
//		// PA0     ------> ADC_IN0
//		GPIO_InitStruct.Pin = GPIO_PIN_POT;
//		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		HAL_GPIO_Init(GPIO_POT, &GPIO_InitStruct);	
//	}
//	//--------------------------------------------------------------------------------------

//	
//		//---включаем ADC1
//	__HAL_ADC_ENABLE( adc );
//	//__HAL_ADC_ENABLE_IT( adc, ADC_IT_EOC );
//	
//	//---запускаем ADC1
//	//HAL_ADC_Start_IT(&ADC_POT_HANDLER);
//	
//}
//============================================================================================



//===старт ADC===============================================================================
//void ADC_Pot_Start(void)
//{
//	HAL_ADC_Start(&hadc);
//}
//===========================================================================================



//===включение ADC===========================================================================
//void ADC_Pot_ENABLE(void)
//{
//	ADC_Enable(&hadc);	
//}
//===========================================================================================



//===выключение ШИМ==========================================================================
//void TIM_Pwm_DISABLE(void)
//{
//	__HAL_TIM_MOE_DISABLE(&htim1);	
//}
//===========================================================================================



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



////============================================================================================
//void TIM_Pwm_Config(TIM_HandleTypeDef *tim)
//{
//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};
//  TIM_OC_InitTypeDef sConfigOC = {0};
//  //TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
//	//GPIO_InitTypeDef GPIO_InitStruct = {0};

//  // Peripheral clock enable 
//  TIM_PWM_CLK_ENABLE();

//	
//  // TIM1 interrupt Init 
//  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1, 1);
//  HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);	
//	
//	//---Base Init--------------------------------------------------------------------------
//  tim->Instance								= TIM_PWM;
//  tim->Init.Prescaler					= 5;//4;//3;//0;
//  tim->Init.CounterMode				= TIM_COUNTERMODE_UP;	
//  tim->Init.Period						= TIM_PWM_ARR;
//  tim->Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
//  tim->Init.RepetitionCounter	= 0;
//  tim->Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_ENABLE;
//  if ( HAL_TIM_Base_Init( tim ) != HAL_OK ) { Error_Handler(); }
//	//--------------------------------------------------------------------------------------

//	
//	//---ClockSource------------------------------------------------------------------------
//  sClockSourceConfig.ClockSource	= TIM_CLOCKSOURCE_INTERNAL;
//  if ( HAL_TIM_ConfigClockSource( tim, &sClockSourceConfig ) != HAL_OK )	{ Error_Handler(); }
//  if ( HAL_TIM_PWM_Init( tim ) != HAL_OK )																{ Error_Handler(); }
//	//--------------------------------------------------------------------------------------
//	
//	
//	//---Synchronization--------------------------------------------------------------------
//  sMasterConfig.MasterOutputTrigger	= TIM_TRGO_UPDATE;
//  sMasterConfig.MasterSlaveMode			= TIM_MASTERSLAVEMODE_DISABLE;//TIM_MASTERSLAVEMODE_ENABLE;
//  if ( HAL_TIMEx_MasterConfigSynchronization( tim, &sMasterConfig ) != HAL_OK ) { Error_Handler(); }
//	//--------------------------------------------------------------------------------------
//	
//	//---OC1--------------------------------------------------------------------------------
//	//---OCMode	= TIM_OCMODE_TOGGLE, OCPolarity	= TIM_OCPOLARITY_LOW, OCNPolarity	= TIM_OCNPOLARITY_LOW
//  sConfigOC.OCMode				= TIM_OCMODE_TOGGLE;
//  sConfigOC.Pulse					= 1;//TIM_PWM_CCR;
//  sConfigOC.OCPolarity		= TIM_OCPOLARITY_HIGH;//TIM_OCPOLARITY_LOW;//
//  sConfigOC.OCNPolarity		= TIM_OCPOLARITY_HIGH;//TIM_OCNPOLARITY_LOW;//
//  sConfigOC.OCFastMode		= TIM_OCFAST_DISABLE;
//  sConfigOC.OCIdleState		= TIM_OCIDLESTATE_RESET;//TIM_OCIDLESTATE_SET;
//  sConfigOC.OCNIdleState	= TIM_OCIDLESTATE_RESET;//TIM_OCNIDLESTATE_SET;
//  if ( HAL_TIM_PWM_ConfigChannel( tim, &sConfigOC, TIM_CHANNEL_1 ) != HAL_OK ) { Error_Handler(); }
//	//--------------------------------------------------------------------------------------	
//	
//	
//	//---Complementary output enable--------------------------------------------------------
//	tim->Instance->CCER |= TIM_CCER_CC1E;

//}



//===включение TIM1==========================================================================
//void TIM_Pwm_Start(void)
//{
//	__HAL_TIM_ENABLE(&TIM_PWM_HANDLER);
//}
//===========================================================================================



//===включение ШИМ==========================================================================
//void TIM_Pwm_ENABLE(void)
//{
//	//HAL_TIM_PWM_Start(&TIM_PWM_HANDLER, TIM_CHANNEL_1);
//	HAL_TIM_OC_Start(&TIM_PWM_HANDLER, TIM_CHANNEL_1);
//	//__HAL_TIM_MOE_ENABLE(&TIM_PWM_HANDLER);	
//	__HAL_TIM_MOE_ENABLE(&TIM_PWM_HANDLER);										// подключим выход таймера
//}
//===========================================================================================



//===выключение ШИМ==========================================================================
//void TIM_Pwm_DISABLE(void)
//{
//	
//	//HAL_TIM_PWM_Stop(&TIM_PWM_HANDLER, TIM_CHANNEL_1);
//	HAL_TIM_OC_Stop(&TIM_PWM_HANDLER, TIM_CHANNEL_1);
//	//__HAL_TIM_MOE_DISABLE(&TIM_PWM_HANDLER);	
//}
//===========================================================================================



//===задание количества повторений===========================================================
//void TIM_Load_RCR(uint8_t cnt)
//{
//	TIM_PWM->RCR = cnt;	
//}
//===========================================================================================



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


#if	(WORK_MODE == WORK_MODE_NORMAL)
//===окончание приема пакета данных===========================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{

  if(huart == &UART_HANDLER) {

		//---принят первый байт------------------------------------------------------------------------		
    if( firstByteWait != 0 )										
		{
				firstByteWait = 0;
				HAL_UART_Receive_IT (&UART_HANDLER, (uint8_t *)(BufferRX+1),Packet.lengthRx-1);	// запуск приема остальных байтов команды
    }
		
		//---принят весь пакет-------------------------------------------------------------------------
    else																				
		{  
				// контрольный байт плохой (для любого пакета)
				if( !( ((BufferRX[0] ^ BufferRX[1] ^ BufferRX[2] ^ 0xE5) == BufferRX[3]) ||\
						((BufferRX[0] ^ 0xE5) == BufferRX[1]) ) )
				{
						// отработка ошибки
						// ...
					
						//---запуск приема первого байта пакета с коммандой----------------------------------------
						StartRx(BufferRX);					
				}
				// хороший
				else
				{
					
						// дешифрируем комманду, сформируем ответ
						MakeAnswer(&Packet, BufferRX);				
				
				}
	
    }
  }
}
//============================================================================================
#endif



//===завершение передачи данных===============================================================
/*void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

  if(huart == &huart1) {

    dataTransmitted=1;

    if( dataReceived != 0 ) {
      HAL_UART_Transmit_IT(&huart1, str, 1);
      dataReceived=0;
      dataTransmitted=0;
    }
  }
}*/
//============================================================================================



//============================================================================================
void SPI1_IRQHandler(void) 
{
    HAL_SPI_IRQHandler(&SPI_ENC_HANDLER); // Обработка прерываний SPI HAL
}
//============================================================================================



//============================================================================================
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) 
{
    // Передача завершена, запускаем прием данных
    HAL_SPI_Receive_IT(&SPI_ENC_HANDLER, (uint8_t*)&as5045_angle, 1); // Запуск приема в неблокирующем режиме
}
//============================================================================================



//============================================================================================
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) 
{
    // Прием данных завершен
    as5045_angle &= 0x3FFF; // Маскирование 14 бит (данные угла)
    as5045_data_ready = 1; // Установка флага готовности данных

    // Деактивация CS
    ENC_CS_HIGH;//HAL_GPIO_WritePin(GPIO_ENC, GPIO_PIN_ENC_CS, GPIO_PIN_SET);
}
//============================================================================================
