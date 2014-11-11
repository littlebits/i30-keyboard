/***********************************************************************************************************************
 * Copyright 2014 littleBits Electronics
 *
 * This file is part of i30-keyboard.
 *
 * i30-keyboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * i30-keyboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License at <http://www.gnu.org/licenses/> for more details.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 2013/06/07
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_grobal_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static uint16_t g_bef_keys[CHECK_KEYS_BEF];
static char g_key;
static char g_range_mode;
static char g_cur_key;

void u_check_current_key(void);
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
	R_ADC_Start();
	R_TAU0_Channel0_Start();
	R_TAU0_Channel2_Start();

	while (1U)
	{
		if(get_param_change_flag())u_check_current_key();
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	char i;

	EI();
	u_timer_Init();
	u_g_param_Init();

	TDR02 = 0xBB80;		/* 2msec interval clock */
	
	TDR00 = 1019;		// 60で割り切れる数にしておく（例：1020サンプル）
	TDR01 = 509;
	
	ADIF  = 0;
	
	set_kind_param(RANGE);
	
	ADS = _02_AD_INPUT_CHANNEL_2;
	R_ADC_Set_OperationOn();
	
	for(i=0; i < CHECK_KEYS_BEF; i++)g_bef_keys[i] = 0;
	g_key = num_keys;
	
	if(P0.0 == ON && P6.0 == ON && P1.2 == ON){
		g_range_mode = 1;
	}else{
		g_range_mode = 0;
	}
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void u_check_current_key(void)
{
	char	i;
	char		l_bef_key;
	uint16_t	l_all_keys;
	uint16_t	l_current_key;
	uint16_t	l_range;
	uint8_t		l_oct;
	
	char	l_flag;
	char	l_press_status;
	
	
	l_bef_key = g_key;
	
	DI();
	l_flag	= get_param_change_flag();
	reset_param_change_flag(FLAG_KEY | FLAG_RANGE | FLAG_PRESS | FLAG_INPUT);
	EI();
	
	l_press_status	= get_press_status();
	l_all_keys	= 0;
	l_current_key	= 0;
	l_range		= 0;
	l_all_keys	= get_all_keys_status();
	
	if(l_flag & FLAG_KEY){
		l_current_key = (l_all_keys & (~g_bef_keys[0]));
	
		if(l_current_key == MUTE){
			i = 0;
			g_bef_keys[i] &= l_all_keys;
			while(l_current_key == MUTE && i < (CHECK_KEYS_BEF-1)){
				g_bef_keys[i+1] &= l_all_keys;
				
				if(g_bef_keys[i] == 0 ){
					l_current_key = MUTE;
					i++;
					break;
				}
				l_current_key = g_bef_keys[i] & (~g_bef_keys[i+1]);
				i++;
			}
		}else{
			for(i=(CHECK_KEYS_BEF-2); i > 0; i--){
				g_bef_keys[i] = g_bef_keys[i-1] & l_all_keys;
			}
			g_bef_keys[0] = l_all_keys;
		}
	
		g_key=0;
		while(l_current_key != 0x1 && g_key < num_keys){
			g_key++;
			l_current_key = l_current_key >> 1;
		}
		if(l_bef_key == g_key){
		}else{
			if(l_press_status == ALWAYS && g_key == num_keys){
				if(i == 0)u_set_Trig_zero();
			}else{
				u_set_Trig_zero();
			}
		}
	}
	if(l_press_status == ALWAYS && g_key == num_keys){
		l_bef_key = g_cur_key;
	}else{
		l_bef_key = g_key;
		g_cur_key = g_key;
	}
	if(l_bef_key == num_keys){
		u_set_current_energy(0);
		u_set_Trig_zero();
	}else{
		if(g_range_mode == 0){
			l_oct		= get_octave();
			l_current_key	= (uint16_t)(l_bef_key + (l_oct*12) + 4) * ONE_NOTE_SEP_SINGLE;
		}else{
			l_range	= get_range();
			l_range	= l_range * RANGE_ADJ >> 6;
			l_current_key = l_bef_key;
			l_current_key *= ONE_NOTE_SEP_OCT;
			l_current_key = l_current_key >> 6;
			l_current_key *= (l_range - LOWESTFREQVALUE + OCTAVE_LENGTH);
			l_current_key = l_current_key >> 6;
			l_current_key += ((0x3FF - l_range) >> 1);
		}
		u_set_current_energy(l_current_key);
	}
}
/* End user code. Do not edit comment generated here */
