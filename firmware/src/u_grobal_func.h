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

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"

#include "r_cg_userdefine.h"

/* common functions declare */
#define FLAG_KEY	1
#define FLAG_PRESS	2
#define FLAG_INPUT	4
#define FLAG_RANGE	8

void		u_g_param_Init(void);

char		get_param_change_flag(void);
void		reset_param_change_flag(char c);

char		get_kind_param(void);
void		set_kind_param(char c);

uint16_t	get_all_keys_status(void);
char 		get_key_status(char key);
void 		set_key_status(uint16_t c, char key);

char		get_press_status(void);
void		set_press_status(char c);

uint16_t	get_input(void);
void		set_input(uint16_t u);

uint16_t	get_range(void);
void		set_range(uint16_t u);

uint8_t		get_octave(void);
void		set_octave(void);


