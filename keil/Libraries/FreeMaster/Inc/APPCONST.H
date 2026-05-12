/*****************************************************************************
*
* Motorola Inc.
* (c) Copyright 2000 Motorola, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************
*
* File Name:         appconst.h
*
* Description:       Description of Application constant for appconst.c file
*
* Modules Included:  none
*                    
* 
*****************************************************************************/

#ifndef __APPCONST_H
#define __APPCONST_H

#define EXPORT extern

#ifdef __cplusplus
EXPORT "C" {
#endif

//#include "main_acim.h" /* application defines */
#include "3ph_ACIM_FR.h" 		  /* application defines */
#include "lut.h"	
	

/******************************************************************************/
/* EXTERN VARIABLES                                                           */
/******************************************************************************/
																      														
/********************* position and speed sensing - begin *********************/


/************************ SIN, COS calculation - Begin ************************/
extern const Frac16 mcgenSineTable256[257];
/************************ SIN, COS calculation - End **************************/

  
EXPORT const short	dataIndex[];
EXPORT const short	dataSize[];
EXPORT const short	dataPoshort[];
EXPORT const short	dataUnit[];
EXPORT const short	*pdataMin[];
EXPORT const short	*pdataMax[];
EXPORT const short	dataDefined[];
EXPORT const short	dataInc[];
EXPORT const short 	*ppdataInc[];
EXPORT const short	*pdataLimMin[];
EXPORT const short	*pdataLimMax[];


EXPORT const short	cIdent;
EXPORT const char*	dataStr[];


EXPORT const short 	cnst0001;
EXPORT const short 	cnst6000;
EXPORT const short 	cnst0000;
EXPORT const short 	cnst0100;
EXPORT const short 	cnst0250;
EXPORT const short 	cnst0025;
EXPORT const short 	cnst0005;
EXPORT const short 	cnstffff;
EXPORT const short 	cnst0003;
EXPORT const short 	cnst0380;
EXPORT const short 	cnst2500;
EXPORT const short 	cnst0004;
EXPORT const short 	cnst0023;
EXPORT const short 	cnst0059;
EXPORT const short 	cnst0600;
EXPORT const short 	cnst9990;
EXPORT const short 	cnst0050;
EXPORT const short 	cnst1000;
EXPORT const short 	cnst9999;
EXPORT const short 	cnst30000;
EXPORT const short  cnst10000;
EXPORT const short  cnst0002;
EXPORT const short  cnst20000;
EXPORT const short cnst_30000;
EXPORT const short cnst_5000;
EXPORT const short cnst5000;
EXPORT const short cnst_16000;
EXPORT const short cnst16000;
EXPORT const short cnst2000;
EXPORT const short cnst_32768;
EXPORT const short cnst32767;
EXPORT const short cnst0999;
EXPORT const short cnst0200;
EXPORT const short cnst0020;
EXPORT const short cnst0012;
EXPORT const short cnst0006;
EXPORT const short cnst0030;
EXPORT const short cnst0011;
EXPORT const short cnst3000;
EXPORT const short cnst0016;
EXPORT const short cnst_8000;
EXPORT const short cnst8000;
EXPORT const short cnst36000;
EXPORT const short cnst6400;
EXPORT const short cnst_10000;
EXPORT const short cnst0360;


EXPORT const short id[];
EXPORT const short iq[];

EXPORT unsigned short MASK16[16];
EXPORT unsigned char MASK16FF[8];


EXPORT const char pSP[];
EXPORT const char pCH[];
EXPORT const char pErr[];
EXPORT const char pEr[];
EXPORT const char pPOFF[];
EXPORT const char pStOP[];
EXPORT const char pErrU[];

EXPORT const short	tblKvadr[4];
EXPORT const short	shiftKvadr[4];
EXPORT const short	maskKvadr[4];
EXPORT const short	decimKvadr[4];

EXPORT const Frac16 tblIToUDelta0002[];
EXPORT const Frac16 tblIToUDelta01[];
EXPORT const Frac16 tblIToUDelta05[];

EXPORT const float tblIToUDelta0002F[];
EXPORT const float tblIToUDelta01F[];
EXPORT const float tblIToUDelta05F[];

EXPORT const char pc[];
EXPORT const char pt[];
EXPORT const char pA[];
EXPORT const char pu[];

EXPORT const char pNULL[];

EXPORT const char pOb_min[];
EXPORT const char pHz[];
EXPORT const char pkW[];
EXPORT const char pms[];
EXPORT const char pV[];
EXPORT const char pGWh[];
EXPORT const char pMWh[];
EXPORT const char pkWh[];
EXPORT const char pOb_min_s[];
EXPORT const char pNm[];
EXPORT const char pC[];
EXPORT const char ps[];
EXPORT const char pOtEd[];
EXPORT const char pOm[];
EXPORT const char pmH[];
EXPORT const char pgrad[];

EXPORT const short	limitDataIndex[];

//EXPORT const short *pdataMenu[];
EXPORT const short *pgroupNumberMenu[];
EXPORT const short *pgroupMenu[];
EXPORT const char*	groupStr[];
EXPORT const char*	groupNumberStr[];
EXPORT const char*    dataStrNew[];
EXPORT const short	dataPointNew[];
EXPORT const spMinMax			pdataLim[];
EXPORT const sdataStruct	dataStruct[];

EXPORT lut_sPointF R4700ToCF[37];

EXPORT const short*	limitDataIndexNew[];


EXPORT char pNP[];

EXPORT const	short		hexTbl[];

EXPORT const char pOrLd[];

EXPORT const Frac16 thetaFrozenSinTbl[];

EXPORT const	int	maskFaults[];

EXPORT const unsigned char auchCRCHi[];
EXPORT const unsigned char auchCRCLo[];

EXPORT const char p_OFF[];
EXPORT const short	cnstTmp[];
EXPORT const	Word16	nSyncTbl[];
EXPORT const	sKScale	kSyncTbl[SIZE_N_SYNC_PWM];
EXPORT const	Frac16	KsyncTbl[SIZE_N_SYNC_PWM];
EXPORT const	Frac16	_kSyncTbl[SIZE_N_SYNC_PWM];
EXPORT const	Frac16	deltaThetaSyncTbl[SIZE_N_SYNC_PWM];
extern const char pP[];
extern const char pPUSK[];
extern const char pSTOP[];
extern const char pSHUNT[];
extern const	word	a_p[];
extern const	word	r_ya[];
extern const	word	A_YA[];
extern const short *pgroupMenu[];
extern const float   mcgenSineTable256F[257];

#define LENG_LIMIT_DATA (21+2)

#ifdef __cplusplus
}
#endif

#endif /* __APPCONST_H */
