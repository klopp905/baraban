#include "protocol.h"
#include "main.h"
#include "periph.h"
#include "D808.h"

//struct  TCommand				Command = {0};
//struct  TConfirmation		Confirmation = {0};
//struct  TAnswer					Answer = {0};

//TIM_HandleTypeDef htim1;
//TIM_HandleTypeDef htim17;
//ADC_HandleTypeDef hadc;
//UART_HandleTypeDef huart1;


//========================================================================
//uint8_t	flagCommand = 0;					// флаг распознанной комманды(0 - не распознана, 1 - распознана)

//extern	uint8_t UartCommStr[5];
//extern	uint8_t str[3];

//extern  uint8_t dataReceived;
//extern  uint8_t dataTransmitted;


//extern	uint8_t buf[5];
//extern	uint8_t firstByteWait;


//extern	uint8_t	flagSetStep;

//extern  uint16_t uRev;
//extern	uint16_t	Steps;
//extern	uint8_t	Dir;


//========================================================================

//===Private function prototypes==========================================

//========================================================================






//==============================================================================
// формирование ответа
// packet - указатель на пакет на передачу
// buffer - указатель на приемный буффер
void  MakeAnswer(struct TPacket *packet, uint8_t * buffer)
{  

		uint16_t	pos = 0;
		extern uint32_t firstByteWaitCnt;
		extern uint32_t confirmWaitCnt;
	
		// дешифрируем комманду или квитирование
		switch( buffer[0] )
		{
				
		case PRT_COMM_SETPOS: 

				pos = ((buffer[1] << 8) & 0xFF00 )|( (uint16_t)buffer[2] );
				if( pos < Drive1.SectorsCount )
				{
						D808_SetSector( &Drive1, pos/*((buffer[1] << 8) & 0xFF00 )|( (uint16_t)buffer[2] )*/ );	// поместим полученное значение сектора в привод
	
						// отправляем Ans1 (ACCEPTED), ожидаем CONF_OK																		// >>>>> выполняем комманду, отправляем Ans2		
						StartSession(buffer);																															// запуск сеанса связи		
						SetAnswer1(packet, PRT_ANS1_ACCEPTED, DUMMY );																		// формируем пакет с Ans1
				}

				break;
					
		case PRT_COMM_GETPOS: 					
				// прочитать позицию	
				SetAnswer1(packet, PRT_ANS1_RESULT, Drive1.PosDrumAdr );			// формируем пакет с Ans1
					
				break;					

		// квитирование
		case PRT_CONF_OK: 
                if( packet->phase != PRT_PHASE_WAIT_CONFIRM )
                {
                        Protocol_ResetSession(packet, BufferRX);
                        break;
                }

				// запуск выполнения комманды							// >>>>>  с последующей отправкой Ans2
				// переместить барабан в позицию

				positionWait = 0;													// включим таймаут на ожидание позиции			
				firstByteWait = 1;
				firstByteWaitCnt = 0;
				confirmWait = 1;
				confirmWaitCnt = 0;
                Protocol_SetPhase(packet, PRT_PHASE_WAIT_POSITION);
		
				SetBit(&StatusRegister, PRT_WAIT_ANS2);		// запуск выполнения комманды для ANS2											

				break;	

		default:
				// неизвестная комманда
		
				//---запуск приема первого байта пакета с коммандой---------
                Protocol_ResetSession(&Packet, BufferRX);
					
		}
  
}
//==============================================================================



//==============================================================================
// задание полей ответа Ans1
void  SetAnswer1(struct TPacket *packet, uint8_t ans, uint16_t data)
{  

		packet->bufTx[0] = ans;
		packet->bufTx[1] = (uint8_t)(data >> 8);
		packet->bufTx[2] = (uint8_t)(data & 0x00FF);
		packet->bufTx[3] = packet->bufTx[0] ^ packet->bufTx[1] ^ packet->bufTx[2] ^ 0xE5;	
	
		packet->lengthTx = SIZE_ANS1;
        if( ans == PRT_ANS1_ACCEPTED )
        {
                Protocol_SetPhase(packet, PRT_PHASE_WAIT_CONFIRM);
        }
        else
        {
                Protocol_SetPhase(packet, PRT_PHASE_WAIT_COMMAND);
        }
		packet->sendReq = 1;
  
}
//==============================================================================



//==============================================================================
// задание полей ответа Ans2
void  SetAnswer2(struct TPacket *packet, uint8_t ans)
{  

		packet->bufTx[0] = ans;
		packet->bufTx[1] = packet->bufTx[0] ^ 0xE5;		
	
		packet->lengthTx = SIZE_ANS2;
        Protocol_SetPhase(packet, PRT_PHASE_WAIT_ANS2_TX);
		packet->sendReq = 1;
  
}
//==============================================================================



