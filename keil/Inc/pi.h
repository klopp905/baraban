/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PI_H
#define __PI_H

//#include "constants.h"
#include "structs.h"
#include "main.h"

/*
// Активное сопротивление фазы (Ом)
#define	RS				0.15f

// Индуктивность фазы (Гн)
#define	L_PH			0.000075f

// Напряжение звена постоянного тока (В)
#define	UDC				27.0f

// Частота ШИМ (Гц)
#define	FPWM			8000.0f

// Частота среза
//#define	PI_MUL2		6.283185307179586476925286766559f
#define	FC				848.0f
#define	WC				PI_MUL2 * FC

// Коэффициент усиления инвертора(ШИМа)
#define	KINV			UDC / SQRT3

// Коэффициент ОС по току
//#define	SQRT3			1.7320508075688772935274463415059f
#define	KOCI			1.0f

// Коэффициент интегральной составляющей
#define	DQ_KI				( WC * RS )/( KINV * KOCI * FPWM )

// Коэффициент пропорциональной составляющей
#define	DQ_KP				( WC * L_PH )/( KINV * KOCI )

// Коэффициент интегральной составляющей регулятора скорости
#define	SPD_KI				0.0005//0.0002f//0.0001f
// Коэффициент пропорциональной составляющей регулятора скорости
#define	SPD_KP				0.008//0.01f//0.005f

// Коэффициент пропорциональной составляющей регулятора положения
#define	POS_KP				0.00f

// Коэффициент интегральной составляющей задатчика интенсивности
#define	POS_ZI_KI				4.000f//0.0001f


//===========================================================================================
*/
#define FREQ_HZ    1000
#define SPEED_FILTER_TAU		(1.0e-2f)
#define SPEED_FILTER_COEFF	(1.0f/(SPEED_FILTER_TAU*FREQ_HZ + 0.5f))


#define	POS_MAX				2729.000f
#define	SPEED_MAX			1.000f

void Pi_Calc(Tpi * pi, float Ref, float Input);
void Pi_Reset(Tpi * pi);
void Zi_Calc( Tzi * zi, float Uin );
														
//===========================================================================================




#endif /* __PI_H */
