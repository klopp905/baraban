#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "structs.h"

#define WORK_MODE_NORMAL        0
#define WORK_MODE_FM      			1

#define WORK_MODE               WORK_MODE_NORMAL

#define		RX_STAGE_COMMAND				0		// ждем комманду
#define		RX_STAGE_CONFIRMATION		1		// ждем квитирование на отправку ответа

//extern	struct	Tpi			pP;// П регулятор положения



#if	(WORK_MODE == WORK_MODE_FM)
		#include "PC_M1.h"
		#define TEST_PULSE_LEN       		(20000)
		#define TEST_PULSE_PERIOD       (TEST_PULSE_LEN * 2)
		
		extern UWord16 PC_M1_SCIRxFullInt; // internal status register of interrupt 
		extern UWord16 PC_M1_SCITxEmptyInt; // internal status register of interrupt 
		extern uint32_t TestCnt;
		extern float	TestValue, TestValueRef;	
		extern float RefPos;	
#endif




//extern	struct  Tanswer	answer;
extern	uint8_t BufferTX[5];
extern	uint8_t BufferRX[5];
extern  uint8_t	StatusRegister;

extern struct	TPacket						Packet;

//extern uint8_t RxStage;
//extern uint8_t RxLength;

extern	uint8_t firstByteWait;		// признак ожидание первого байта
extern	uint8_t confirmWait;			// признак ожидание квитирования
extern	uint8_t positionWait;			// признак ожидание позиции

extern	float ang;

extern	uint8_t	dir;
//extern struct  TDrumParams		DrumParams;
extern	Tdrv		Drive1;
extern  Tbutton	But1;


extern	float		K;
extern	uint16_t	arr, gate;
extern	float		ref, pos, rem;

extern	float K_Speed;
extern	float deadzone;
extern	float hysteresis;
extern	float lastOutput;

extern	Tflag	flagSetpos;
extern  uint8_t	flagButtons;
extern	uint8_t	flagAS5045;
extern  uint8_t	flagStartupDelay;

extern	volatile uint16_t as5045_angle;
extern	volatile uint8_t as5045_data_ready;
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