//==============================================================================
//---очистка буффера------------------------------------------------------------
void  ClearBuffer(uint8_t * buffer)
{  
		//answer->answer = 0;																					// очистка кода ответа
		//answer->ansReady = 0;																				// сброс флага готовности ответа
		*buffer = 0;																								// очистка приемного буффера
		*(buffer+1) = 0; 
		*(buffer+2) = 0; 
		*(buffer+3) = 0; 
		*(buffer+4) = 0;	
}
//==============================================================================



//==============================================================================
// отправка пакета в слейв
void  SendPacket(struct TPacket *packet)
{  

		//for( uint8_t i = 0; i < packet->length; i++)
		//{
		//		packet->bufTx[i] = packet->byte[i];
		//}
				
		while( HAL_UART_GetState (&UART_HANDLER) == HAL_UART_STATE_BUSY_TX ){};					// если не идёт передача
		HAL_UART_Transmit_IT(&UART_HANDLER, (uint8_t *)packet->bufTx, packet->lengthTx ); // передаем все байты пакета
		
		packet->sendReq = 0;
  
}
//==============================================================================



//==============================================================================
//---запуск приема первого байта пакета-----------------------------------------
void  StartRx(uint8_t *buffer)
{  
		ClearBuffer(buffer);																					// очистка буффера
		firstByteWait = 1;																						// ожидание первого байта
		confirmWait = 1;																						// ожидание квитирования
		HAL_UART_Receive_IT (&UART_HANDLER, (uint8_t *)buffer, 1);		// запуск приема

}
//==============================================================================



//==============================================================================
//---запуск сеанса связи--------------------------------------------------------
void  StartSession(uint8_t *buffer)
{  

		// если не ожидаем выполнения комманды И
		// сеанс связи уже не запущен	
		if( (!(StatusRegister & (1 << PRT_SESSION))) && \
				(!(StatusRegister & (1 << PRT_WAIT_ANS2)))	)
		{
			ClrRegister(&StatusRegister); 					// сбросим статусный регистр
			SetBit(&StatusRegister, PRT_SESSION);		// поставим начало сеанса связи										
		}
	
		// запуск приема первого байта пакета с ответом Ans1
		StartRx(buffer);		
}
//==============================================================================



//==============================================================================
//---разрыв сеанса связи--------------------------------------------------------
void  StopSession(uint8_t *buffer)
{  

		// завершаем общение
		ClrBit(&StatusRegister, PRT_SESSION);	
	
		// размеры пакета в исходное состояние
		Packet.lengthTx = SIZE_ANS1;
                Protocol_SetPhase(&Packet, PRT_PHASE_WAIT_COMMAND);
	
		// запуск приема первого байта пакета с коммандой	
		StartRx(buffer);		
}
//==============================================================================



//==============================================================================
//---установка бита-------------------------------------------------------------
void  SetBit(uint8_t * reg, uint8_t bit)
{  
		*reg |= (1 << bit);
}
//==============================================================================



//==============================================================================
//---сброс бита-----------------------------------------------------------------
void  ClrBit(uint8_t * reg, uint8_t bit)
{  
		*reg &= ~(1 << bit);
}
//==============================================================================



//==============================================================================
//---сброс бита-----------------------------------------------------------------
void  ClrRegister(uint8_t * reg)
{  
		*reg = 0x00;
}
//==============================================================================



//====Установка значения флага==================================================
//==============================================================================
void Protocol_SetPhase(struct TPacket *packet, prt_phase_t phase)
{
	packet->phase = phase;

	switch( phase )
	{
		case PRT_PHASE_WAIT_CONFIRM:
			packet->lengthRx = SIZE_CONFIRMATION;
			break;

		case PRT_PHASE_IDLE:
		case PRT_PHASE_WAIT_COMMAND:
		case PRT_PHASE_WAIT_POSITION:
		case PRT_PHASE_WAIT_ANS2_TX:
		default:
			packet->lengthRx = SIZE_COMMAND;
			break;
	}
}
//==============================================================================


void Protocol_ResetSession(struct TPacket *packet, uint8_t *buffer)
{
	extern uint32_t firstByteWaitCnt;
	extern uint32_t confirmWaitCnt;
	extern uint32_t positionWaitCnt;

	ClrRegister(&StatusRegister);
	Protocol_SetPhase(packet, PRT_PHASE_WAIT_COMMAND);

	firstByteWait = 1;
	firstByteWaitCnt = 0;
	confirmWait = 1;
	confirmWaitCnt = 0;
	positionWait = 1;
	positionWaitCnt = 0;
	packet->sendReq = 0;

	StartRx(buffer);
}
//==============================================================================


