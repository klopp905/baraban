/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
#include "periph.h"
#include "stepdir.h"
#include "protocol.h"
#include "pi.h"
#include "d808.h"
#include <math.h>
/*
#if	(WORK_MODE == WORK_MODE_FM)
		#include "PC_M1.h"
		#define TEST_PULSE_LEN       		(1000)
		#define TEST_PULSE_PERIOD       (TEST_PULSE_LEN * 3)
		
		extern UWord16 PC_M1_SCIRxFullInt; // internal status register of interrupt 
		extern UWord16 PC_M1_SCITxEmptyInt; // internal status register of interrupt 
		extern uint32_t TestCnt;
		extern float	TestValue, TestValueRef;	
		extern float RefPos;	
#endif		
*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t	firstByteWaitCnt = 0;
uint32_t	confirmWaitCnt = 0;
uint32_t	positionWaitCnt = 0;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
extern	uint8_t firstByteWait;
extern	uint8_t confirmWait;
extern	uint8_t positionWait;			// признак ожидание позиции
//extern	uint8_t buf[5];
/* USER CODE END EV */



/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
		static  uint32_t  cntReg = 0, cntButtons = 0, cntAS5045 = 0, cntStartupDelay = 0;
	
		HAL_IncTick();
		D808_ReadPosition(); 

    //------задержка включения--------------------------------------------
    if( cntStartupDelay < 10000 ) cntStartupDelay++;
    else{
      flagStartupDelay = 1;
      cntStartupDelay = 0;      
    }
	
    //------чтение кнопок-------------------------------------------------
    if( cntButtons < 10 ) cntButtons++;
    else{
      flagButtons = 1;
      cntButtons = 0;      
    } 	


    //------чтение угла---------------------------------------------------
    if( cntAS5045 < 2 ) cntAS5045++;
    else{
      flagAS5045 = 1;
      cntAS5045 = 0;      
    } 			
#if (WORK_MODE == WORK_MODE_FM)
	
    //#warning "Tune mode enabled!"
			// Generate test signals
			if( TestCnt < TEST_PULSE_LEN )
			{				
        TestValue = TestValueRef;
			}
			else
			{
        TestValue = 0;
			}

			TestCnt++;
			TestCnt %= TEST_PULSE_PERIOD;

			//RefPos = TestValue;
			
			// каждые 10мс
			if( cntReg < 10 ) cntReg++;
			else
			{
					//---регулятор-------------------------------------------------------------------------
					//---вычисление положения барабана в градусах(-540;+540)------------------------------
					//D808_GetDrumAngle(&Drive1);
					Drive1.PosDrumAng = D808_GetDrumLinAngle( Drive1.PosADCFiltered );
		
					// определим угол задания
					//Drive1.RefDrumAng = findNearestAngle( Drive1.RefDrumAdr );
					Drive1.RefDrumAng =	TestValueRef;//TestValue;
				
					//---регулятор положения---------------------------------------------------------------
					//if( fabs(Drive1.PosDrumAng) < noise )
					//{
					//		Drive1.PosDrumAng = Drive1.RefDrumAng;
					//}
					//else
					//{
						
					//}

					Pi_Calc(&Drive1.pP, Drive1.RefDrumAng, Drive1.PosDrumAng );
					//---притормозим скачок отработки------------------------------------------------------
					//Zi_Calc(&Drive1.ziP, Drive1.pP.Output);	
		
					//---загрузим в таймер-----------------------------------------------------------------
					//StepDir_Control(Drive1.ziP.Output);
					StepDir_Control(K_Speed * Drive1.pP.Output);
				
					cntReg = 0;      
			}				
			
			
			
#endif		

	
	
