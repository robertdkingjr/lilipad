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
* @brief  The function computing angle of two sine waves shifted in phase one 
* 		  to each other 
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYXSHIFTED_F16_ASM_H_
#define _GFLIB_ATANYXSHIFTED_F16_ASM_H_

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
#define GFLIB_ATANYXSHIFTED_DEFAULT {32767, 32767, 0, 0, 0}

#define GFLIB_AtanYXShifted_F16_Asm(f16ArgY, f16ArgX, psParam)\
           GFLIB_AtanYXShifted_F16_FAsm(f16ArgY, f16ArgX, psParam)

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
   frac16_t       f16Ky;      
   frac16_t       f16Kx;      
   int16_t        i16Ny;      
   int16_t        i16Nx;      
   frac16_t       f16ThetaAdj;
} GFLIB_ATANYXSHIFTED_T_F16;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    The GFLIB_AtanYXShifted function computes the angle using    
*		    two sine waves. The signals should be scaled to be in 
*			the range [-1,1) (fractional range).                  
*
* @param  ptr   *psParam
*				     Pointer to the table of the AtanYX function
*
* @param  in    f16ValY - the pInterval1 signal assumed to be equal to sin(theta) where 
*                         theta is the shaft angular position, the signal should be    
*				          scaled to the fractional range [-1, 1)                   
*				          ([0x8000, 0x7FFF) hex).                                  
*				                                                                
*				f16ValX - the pInterval2 signal from position sensor assumed to be     
*					      equal to sin(theta + dtheta) where theta is the shaft    
*					      angular and dtheta the phase shift to the pInterval1 position 
*					      sensor signal. The signal should be scaled to the        
*					      fractional range [-1, 1) ([0x8000, 0x7FFF) hex)          
*
*           f16Ky   - multiplication coefficient of y signal
*			f16Kx   - multiplication coefficient of x signal
*			i16Ny   - scaling coefficient of y signal       
*			i16Nx   - scaling coefficient of x signal       
*			f16ThetaAdj - adjusting angle                   
*           
*           This coefficients initializes the internal   
*		  	variables and should be	initializes before the call to this function.
*
*           
* @return   The function returns an angle of two sine waves shifted in 
*     	    in phase. The output data value is in the range [-1,1)    
*     	    which corresponds to [-pi,pi) range of angle.             
*		
* @remarks 	The AtanYXShifted function computes the angle using     
*		  	two sine waves. The signals should be scaled to be in   
*			the range [-1,1) (fractional range).                    
*			                                                        
*           The function assumes that the provided arguments        
*			correspond to the following signals:                    
*                                                                   
*			- nValY (pInterval1 argument) = sin(theta)                    
*           - nValX (pInterval2 argument) = sin(theta + dtheta)          
*                                                                   
*			where:                                                  
*             theta     the actual angle                            
*			  dtheta    the shift betwen signals, can be set in the 
*			            header file                                 
*			                                                        
*			The returned angle is equal:                            
*			                                                        
*			  thetac = theta + thetaoffset                          
*			                                                        
*			where:                                                  
*			  thetac       computed angle                           
*			  dtheta       as above                                 
*			  thetaoffset  angle offset, can be set in header file  
*			                                                        
*			The signal values (sin(theta) and sin(theta + dtheta))  
*			are provided to the function as argument.                 
*			                                                        
*			The function computes angle with accuracy depending on  
*			signals' phase difference. The function is most         
*			accurate when dtheta does not differ from pi/2 or -pi2. 
*			On the other hand the function error raises to infinity 
*			with dtheta approaching 0 or -pi. The detailed error    
*			analysis can be found in the accompanying documentation 
*
*       Special Issues: The function requires GFLIB_AtanYX function to be called
*		                                                                   		                                                                   
*
*		SATURATION INDEPENDENT!
*
****************************************************************************/
extern asm frac16_t GFLIB_AtanYXShifted_F16_FAsm(frac16_t f16ValY,
												 frac16_t f16ValX,
												 const GFLIB_ATANYXSHIFTED_T_F16 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_ATANYXSHIFTED_F32_ASM_H_ */
