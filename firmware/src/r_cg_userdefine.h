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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 2013/06/07
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#define NOTE_RANGE	60
#define RANGE_MOVE	48
#define OCTAVE		12

#define ONE_NOTE_SEP	0x1111	// this value is shifted left 8
#define RANGE_ADJ	0x32	// this value is shifted left 6

#define CHECK_KEYS_BEF	15	// can sound before 

#define	ONE_NOTE_SEP_SINGLE	17
#define	OCT_SEP_SINGLE		204

#define ONE_NOTE_SEP_OCT	0x13B	// this value is shifted left 12
#define OCTAVE_LENGTH		0xCD

#define LOWESTFREQVALUE		20

#define MUTE	0x0

enum NOB_SELECT{
	INPUT = 0,
	RANGE,
	SW13_KEY12,
	ELSE
};

enum SW_STATUS{
	PRESS = 0,
	ALWAYS
};
	
enum KEY_NUMBER
{
	KEY1 = 0,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY10,
	KEY11,
	KEY12,
	KEY13,
	
	num_keys,
};

enum KEY_STATUS{
	ON = 0,
	OFF
};
/* End user code. Do not edit comment generated here */
#endif
