/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#ifndef __GLW_SDL_H__
#define __GLW_SDL_H__

#include "SDL2/SDL.h"

typedef struct
{
	char *applicationName;

	SDL_Window* sdl_window;
	SDL_GLContext sdl_glcontext;

	int hinstOpenGL;      // HINSTANCE for the OpenGL library

	int win_x, win_y;
	qboolean allowdisplaydepthchange;
} glwstate_t;

extern glwstate_t glw_state;

extern cvar_t *vid_fullscreen;
extern cvar_t *vid_displayfrequency;

#endif