void  SetFlag(Tflag *flag, uint8_t value){
  
  //__disable_interrupt();  
  
  flag->oldCode = flag->Code;           // сохраним прежнее значение
  flag->Code = value;                   // запишем новое
  
  //__enable_interrupt();
}
//==============================================================================



//==============================================================================
// отправка квитирования команды в УАРТ
/*void  SendConfirmation(struct TConfirmation *confirmation)
{  
		confirmation->confirmation = PROTOCOL_CONF_OK;
		confirmation->crc = confirmation->confirmation ^ 0xE5;
		confirmation->length = 2;
	
		//UART Transmit           //  
		UartCommStr[0] = confirmation->confirmation;
		UartCommStr[1] = confirmation->crc;
				
		while( HAL_UART_GetState (&UART_HANDLER) == HAL_UART_STATE_BUSY_TX ){};
		HAL_UART_Transmit_IT(&UART_HANDLER, (uint8_t *)UartCommStr, confirmation->length); // передача всех байт 
  
}*/
//==============================================================================



//===окончание приема пакета ===========================================================
/*void Rx(UART_HandleTypeDef *huart) {


    if( firstByteWait != 0 )										// пришел первый байт
		{
      firstByteWait=0;
      HAL_UART_Receive_IT (&huart1, (uint8_t *)(buf+1), 4);	// запуск приема остальных байтов команды
    }
    else																				// принят весь пакет (5 байта)
		{  
      // проверка команды
      if ( (buf[0] == PROTOCOL_COMM_ROTATE) &&\
					((buf[0] ^ buf[1] ^ buf[2] ^ buf[3] ^ 0xE5) == buf[4]) )// команда принята правильно 
			{

				flagSetStep = 1;
				//Steps = ( (uint8_t)(buf[1] << 8) )|( (uint8_t)buf[2] );
				uRev = ( (uint8_t)(buf[1] << 8) )|( (uint8_t)buf[2] );
				Dir	=	buf[3];
				

        //// подсчет контрольного кода ответа (3 байта)
        //uint16_t sum= 0;
        //for (uint16_t i=0; i<3; i++) sum += * ((uint8_t *)(& answer) + i);
        //answer.crc = sum ^ 0xa1E3;

        //// ответ на компьютер
        //HAL_UART_Transmit_IT(&huart1, (uint8_t *)(& answer), 5); // 3 байта + 2 байта crc

        // запуск приема
        buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        firstByteWait=1;
        HAL_UART_Receive_IT (&huart1, (uint8_t *)buf, 1);
      }
      else 																		  // команда принята не правильно, ошибка
			{
        buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        firstByteWait=1;
        HAL_UART_Receive_IT (&huart1, (uint8_t *)buf, 1);  // запуск приема
      }
    }

}*/

//============================================================================================

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*

//===окончание приема пакета данных===========================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  if(huart == &huart1) {

    if( firstByteWait != 0 )										// пришел первый байт
		{
      firstByteWait=0;
      HAL_UART_Receive_IT (&huart1, (uint8_t *)(buf+1), 4);	// запуск приема остальных байтов команды
    }
    else																				// принят весь пакет (5 байта)
		{  
      // проверка команды
      if ( (buf[0] == PROTOCOL_COMM_ROTATE) &&\
					((buf[0] ^ buf[1] ^ buf[2] ^ buf[3] ^ 0xE5) == buf[4]) )// команда принята правильно 
			{

				flagSetStep = 1;
				//Steps = ( (uint8_t)(buf[1] << 8) )|( (uint8_t)buf[2] );
				uRev = ( (uint8_t)(buf[1] << 8) )|( (uint8_t)buf[2] );
				Dir	=	buf[3];
				

     //   // подсчет контрольного кода ответа (3 байта)
     //   uint16_t sum= 0;
     //   for (uint16_t i=0; i<3; i++) sum += * ((uint8_t *)(& answer) + i);
     //   answer.crc = sum ^ 0xa1E3;

        // ответ на компьютер
     //   HAL_UART_Transmit_IT(&huart1, (uint8_t *)(& answer), 5); // 3 байта + 2 байта crc

        // запуск приема
        buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        firstByteWait=1;
        HAL_UART_Receive_IT (&huart1, (uint8_t *)buf, 1);
      }
      else 																		  // команда принята не правильно, ошибка
			{
        buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        firstByteWait=1;
        HAL_UART_Receive_IT (&huart1, (uint8_t *)buf, 1);  // запуск приема
      }
    }
  }
}
*/
//============================================================================================



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