#if	(WORK_MODE == WORK_MODE_NORMAL)		


			// каждые 10мс
			if( cntReg < 10 ) cntReg++;
			else
			{
					//---регулятор-------------------------------------------------------------------------
					//if( flagStartupDelay == 1 )
					//{
						D808_Reg();	
					//}

					//...если был запуск выполнения комманды для ANS2..........................................
					if( StatusRegister & (1 << PRT_WAIT_ANS2) )
					{	
							if( D808_CheckInpos() == 0)
							{
									//   положение отработано
									//__HAL_TIM_MOE_DISABLE_UNCONDITIONALLY(&TIM_PWM_HANDLER);	// выключим ногу шима
									//ENA_OUT_OFF;																							// опустим ENA					
				  
									ClrBit(&StatusRegister, PRT_WAIT_ANS2);										// сбросим бит выполнения комманды для ANS2
									positionWait = 1;
									positionWaitCnt = 0;
						
									SetAnswer2(&Packet, PRT_ANS2_OK );												// формируем пакет с Ans2......................	
																																						// из main, произойдет отправка PRT_ANS2_OK....					
							}		
							else
							{
									//   положение еще отрабатывается
							}						
				
					}					
					cntReg = 0;      
			}				
	

	
		//---таймаут на обрыв между байтами----------------------------------------------------
		if( firstByteWait != 0 )  firstByteWaitCnt = 0; 
		else {
			firstByteWaitCnt++;
			if( firstByteWaitCnt >= TIMEOUT_FIRSTBYTE ) {
				// ошибка таймаута
				HAL_UART_AbortReceive_IT(&UART_HANDLER);											// остановка приема
                Protocol_ResetSession(&Packet, BufferRX);
			}
		}
	
		//---таймаут на ожидание квитирования--------------------------------------------------
		if( confirmWait != 0 )  confirmWaitCnt=0; 
		else {
			confirmWaitCnt++;
			if( confirmWaitCnt >= TIMEOUT_CONFIRM ) {
				// ошибка таймаута
				HAL_UART_AbortReceive_IT(&UART_HANDLER);											// остановка приема
                Protocol_ResetSession(&Packet, BufferRX);
			}
		}	
	
		//---таймаут на отработку положения----------------------------------------------------
		if( positionWait != 0 )  positionWaitCnt=0; 
		else {
			positionWaitCnt++;
			if( positionWaitCnt >= TIMEOUT_POSITION ) {
				// ошибка таймаута
				//HAL_UART_AbortReceive_IT(&UART_HANDLER);											// остановка приема
				ClrBit(&StatusRegister, PRT_WAIT_ANS2);							// сбросим ожидание ANS2
				SetAnswer2(&Packet, PRT_ANS2_ERR );							// формируем пакет с Ans2 ERR
				positionWait = 1;															// признак ожидание позиции
					positionWaitCnt = 0;
			}
		}		
	
#elif	(WORK_MODE == WORK_MODE_FM)
	//---FreeMaster--------------------------------------------------------------------------
		PC_M1_pcmasterdrvRecorder();		
#endif

  

}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
//===FreeMaster===============================================================	
#if	WORK_MODE == WORK_MODE_FM
	
	uint32_t tmp_flag = 0, tmp_it_source = 0;


	tmp_it_source = __HAL_UART_GET_IT_SOURCE(&UARTFM_HANDLER, UART_IT_RXNE);

	// UART in mode Receiver ---------------------------------------------------
	if( tmp_it_source != RESET )
	{
		tmp_flag = __HAL_UART_GET_FLAG(&UARTFM_HANDLER, UART_FLAG_RXNE);

		if( tmp_flag != RESET )
		{
				if (PC_M1_SCIRxFullInt)
				{
					PC_M1_pcmasterdrvIsr();
				}
		}

		tmp_flag = __HAL_UART_GET_FLAG(&UARTFM_HANDLER, UART_FLAG_ORE);

		if( tmp_flag != RESET )
		{
			__HAL_UART_CLEAR_OREFLAG(&UARTFM_HANDLER);
		}
	}

	tmp_flag = __HAL_UART_GET_FLAG(&UARTFM_HANDLER, UART_FLAG_TXE);
	tmp_it_source = __HAL_UART_GET_IT_SOURCE(&UARTFM_HANDLER, UART_IT_TXE);

	// UART in mode Transmiter -------------------------------------------------
	if((tmp_flag != RESET) && (tmp_it_source != RESET))
	{
			if (PC_M1_SCITxEmptyInt)
			{
				PC_M1_pcmasterdrvIsr();
			}
	}

//===Normal Mode==============================================================	
#elif WORK_MODE == WORK_MODE_NORMAL	
	HAL_UART_IRQHandler(&UART_HANDLER);
#endif	
}




//===========================================================================================
//void ADC1_IRQHandler(void)
//{	
//	
//	HAL_ADC_IRQHandler(&ADC_POT_HANDLER);
//	
//	//---читаем АЦП----------------------------------------------------------------------------
//	//Drive1.PositionADC = (uint16_t)HAL_ADC_GetValue(&ADC_POT_HANDLER);

//		
//}
//===========================================================================================



//===========================================================================================
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{	
		//HAL_TIM_IRQHandler( &TIM_PWM_HANDLER );	
	
		__HAL_TIM_CLEAR_IT(&TIM_PWM_HANDLER, TIM_IT_UPDATE);

	/*
		HAL_ADC_Start(&ADC_POT_HANDLER);
		//while(  HAL_ADC_GetState(&ADC_POT_HANDLER) != HAL_ADC_STATE_REG_EOC );
		Drive1.PositionADC = (uint16_t)HAL_ADC_GetValue(&ADC_POT_HANDLER);	
	
		// Вход: -682 до +682
		// Выход:
		Pi_Calc(&Drive1.pP, (float)Drive1.Ref, Drive1.Filtered - (float)Drive1.CalADC );
	
		Zi_Calc(&Drive1.ziP, Drive1.pP.Output);	
		//D808_SpeedMotor( Drive1.pP.Output );
		D808_SpeedMotor( Drive1.ziP.Output );
	*/
}
//===========================================================================================

