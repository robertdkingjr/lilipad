/*******************************************************************************
*
* Copyright 2007-2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
****************************************************************************//*!
*
* @brief  Integrator algorithms implemented in assembler for V2
* 
*******************************************************************************/
#ifndef _GFLIB_INTEGRATOR_A32_ASMI_H_
#define _GFLIB_INTEGRATOR_A32_ASMI_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
//#include "mlib.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
   acc32_t a32Gain;
   frac32_t f32IAccK_1;
   frac16_t f16InValK_1;
} GFLIB_INTEGRATOR_T_A32;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief Scaled integrator with the bilinear transformation.  
*		   
* @param  ptr   		GFLIB_INTEGRATOR_T_A32 *psParam
*						  	- Pointer to integrator structure
*
*						GFLIB_INTEGRATOR_T_A32 data structure:						  
*                           acc32 a32Gain
*                               - gain; input parameter
*                                 in the following range
*                                 0 <= f16IGain < 65536.0    
*                           frac32_t f32IAccK_1
*                               - state variable; fractional part (lower long)
*                                 of integral part at step k-1;
*                                 can be modified outside of the function;
*                                 input/output parameter
*                                 in the following range
*                                 -1.0 <= f32IAccK_1 < 1.0                               
*							frac16_t f16InValK_1
*								- input value at the step k-1;
*                                 input/output parameter
*                                 in the following range
*                                 -1.0 <= f16InVal_1 < 1.0   
*						  
* @param  in    		Frac16 f16InVal
*                         - input value
*
* @return out			This function returns the integrated value.
*     - frac16_t value [-1;1]
*		
* @remarks Integrated input value according to equation:	
* 
* 		  f32IAccK_1 = [(f16InVal + f16InValK_1) / 2]  * a32Gain) + f32IAccK_1
* 		  f16InValK_1 = f16InVal
* 		  output = round(f32IAccK_1)
* 
* 		  THE SATURATION MUST BE TURNED OFF!
*
****************************************************************************/
extern inline frac16_t GFLIB_Integrator_F16_FAsmi(register frac16_t f16InVal, register GFLIB_INTEGRATOR_T_A32 *psParam)
{
	register acc32_t a32InVal;
	register acc32_t a32InValK_1;
	register acc32_t a32Gain;
	register acc32_t a32ResultH;
	register frac32_t f32ResultL;
	register frac16_t f16Result;
	register frac16_t f16Val;	

	asm(move.l x:(psParam)+,a32Gain);			/* a32Gain = psParam->a32Gain */
	asm(move.w x:(psParam+2),a32InValK_1);		/* a32InValK_1 = psParam->a32InValK_1 */
	asm(move.w f16InVal,x:(psParam+2));			/* psParam->a32InValK_1 = f16InVal */
	asm(asr16 f16InVal,a32InVal);				/* a32InVal = f16InVal >> 16 */
	asm(asr16 a32InValK_1);						/* a32InValK_1 = a32InValK_1 >> 16 */
	asm(add a32InValK_1,a32InVal);				/* a32InVal = a32InVal + a32InValK_1 */
	asm(move.w a32Gain.1,f16Val);				/* f16Val = a32Gain.1 */
	asm(impysu a32Gain.1,a32InVal.0,Y);			/* Y = a32Gain.1*a32InVal.0; used integer mult to create div by 2 */
	asm(imacus a32Gain.0,a32InVal.1,Y);			/* Y = a32InVal.1*a32Gain.0 + a32Gain.1*a32InVal.0; used integer mult to create div by 2 */
	asm(impyuu a32Gain.0,a32InVal.0,a32Gain);	/* a32Gain = a32Gain.0*a32InVal.0; used integer mult to create div by 2 */
	asm(impy.l a32InVal.1,f16Val,a32ResultH);	/* a32ResultH = a32InVal.1*f16Val; used integer mult to create div by 2 */
	asm(move.w a32Gain.1,f16Val);				/* f16Val = a32Gain.1 */
	asm(asr16 f16Val);							/* f16Val >> 16; defines sign of a32Gain for correct addition */
	asm(move.w f16Val,a32Gain.2);				/* a32Gain.2 = f16Val */
	asm(add Y0,a32Gain);						/* a32Gain = Lower part of a32IGain * (f16InErr + f16InErrK_1) / 2 */
	asm(asr16 Y);								/* Y >> 16 */
	asm(adc Y,a32ResultH);						/* a32ResultH = Higher part of a32IGain * (f16InErr + f16InErrK_1) / 2 */
	asm(move.l x:(psParam),f32ResultL);			/* f32ResultL = psParam-> f32IAccK_1 */
	asm(add a32Gain,f32ResultL);				/* f32ResultL = psParam-> f32IAccK_1 + lower long from the multiplication */
	asm(move.l f32ResultL,x:(psParam));			/* psParam-> f32IAccK_1 = f32ResultL */
	asm(rnd f32ResultL);						/* Rounds f32ResultL to 16 bits */
	asm(move.w f32ResultL.1,f16Result);			/* f16Result = upper 16 bits of f32ResultL */
	
	return (f16Result);
}

#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_INTEGRATOR_A32_ASMI_H_ */
