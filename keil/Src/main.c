#include "main.h"
#include "d808.h"
#include "periph.h"
#include "stepdir.h"
#include "delays.h"
#include "buttons.h"
#include "flash.h"

#include "protocol.h"
#include "pi.h"
#include <math.h>



//#define		K_DRUM									(50.0f * 5.917f)
//#define		K_DRUM_DIV16						(K_DRUM / 16.0f)

//===Private function prototypes==========================================
void SystemClock_Config(void);
//========================================================================

#if	(WORK_MODE == WORK_MODE_FM)
		#include "PC_M1.h"	
		uint32_t TestCnt = 0;
		float	TestValue, TestValueRef = 0;
		float RefPos;	
#endif	

uint8_t BufferTX[5];			// на передачу
uint8_t BufferRX[5];			// на прием
uint8_t	StatusRegister = 0;

struct	TPacket	Packet = {0, 0, BufferTX, &StatusRegister, 0};


//uint8_t dataReceived=0; 		// признак данное получено
//uint8_t dataTransmitted=1;	// признак данное передано


uint8_t firstByteWait = 1;		// признак ожидание первого байта (1 - ждем, счетчик в нуле; 0 - тикаем счетчиком)
uint8_t confirmWait = 1;			// признак ожидание квитирования
uint8_t positionWait = 1;			// признак ожидание позиции

Tdrv		Drive1 = TDRV_DEFAULTS;
Tbutton	But1   = {0, 0, 0, 50, GPIO_BUT1, GPIO_PIN_BUT1};//TBUTTON_DEFAULTS;
//float		Pos;
//		RefPos;
float ang;

//uint16_t	Position = 0;

uint8_t dir;
//uint16_t	a = 0, noise = 5;
//float	b = -255.0f;
//float	Ref = 0, Pos = 0;

float		K = 1800.0f;
uint16_t	arr, gate = 30;
float		ref, pos, rem;
float speed;
float K_Speed = 1.0f;
float deadzone = 0.5f;
float hysteresis = 5.0f; // Ширина гистерезиса
float lastOutput = 0.0f;

Tflag		flagSetpos  = TFLAG_DEFAULTS;
uint8_t	flagButtons = 0;
uint8_t	flagAS5045 = 0;
uint8_t	flagStartupDelay = 0;

volatile uint16_t as5045_angle = 0; // Глобальная переменная для хранения угла
volatile uint8_t as5045_data_ready = 0; // Флаг готовности данных
float PosAdc;


//========================================================================
int main(void)
{

  HAL_Init();										// Reset of all peripherals, Initializes the Flash interface and the Systick.
	__HAL_DBGMCU_FREEZE_TIM1();		// Останавливаем таймер в паузах при отладке
  SystemClock_Config();					// Configure the system clock to 48 MHz
	
	Drive1.PosDrumZeroAng = ReadI16FromFlash(FLASH_USER_START_ADDR);

	AS5045_Config(&SPI_ENC_HANDLER);
	
	
  //---Настройка TIM------------------------------------------------------
  //D808_Config();
	StepDir_Init();
	
	// GPIO кнопки и светодиодик
	GPIO_Config();

#if	(WORK_MODE == WORK_MODE_FM)
		USART_FM_Config();
		PC_M1_PE_Init();
		USART_FM_ENABLE_IT();		
#else
		UART_Config(&UART_HANDLER);
		
		//---запуск приема первого байта пакета с коммандой---------------------
		Packet.lengthRx = SIZE_COMMAND;
		StartRx(BufferRX);			
#endif	

	
	//---включение TIM1-----------------------------------------------------
	//TIM_Pwm_Start();
	//TIM_Pwm_ENABLE();
	
	//DRIVE_ON(&Drive1);
	
HAL_TIM_PWM_Start(&TIM_PWM_HANDLER, TIM_CHANNEL_1);

if(HAL_GPIO_ReadPin(GPIO_FLT, GPIO_PIN_FLT) == GPIO_PIN_SET)
{
	Drive1.SectorsCount = 32;
	Drive1.SectorSize = (360.0f/(float)Drive1.SectorsCount); 
}
else 
{
	Drive1.SectorsCount = 26;
	Drive1.SectorSize = (360.0f/(float)Drive1.SectorsCount);//26;
}
	while (1)
  {
		
					if( flagStartupDelay == 1 )
					{
						//HAL_TIM_PWM_Start(&TIM_PWM_HANDLER, TIM_CHANNEL_1);	
					}		
		
      //--------------кнопки 10 мс----------------------------------------
      if(flagButtons == 1)
			{          
        
        ReadButton(&But1);
        flagButtons = 0;
      } 
		
			//--------------датчик угла-----------------------------------------
      if(flagAS5045 == 1)
			{  			
				// Запуск чтения угла
				//as5045_angle = AS5045_ReadAngle();
				//angle_degrees = (as5045_angle / 4096.0f) * 360.0f;
				PosAdc = (float)( ( (int16_t)Drive1.PosADC/*Drive1.PosADCFiltered*/ - Drive1.PosDrumZeroAng + MAX_ANGLE ) % MAX_ANGLE );	// с офсетом
				//Drive1.PosDrumAdr = (uint16_t)( (PosAdc * (float)NUM_SECTORS) / (float)MAX_ANGLE );											// адрес сектора	
				 
				
				Drive1.PosDrumAng = (PosAdc / 4096.0f) * 360.0f;
				
				Drive1.PosDrumAdr = (uint16_t)(Drive1.PosDrumAng / Drive1.SectorSize);
				//Drive1.PosDrumAdr = (uint16_t)(Drive1.PosDrumAng / Drive1.SectorSize);
	
				Drive1.RefDrumAng = ( (float)Drive1.RefDrumAdr * Drive1.SectorSize);  //
									

        flagAS5045 = 0;
      } 

			
#if	(WORK_MODE == WORK_MODE_NORMAL)		
			//...<<< Протокол >>>........................................................................
			//...выполнение отправки, если есть запрос на передачу.......................................
			if( Packet.sendReq )
			{				
					SendPacket(&Packet);			// отправка 
				
					switch( Packet.bufTx[0] )
					{
					
						case	PRT_ANS1_ACCEPTED:
						{
							confirmWait = 0;										//...запустим таймаут на квитирование............						
						} break;
						
						case	PRT_ANS1_BUSY:
						case	PRT_ANS1_UNKNOWN:
						case	PRT_ANS1_RESULT:
						{
								//...запуск приема первого байта пакета с коммандой..................................
							Packet.lengthRx = SIZE_COMMAND;
							StartRx(BufferRX);						
						} break;				

						case	PRT_ANS2_OK:
						case	PRT_ANS2_ERR:
						{
							StopSession(BufferRX);							//...завершаем сеанс.............................						
						} break;						
					
					}
			}
#endif			
		

  }

}
//========================================================================



//========================================================================
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  // Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

//========================================================================



//========================================================================
//This function is executed in case of error occurrence.
void Error_Handler(void)
{
  // USER CODE BEGIN Error_Handler_Debug 
  // User can add his own implementation to report the HAL error return state 
  __disable_irq();
  while (1)
  {
  }
}
//========================================================================

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

