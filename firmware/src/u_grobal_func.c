 /*
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
 */
 
#include "u_grobal_func.h"
#include <math.h>

// common parameters
static char	g_param_change_flag;

static char	g_kind_param;

static uint16_t g_key_status;
static char	g_press_status;

static uint16_t g_input_signal;
static uint16_t g_range;
static uint16_t g_range_oct;

static uint8_t	g_oct;

// common functions  
void u_g_param_Init(void)
{
	char i;
	
	g_param_change_flag = 0;
	
	g_kind_param	= 0;

	g_key_status	= 0;
	g_press_status	= 0;
	
	g_input_signal	= 0;
	g_range		= 0;
	g_range_oct	= 0;
	g_oct		= 0;
}

// param changed /////
char get_param_change_flag(void)
{
	return g_param_change_flag;
}
void reset_param_change_flag(char c)
{
	g_param_change_flag &= ~c;
}
// kind param (Which step value knob is looked now) //////////
char get_kind_param(void)
{
	return g_kind_param;
}
void set_kind_param(char c)
{
	g_kind_param = c;
}
// key status /////
uint16_t get_all_keys_status(void)
{
	return g_key_status;
}

char get_key_status(char key)
{
	return (((g_key_status >> key) & 0x1) ^ 0x1);
}
void set_key_status(uint16_t c, char key)
{
	if(((g_key_status >> key) & 1) != (~c & 1)){
		if((~c & 1) == 0x0){
			g_key_status &= (0x1FFF - ((c & 1) << key));
		}else{
			g_key_status |= (~c & 1) << key;
		}
		g_param_change_flag |= FLAG_KEY;
	}
}
// press status /////
char get_press_status(void)
{
	return g_press_status;
}
void set_press_status(char c)
{
	if(g_press_status != c){
		g_press_status = c;
		g_param_change_flag |= FLAG_PRESS;
	}
}
// input /////
uint16_t get_input(void)
{
	return g_input_signal;
}
void set_input(uint16_t u)
{
	if(g_input_signal != u){
		g_input_signal = (u + g_input_signal) >> 1;
		g_param_change_flag |= FLAG_INPUT;
	}
}
// range /////
uint16_t get_range(void)
{
	return g_range;
}
void set_range(uint16_t u)
{
	if(g_range != u){
		g_range = (u + g_range) >> 1;
		g_param_change_flag |= FLAG_RANGE;
		set_octave();
	}
}

// octave /////
#define OCT_GAP	32

void set_octave(void)
{
	uint16_t si, sj;
	uint8_t oct;
	
	si = (g_range ^ 0x3ff);
	sj = ((uint16_t)g_oct << 8) + 128;	// Œ»ÝOCT‚ÌCenter‚Æ”äŠr‚µ‚Ä•ûŒü”»’è
	
	if (si > sj) {
		//----- ãŒü‚«
		if (si > OCT_GAP) {
			si -= OCT_GAP;
		} else {
			si = 0;
		}
	} else if (si < sj) {
		si += OCT_GAP;
		if (si > 1023) {
			si = 1023;
		}
	}
	oct	= (uint8_t)(si >> 8);	// 0-1023 -> 0-3
	g_range_oct = (g_range ^ 0x3ff);
	g_oct	= oct;
}

uint8_t get_octave(void)
{
	return g_oct;
}

