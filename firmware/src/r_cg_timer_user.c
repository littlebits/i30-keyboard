/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2013/06/07
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_grobal_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static char	g_count_sw_off[num_keys];
static char	g_count_sw_on[num_keys];
static char	g_count_press_off;
static char	g_count_press_on;

static char	g_count_Trig_zero;

static uint16_t g_current_energy;
static uint16_t g_pre_energy;

static uint16_t g_input_signal_t;
static uint16_t g_energy_mask;

static char	g_ad_sw;
static char	g_port_status_KEY12;

void u_check_key_status(char port_status, char key);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	uint32_t	l_temp32;
   	uint16_t	temp_result;
	
	TDR01 = g_current_energy;
	
	l_temp32	 = (uint32_t)g_pre_energy * (uint32_t)g_input_signal_t;
	l_temp32	 = l_temp32 >> 10;
	g_current_energy = ((uint16_t)(l_temp32) & g_energy_mask);
	
	//
	// Triggerêßå‰ÇÕTimer2Ç÷à⁄ìÆ
	//
	
	if(ADIF == 1){
		ADIF = 0;
		R_ADC_Get_Result(&temp_result);
		if(g_ad_sw == INPUT){
			if(temp_result < 15){
				g_input_signal_t = 0;
			}else{
				g_input_signal_t = ((g_input_signal_t + temp_result) >> 1);
			}
			g_ad_sw = ELSE; // set read ad not input.
			
			switch(get_kind_param()){
			case RANGE:	set_kind_param(SW13_KEY12);
					ADS = _00_AD_INPUT_CHANNEL_0;
					break;
			case SW13_KEY12:set_kind_param(RANGE);
					ADS = _02_AD_INPUT_CHANNEL_2;
					break;
			default:	break;
			}
		}else{
			switch(get_kind_param()){
			case RANGE:	set_range(temp_result);
					break;
			case SW13_KEY12:if(temp_result > 0x200){
						g_port_status_KEY12 = OFF;
					}else{
						g_port_status_KEY12 = ON;
					}
					break;
			default:	break;
			}
			g_ad_sw = INPUT; // set read ad input.
			ADS = _01_AD_INPUT_CHANNEL_1;
		}
		R_ADC_Start();
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
 	char l_port_status;
	
	if (g_pre_energy) {
		if (g_count_Trig_zero < 25) {
			if ((++g_count_Trig_zero) == 25) {
				P4.2 = 1;
				g_energy_mask = 0xffff;
			}
		}
	}
	
	if( P13.7 == ALWAYS && get_press_status() == PRESS ){
		g_count_press_on = 0;
		g_count_press_off++;
		if( g_count_press_off == 3 ){
			set_press_status(ALWAYS);
			g_count_press_off = 0;
		}
	}else if( P13.7 != ALWAYS && get_press_status() == ALWAYS){
		g_count_press_off = 0;
		g_count_press_on++;
		if( g_count_press_on == 3 ){
			set_press_status(PRESS);
			g_count_press_on = 0;
		}
	}
	
	l_port_status = P0.0;	u_check_key_status( l_port_status, KEY1);
	l_port_status = P0.1;	u_check_key_status( l_port_status, KEY2);
	l_port_status = P0.2;	u_check_key_status( l_port_status, KEY3);
	l_port_status = P0.3;	u_check_key_status( l_port_status, KEY4);
	l_port_status = P6.0;	u_check_key_status( l_port_status, KEY5);
	l_port_status = P6.1;	u_check_key_status( l_port_status, KEY6);
	l_port_status = P1.3;	u_check_key_status( l_port_status, KEY7);
	l_port_status = P1.2;	u_check_key_status( l_port_status, KEY8);
	l_port_status = P1.1;	u_check_key_status( l_port_status, KEY9);
	l_port_status = P1.0;	u_check_key_status( l_port_status, KEY10);
	l_port_status = P2.3;	u_check_key_status( l_port_status, KEY11);
	u_check_key_status( g_port_status_KEY12, KEY12);
	l_port_status = P4.1;	u_check_key_status( l_port_status, KEY13);
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void u_check_key_status(char port_status, char key)
{
	if( port_status == ON && get_key_status(key) == OFF ){
		g_count_sw_off[key] = 0;
		g_count_sw_on[key]++;
		if( g_count_sw_on[key] == 3 ){
			set_key_status(ON, key);
			g_count_sw_on[key] = 0;
		}
	}else if( port_status > ON && get_key_status(key) == ON ){
		g_count_sw_on[key] = 0;
		g_count_sw_off[key]++;
		if( g_count_sw_off[key] == 3 ){
			set_key_status(OFF, key);
			g_count_sw_off[key] = 0;
		}
	}
}

void u_timer_Init(void)
{
	char i;
	for(i=0; i < num_keys; i++){
		g_count_sw_off[i] = 0;
		g_count_sw_on[i]  = 0;
	}
	g_current_energy	= 0;
	g_count_Trig_zero	= 0;
	g_input_signal_t	= 0;
	g_energy_mask		= 0;
	
	g_ad_sw = ELSE;
}

void u_set_current_energy(uint16_t energy)
{
	DI();
	g_pre_energy = energy;
	EI();
}

void u_set_Trig_zero(void)
{
	char sts;
	
	sts = get_press_status();

	DI();
	g_count_Trig_zero = 0;
	P4.2 = 0;
	g_energy_mask	  = 0;
	EI();
}
/* End user code. Do not edit comment generated here */
