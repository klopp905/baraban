#ifndef __DSP_CMD_H
#define __DSP_CMD_H

//#include	"dsp_cmd.h"
//#include "MDR1901VC1T.h"
//#include "config.h"

/* void archEnableInt (void); */
#define archEnableInt() __enable_irq()//asm(bfclr #0x0300,SR)

/* void archDisableInt (void); */
#define archDisableInt() __disable_irq()//asm(bfset #0x0300,SR)

/* void archSetNoSat (void); */
//#define archSetNoSat() __NOP()//asm(bfclr #0x10,OMR)

/* void archSetSat32 (void); */
//#define archSetSat32() __NOP()//asm(bfset #0x10,OMR)

#define	bitClear(mask,data)		data &= (~(mask))
#define	bitSet(mask,data)		data |= (mask)


/* Convert int/float to Frac16; constant x generates compile time constant */
#define FRAC16(x) ((Frac16)((x) < 1 ? ((x) >= -1 ? ((Frac16)((x)*0x8000)) : ((Frac16)0x8000)) : ((Frac16)0x7FFF)))
/* Convert int/float to Frac32; constant x generates compile time constant */
#define FRAC32(x) ((Frac32)((x) < 1 ? ((x) >= -1 ? ((Frac32)((x)*0x80000000)) : ((Frac32)0x80000000)) : ((Frac32)0x7FFFFFFF)))


//typedef   signed          int Frac32;
//typedef           short int Word16;
typedef   unsigned        short int UWord16;
typedef   signed          short int Frac16;

#ifdef	NOT
#define	__STATIC_INLINE		__inline
__attribute__( ( always_inline ) ) __STATIC_INLINE void NVIC_EnableIRQ_my(IRQn_Type IRQn)
{
  NVIC->ISER[((uint32_t)(IRQn) >> 5)] |= (1 << ((uint32_t)(IRQn) & 0x1F)); /* enable interrupt */
}


__attribute__( ( always_inline ) ) __STATIC_INLINE int L_mult(register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	register int sresult0;
	//asm(.optimize_iasm on);
	__asm{
		lsls		sinp1,sinp1,#16
		lsls		sinp2,sinp2,#16
		smull sresult0, sresult, sinp1,sinp2
		lsls		sresult,sresult,#1
	};
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}
#endif

//__attribute__( ( always_inline ) ) __STATIC_INLINE int L_mult_ls(register int sinp1, register int sinp2)
//{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
//	register int sresult;
//	register int sresult0;
//	//asm(.optimize_iasm on);
//	__asm{
//		//lsls		sinp1,sinp1,#16
//		lsls		sinp2,sinp2,#16
//		smull sresult0, sresult, sinp1,sinp2
//		lsls		sresult,sresult,#1
//	};
//	
//	///sresult = (sresult << 32)  + sresult0
//	//asm(.optimize_iasm off);
//	return sresult;
//}

#ifdef	NOT
__attribute__( ( always_inline ) ) __STATIC_INLINE short mult_r(register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	register int sresult0;
	//asm(.optimize_iasm on);
	__asm{
		lsls		sinp1,sinp1,#16
		lsls		sinp2,sinp2,#16
		smull sresult0, sresult, sinp1,sinp2
		lsls		sresult,sresult,#1
		lsrs		sresult,sresult,#16
	};
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int Lmult_r(register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	register int sresult0;
	//asm(.optimize_iasm on);
	__asm{
		lsls		sinp1,sinp1,#16
		lsls		sinp2,sinp2,#16
		smull sresult0, sresult, sinp1,sinp2
		lsls		sresult,sresult,#1
		//lsrs		sresult,sresult,#16
	};
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short mac_r(register int acc, register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return (acc + Lmult_r(sinp1,sinp2)) >> 16;
}
#endif
__attribute__( ( always_inline ) ) __STATIC_INLINE short negates(register short sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return (~sinp1);
}
#ifdef	NOT
__attribute__( ( always_inline ) ) __STATIC_INLINE short round(register int sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	//return (sinp1);
	register int sresult;

	sresult = sinp1;
	
	__asm{
		lsrs		sresult,sresult,#16
		//ssat	sresult,16,sresult
	};
	
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_sub(register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;

  sresult = sinp1 - sinp2;
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_add(register int sinp1, register int sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;

  sresult = sinp1 + sinp2;
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short add(register short sinp1, register short sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register short sresult;

  sresult = sinp1 + sinp2;
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short sub(register short sinp1, register short sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register short sresult;

  sresult = sinp1 - sinp2;
	
	//L_mac(sresult);
	__asm{
	SSAT		sresult
	};
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}
#endif
__attribute__( ( always_inline ) ) __STATIC_INLINE short div_s4q(register short sinp1, register short sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;

	if(sinp1 >= sinp2)
		return	FRAC16(1.0);
	else
    sresult = (sinp1 << 15) / sinp2;
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}
#ifdef	NOT
__attribute__( ( always_inline ) ) __STATIC_INLINE short div_s(register short sinp1, register short sinp2)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;

	if(sinp1 >= sinp2)
		return	FRAC16(1.0);
	else
    sresult = (sinp1 << 15) / sinp2;
	
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_shl(register int sinp1, register short int shift)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	
	sresult = sinp1 << shift;
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_shr(register int sinp1, register short int shift)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	
	sresult = sinp1 >> shift;
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short shl(register short sinp1, register short int shift)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register short sresult;
	
	sresult = sinp1 << shift;
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short shr(register short sinp1, register short int shift)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register short sresult;
	
	sresult = sinp1 >> shift;
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short abs_ss(register short sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register short sresult;
	
	sresult = abs(sinp1);
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}
#endif

#ifdef	NOT
__attribute__( ( always_inline ) ) __STATIC_INLINE int L_abs(register int sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	
	sresult = abs(sinp1);
	///sresult = (sresult << 32)  + sresult0
	//asm(.optimize_iasm off);
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short extract_l(register int sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	sresult = sinp1;
	
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE short extract_h(register int sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;

	sresult = sinp1;
	
	__asm{
		lsrs		sresult,sresult,#16
	};
	
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_deposit_h(register short sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	sresult = sinp1;
	
	__asm{
		lsls		sresult,sresult,#16
	};
	
	return sresult;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE int L_deposit_l(register short sinp1)
{
/*  Defn: Multiply two 16-bit fractional values,
 *        round into a 16-bit fractional result.
 *        Saturates only for the case of $8000 x $8000.
 *        When an accumulator is the destination, zeroes out the LSP portion.
 *  
 *  Hawk V2 instruction syntax:  mpyr FFF1,FFF1,FFF
 *            Allowed src regs:  FFF1
 *            Allowed dst regs:  FFF
 *  
 *  Assumptions: OMR's SA bit was set to 1 at least 3 cycles before this code.
 *               (i.e. saturation on data alu results enabled)
 *  Assumptions: OMR's R  bit was set to 1 at least 3 cycles before this code.
 *               (i.e. 2's complement rounding, not convergent rounding)
 */
	register int sresult;
	sresult = sinp1;
	
	
	return sresult;
}
#endif
#endif
