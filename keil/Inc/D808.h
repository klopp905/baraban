#ifndef __D808_H
#define __D808_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "structs.h"

//#define		PI_MUL3		9.4247779607693797153879301498385f
#define		PI_MUL3					(3.0f * 180.0f)	

#define		POS_CUTOFF			(4.000f)
#define		SPEED_CUTOFF		(0.05f)//(0.05f)//

#define		STEPS_PER_REV		200
#define		ADC_MAX					4095
#define		ADC_MIN					0
#define		SIZE						16.0f																									// количество секторов на барабане
#define		SECTORS_CNT			(SIZE * 3)																								// количество секторов на диапазоне АЦП
#define		SECTOR_LENGTH		(float)( (float)(ADC_MAX - ADC_MIN)/(float)SECTORS_CNT )	// длина одного сектора в единицах АЦП

//#define NUM_SECTORS 26
#define MAX_ANGLE 4096  // 12 бит = 4096 значений

//============================================================================================

//---STEP-DIR линия PUL---------------------------------------------------
#define		GPIO_PUL								GPIOA
#define		GPIO_PIN_PUL						GPIO_PIN_8
#define		GPIO_PUL_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define		GPIO_AF_PUL							GPIO_AF2_TIM1

#define		PUL_ENABLE						  GPIO_PUL->BRR   = GPIO_PIN_PUL
#define		PUL_DISABLE						  GPIO_PUL->BSRR  = GPIO_PIN_PUL

//---STEP-DIR линия DIR(выход - ключ с инверсией, активный - минус)-------
#define		GPIO_DIR								GPIOB
#define		GPIO_PIN_DIR						GPIO_PIN_14
#define		GPIO_DIR_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

//#define		DIR_ENABLE						  GPIO_DIR->BSRR = GPIO_PIN_DIR
//#define		DIR_DISABLE						  GPIO_DIR->BRR  = GPIO_PIN_DIR
#define		DIR_PIN_HI				  		GPIO_DIR->BSRR  = GPIO_PIN_DIR
#define		DIR_PIN_LO				  		GPIO_DIR->BRR   = GPIO_PIN_DIR
#define		DIR_OUT_ON				  		DIR_PIN_LO
#define		DIR_OUT_OFF				  		DIR_PIN_HI

//---STEP-DIR линия ENA(выход - ключ с инверсией, активный - плюс)--------
#define		GPIO_ENA								GPIOB
#define		GPIO_PIN_ENA						GPIO_PIN_15
#define		GPIO_ENA_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

//#define		ENA_ENABLE						  GPIO_ENA->BSRR  = GPIO_PIN_ENA/*cs*///
//#define		ENA_DISABLE						  GPIO_ENA->BRR   = GPIO_PIN_ENA/*cs*///
#define		ENA_PIN_HI				  		GPIO_ENA->BSRR  = GPIO_PIN_ENA
#define		ENA_PIN_LO				  		GPIO_ENA->BRR   = GPIO_PIN_ENA
#define		ENA_OUT_ON				  		ENA_PIN_LO//ENA_PIN_HI//
#define		ENA_OUT_OFF				  		ENA_PIN_HI//ENA_PIN_LO//

//---STEP-DIR линия Fault(вход: высокий - есть, низкий - нет)-------------
#define		GPIO_FLT								GPIOB
#define		GPIO_PIN_FLT						GPIO_PIN_12
#define		GPIO_FLT_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

//---STEP-DIR линия In Position(вход: высокий - есть, низкий - нет)-------
#define		GPIO_INP								GPIOB
#define		GPIO_PIN_INP						GPIO_PIN_13
#define		GPIO_INP_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

//============================================================================================

//#define		K_DRUM					(50.0f * 5.917f)
//#define		K_DRUM_DIV16		(K_DRUM / 16.0f)


//=======Структура параметров барабана====================================== 
struct  TDrumParams{
		uint16_t								Position;				//	адрес сектора барабана										 
		uint16_t								Speed;					//	скорость вращения
		uint16_t								SensorRef;			//	величина ДОС привязанная к нулю
		uint16_t								Sensor;					//	величина ДОС текущая
		uint16_t								StepsPerSector;	//	количество шагов ШД на один сектор барабана
		uint8_t									flagBusy;				//  барабан занят
};




//============================================================================================
void D808_Config(void);
void D808_SpeedMotor(float speed);
//void D808_StepMotor(uint8_t steps, uint8_t dir);
//void D808_RevMotor(uint16_t revCnt, uint8_t dir);
//void D808_RevGear(uint16_t revCnt, uint8_t dir);
//void D808_RevDrum(uint16_t revCnt, uint8_t dir);
//void D808_RevDrumDiv16(uint16_t revCnt, uint8_t dir);
//void D808_GetMotorStepsToDrumStep(uint16_t steps);
//void D808_DrumStep32(uint16_t stepCnt, uint8_t dir);
//void D808_GetNearestPosition(Tdrv	*drv);
//void D808_GetDrumAddress(Tdrv	*drv);
void D808_SetSector(Tdrv	*drv, uint16_t sector);
void D808_GetDrumAngle(Tdrv	*drv);
float D808_GetDrumLinAngle(float	adcVal);
void D808_Reg(void);
uint8_t D808_CheckInpos(void);
void D808_ReadPosition(void);
//int16_t findMin(int16_t a, int16_t b, int16_t c);
float findNearestAngle( uint16_t adr ); 
//============================================================================================



#ifdef __cplusplus
}
#endif

#endif /* __D808_H */
