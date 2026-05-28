#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "structs.h"


//===протокол=================================================================================
//====Slave ==================================================================================

//---таймауты, миллисекунды-----------------------------------
#define		TIMEOUT_FIRSTBYTE							(20)		// таймаут на обрыв между байтами (миллисекунды)
#define		TIMEOUT_CONFIRM								(100)		// таймаут на ожидание квитирования (миллисекунды)
#define		TIMEOUT_POSITION							(15000)	// таймаут на ожидание квитирования (миллисекунды)


//---прием комманд от мастера---------------------------------
#define		PRT_COMM_SETPOS								0x11 // переместить барабан в позицию (адрес сектора барабана)															// аргумент	8(16)		бит	(0 - 15)
#define		PRT_COMM_GETPOS								0x12 // прочитать позицию (адрес сектора барабана)																					// аргумент	8(16)		бит	(dummy)
//#define		PRT_COMM_SETSPEED							0x13 // задать величину скорости вращения																										// аргумент	8(16)		бит	(0 - 255)
//#define		PRT_COMM_GETSPEED							0x14 // прочитать величину скорости вращения																								// аргумент	8(16)		бит	(dummy)
//#define		PRT_COMM_SETREF								0x15 // привязать величину датчика к нулевой позиции (определить нулевой сектор барабана)		// аргумент	8(16)		бит	(dummy)
//#define		PRT_COMM_GETREF								0x16 // прочитать текущую величину датчика																									// аргумент	16(16)	бит	(dummy)
//#define		PRT_COMM_SETSTEPS_PERSECTOR		0x17 // задать количество шагов ШД на один сектор барабана																	// аргумент	16(16)	бит	(расч 3698)
//#define		PRT_COMM_GETSTEPS_PERSECTOR		0x18 // прочитать количество шагов ШД на один сектор барабана																// аргумент	16(16)	бит	(dummy)
//#define		PRT_COMM_NEXT_SECTORS					0x19 // переместить барабан на N секторов вперед																						// аргумент	8(16)		бит	(0 - 15)
//#define		PRT_COMM_PREV_SECTORS					0x1A // переместить барабан на N секторов назад																							// аргумент	8(16)		бит	(0 - 15)

//---квитирование на команду от мастера(ответ Ans1)------------------------
#define		PRT_ANS1_ACCEPTED							0x21 // комманда принята к выполнению
#define		PRT_ANS1_BUSY									0x22 // предыдущая комманда еще выполняется
#define		PRT_ANS1_UNKNOWN							0x23 // неизвестная комманда
//---ответы на команду----------------------------------------
#define		PRT_ANS1_RESULT								0x24 // ответ с данными на комманду																													// данные	8(16)			бит
#define		PRT_ANS2_OK										0x25 // успешный результат выполнения комманды, после ACCEPTED															// данные	dummy(16)	бит 
#define		PRT_ANS2_ERR									0x26 // неуспешный результат выполнения комманды, после ACCEPTED														// данные	dummy(16)	бит

//---прием квитирования от мастера на квитирование с ответом--
#define		PRT_CONF_OK										0x30



#define		DUMMY													0xAAAA
#define		SIZE_COMMAND									4
#define		SIZE_CONFIRMATION							2

#define		SIZE_ANS1											4
#define		SIZE_ANS2											2



//---биты ошибок----------------------------------------------
#define		PRT_ERR_BYTE_TIMEOUT_ANS1		0x00			// таймаут при приеме байтов
#define		PRT_ERR_BYTE_TIMEOUT_ANS2		0x01			// таймаут при приеме байтов
#define		PRT_ERR_TIMEOUT_ANS1				0x02			// таймаут ответа Ans1
#define		PRT_ERR_TIMEOUT_ANS2				0x03			// таймаут ответа Ans2
#define		PRT_ERR_ANS2								0x04			// команда не выполнена
#define		PRT_SESSION									0x05			// сеанс связи активен
#define		PRT_WAIT_ANS2								0x06			// ожидание выполнения комманды
#define		PRT_WAIT_CONF								0x07			// ожидание квитирования

//===Slave ===================================================================================






//-------------------------------------
typedef enum
{
	PRT_PHASE_IDLE = 0,
	PRT_PHASE_WAIT_COMMAND,
	PRT_PHASE_WAIT_CONFIRM,
	PRT_PHASE_WAIT_POSITION,
	PRT_PHASE_WAIT_ANS2_TX
} prt_phase_t;

struct  TPacket{

	uint8_t lengthRx; 	// количество байтов в пакете
	uint8_t lengthTx; 	// количество байтов в пакете
	uint8_t *bufTx;			// указатель на буффер
	uint8_t *sr;				// указатель на регистр ошибок
  uint8_t	sendReq;		// признак требования отправки
	prt_phase_t phase;
};


//---прием-----------------------------
struct  TCommand{

	uint8_t comm; 	// 
	//uint16_t data; 	//
	uint8_t dataH; 	// 
	uint8_t dataL; 	//
	//uint8_t dataDir; 	//
	uint8_t crc; 	// контрольная сумма
  
};

//---отправка--------------------------
struct  TConfirmation{

	uint8_t confirmation; 	// 
	uint8_t crc; 	// контрольная сумма
	uint8_t length;
  
};

//---отправка--------------------------
//struct  TAnswer{

//	uint8_t answer; 	// 
//	uint8_t dataH; 	// 
//	uint8_t dataL; 	//	
//	uint8_t crc; 	// контрольная сумма
//	uint8_t length;
  
//};

//---прием-----------------------------
/*
struct  TPacketRx{

	uint8_t		code; 			// 
	uint8_t		dataH; 			  //
	uint8_t		dataL; 				//
	uint8_t		ansReady; 		// флаг готовности ответа
	
	// признаки ожидания ответа и байта
	// 0 - счетчик не тикает, 
	// 1 - счетчик тикает и каждые 20мс делается AbortReceive и установка флага ошибки
	uint8_t		ansWait;		// признаки ожидания ответа			
	uint8_t		ByteWait;		// признаки ожидания байта
	
	uint32_t	byteWaitCnt;			// счетчик таймаута на обрыв между байтами
	uint32_t	ansWaitCnt;				// счетчик таймаута на получение ответа
  
};*/

//============================================================================================




//============================================================================================
//extern	TIM_HandleTypeDef htim1;
//extern  TIM_HandleTypeDef htim17;
//extern	ADC_HandleTypeDef hadc;
//extern	UART_HandleTypeDef huart1;
//============================================================================================
void  MakeAnswer(struct TPacket * packet, uint8_t * buffer);
void  SetAnswer1(struct TPacket *packet, uint8_t ans, uint16_t data);
void  SetAnswer2(struct TPacket *packet, uint8_t ans);
void  ClearBuffer(uint8_t * buffer);
void  SendPacket(struct TPacket *packet);
void	StartRx(uint8_t *buffer);

void  StartSession(uint8_t *buffer);
void  StopSession(uint8_t *buffer);
void  Protocol_SetPhase(struct TPacket *packet, prt_phase_t phase);
void  Protocol_ResetSession(struct TPacket *packet, uint8_t *buffer);

void  SetBit(uint8_t * reg, uint8_t bit);
void  ClrBit(uint8_t * reg, uint8_t bit);
void  ClrRegister(uint8_t * reg);
void  SetFlag(Tflag *flag, uint8_t value);


//============================================================================================

//============================================================================================



#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_H */
