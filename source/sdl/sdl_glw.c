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
/*
** GLW_IMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_EndFrame
** GLimp_Init
** GLimp_Shutdown
**
*/
#include <assert.h>
#include "../ref_gl/r_local.h"
#include "../sdl/sdl_glw.h"
#include "../win32/resource.h"
#ifndef _WIN32
    #include "SDL2/SDL.h"
#else
    #include "SDL.h"
#endif

static int GLimp_InitGL( void );

glwstate_t glw_state;

/*
** VID_CreateWindow
*/
#define WITH_UTF8

#pragma warning( disable : 4055 )

static void VID_SetWindowSize( qboolean fullscreen )
{
	if (fullscreen)
	{
		SDL_SetWindowFullscreen(glw_state.sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else 
	{
		SDL_SetWindowSize(glw_state.sdl_window, glConfig.width, glConfig.height);
	}
	/*
	RECT r;
	int stylebits;
	int exstyle;
	int x = glw_state.win_x, y = glw_state.win_y;
	int width = glConfig.width, height = glConfig.height;
	HWND parentHWND = glw_state.parenthWnd;

	if( fullscreen )
	{
		exstyle = WS_EX_TOPMOST;
		stylebits = ( WS_POPUP|WS_VISIBLE );
		parentHWND = NULL;
	}
	else if( parentHWND )
	{
		exstyle = 0;
		stylebits = WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE;
	}
	else
	{
		exstyle = 0;
		stylebits = WINDOW_STYLE;
	}

	r.left = 0;
	r.top = 0;
	r.right  = width;
	r.bottom = height;

	//SDL_SetWindowPosition(glw_state.sdlWindow, x, y);
	SDL_SetWindowSize(glw_state.sdlWindow, width, height);

	AdjustWindowRect( &r, stylebits, FALSE );

	width = r.right - r.left;
	height = r.bottom - r.top;

	if( fullscreen )
	{
		x = 0;
		y = 0;
	}
	else if( parentHWND )
	{
		RECT parentWindowRect;

		GetWindowRect( parentHWND, &parentWindowRect );

		// share centre with the parent window
		x = (parentWindowRect.right - parentWindowRect.left - width) / 2;
		y = (parentWindowRect.bottom - parentWindowRect.top - height) / 2;
	}

	SetActiveWindow( glw_state.hWnd );

	SetWindowLong( glw_state.hWnd, GWL_EXSTYLE, exstyle );
	SetWindowLong( glw_state.hWnd, GWL_STYLE, stylebits );

	SetWindowPos( glw_state.hWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED );

	ShowWindow( glw_state.hWnd, SW_SHOW );
	UpdateWindow( glw_state.hWnd );

	SetForegroundWindow( glw_state.hWnd );
	SetFocus( glw_state.hWnd );
	*/
}

static qboolean VID_CreateWindow( void )
{
	qboolean fullscreen = glConfig.fullScreen;
	
	glw_state.sdl_window = SDL_CreateWindow(
        glw_state.applicationName,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0,
        0,
        SDL_WINDOW_OPENGL
    );
			
	if( !glw_state.sdl_window )
		Sys_Error( "Couldn't create window: \"%s\"", SDL_GetError() );
		
	VID_SetWindowSize( fullscreen );

	// init all the gl stuff for the window
	if( !GLimp_InitGL() )
	{
		ri.Com_Printf( "VID_CreateWindow() - GLimp_InitGL failed\n" );
		return qfalse;
	}

	return qtrue;
}

/*
** VID_SetFullscreenMode
*/
static qboolean VID_SetFullscreenMode( int displayFrequency, qboolean fullscreen )
{
//	// do a CDS if needed
//	if( fullscreen )
//	{
//		int a;
//		DEVMODE dm;
//		HDC hdc;
//		int bitspixel;
//
//		ri.Com_DPrintf( "...attempting fullscreen\n" );
//
//		memset( &dm, 0, sizeof( dm ) );
//
//		dm.dmSize = sizeof( dm );
//
//		dm.dmPelsWidth  = glConfig.width;
//		dm.dmPelsHeight = glConfig.height;
//		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
//
//		hdc = GetDC( NULL );
//		bitspixel = GetDeviceCaps( hdc, BITSPIXEL );
//
//		ri.Com_DPrintf( "...using desktop display depth of %d\n", bitspixel );
//
//		ReleaseDC( 0, hdc );
//
//		if( displayFrequency > 0 )
//		{
//			dm.dmFields |= DM_DISPLAYFREQUENCY;
//			dm.dmDisplayFrequency = displayFrequency;
//			ri.Com_DPrintf( "...using display frequency %i\n", dm.dmDisplayFrequency );
//		}
//
//		ri.Com_DPrintf( "...calling CDS: " );
//		a = ChangeDisplaySettings( &dm, CDS_FULLSCREEN );
//		if( a == DISP_CHANGE_SUCCESSFUL )
//		{
//			ri.Com_DPrintf( "ok\n" );
//
//			if( glw_state.sdl_window ) {
//				VID_SetWindowSize( qtrue );
//			}
//			return qtrue;
//		}
//		else
//		{
//			ri.Com_DPrintf( "failed: %i\n", a );
//
//			ri.Com_DPrintf( "...calling CDS assuming dual monitors:" );
//
//			dm.dmPelsWidth = glConfig.width * 2;
//			dm.dmPelsHeight = glConfig.height;
//			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
//
//			if( displayFrequency > 0 )
//			{
//				dm.dmFields |= DM_DISPLAYFREQUENCY;
//				dm.dmDisplayFrequency = displayFrequency;
//				ri.Com_DPrintf( "...using display frequency %i\n", dm.dmDisplayFrequency );
//			}
//
//			/*
//			** our first CDS failed, so maybe we're running on some weird dual monitor
//			** system
//			*/
//			if( ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
//			{
//				ri.Com_DPrintf( " failed\n" );
//
//				ri.Com_DPrintf( "...setting windowed mode\n" );
//
//				ChangeDisplaySettings( 0, 0 );
//
//				if( glw_state.sdl_window ) {
//					VID_SetWindowSize( qfalse );
//				}
//				return qfalse;
//			}
//			else
//			{
//				ri.Com_DPrintf( " ok\n" );
//
//				if( glw_state.sdl_window ) {
//					VID_SetWindowSize( qtrue );
//				}
//				return qtrue;
//			}
//		}
//	}
//	else
//	{
//		ri.Com_DPrintf( "...setting windowed mode\n" );
//
//		ChangeDisplaySettings( 0, 0 );
//
//		if( glw_state.sdl_window ) {
//			VID_SetWindowSize( qfalse );
//		}
//	}
//
//	return qfalse;
    return qtrue;
}

/*
** GLimp_SetMode
*/
rserr_t GLimp_SetMode( int x, int y, int width, int height, int displayFrequency, 
	qboolean fullscreen, qboolean wideScreen )
{
	const char *win_fs[] = { "W", "FS" };

	// check whether we can toggle fullscreen without a vid_restart
	if( glw_state.sdl_window ) {
		if( glConfig.width == width && glConfig.height == height && fullscreen != glConfig.fullScreen ) {
			glConfig.fullScreen = VID_SetFullscreenMode( displayFrequency, fullscreen );

			if( glConfig.fullScreen == fullscreen ) {
				VID_SetWindowSize( fullscreen );
				return rserr_ok;
			}

			return rserr_restart_required;
		}
	}

	ri.Com_Printf( "Initializing OpenGL display\n" );

	ri.Com_Printf( "...setting mode:" );

	// disable fullscreen if rendering to a parent window
	/*
	if( glw_state.parenthWnd ) {
		RECT parentWindowRect;

		fullscreen = qfalse;
		wideScreen = qfalse;

		GetWindowRect( glw_state.parenthWnd, &parentWindowRect );
		width = parentWindowRect.right - parentWindowRect.left;
		height = parentWindowRect.bottom - parentWindowRect.top;
	}
	*/

	ri.Com_Printf( " %d %d %s\n", width, height, win_fs[fullscreen] );

	// destroy the existing window
	if( glw_state.sdl_window )
	{
		GLimp_Shutdown();
	}

	glw_state.win_x = x;
	glw_state.win_y = y;

	glConfig.width = width;
	glConfig.height = height;
	glConfig.wideScreen = wideScreen;
	glConfig.fullScreen = VID_SetFullscreenMode( displayFrequency, fullscreen );

	if( !VID_CreateWindow() ) {
		return rserr_invalid_mode;
	}

	return ( fullscreen == glConfig.fullScreen ? rserr_ok : rserr_invalid_fullscreen );
}

/*
** GLimp_Shutdown
**
** This routine does all OS specific shutdown procedures for the OpenGL
** subsystem.  Under OpenGL this means NULLing out the current DC and
** HGLRC, deleting the rendering context, and releasing the DC acquired
** for the window.  The state structure is also nulled out.
**
*/
void GLimp_Shutdown( void )
{
	//if( glw_state.parenthWnd )
	//	PostMessage( glw_state.parenthWnd, UWM_APPACTIVE, WA_INACTIVE, 0 );

	//if( qwglMakeCurrent && !qwglMakeCurrent( NULL, NULL ) )
	//	ri.Com_Printf( "ref_gl::R_Shutdown() - wglMakeCurrent failed\n" );
	SDL_GL_MakeCurrent(NULL, NULL);

	//if( glw_state.hGLRC )
	//{
	//	if( qwglDeleteContext && !qwglDeleteContext( glw_state.hGLRC ) )
	//		ri.Com_Printf( "ref_gl::R_Shutdown() - wglDeleteContext failed\n" );
	//	glw_state.hGLRC = NULL;
	//}
	if( glw_state.sdl_glcontext ) 
	{
		SDL_GL_DeleteContext(glw_state.sdl_glcontext);

		glw_state.sdl_glcontext = NULL;
	}

	//if( glw_state.hDC )
	//{
	//	if( !ReleaseDC( glw_state.hWnd, glw_state.hDC ) )
	//		ri.Com_Printf( "ref_gl::R_Shutdown() - ReleaseDC failed\n" );
	//	glw_state.hDC   = NULL;
	//}
	//if( glw_state.hWnd )
	//{
	//	ShowWindow( glw_state.hWnd, SW_HIDE );
	//	DestroyWindow( glw_state.hWnd );
	//	glw_state.hWnd = NULL;
	//}

	if( glw_state.sdl_window )
	{
		SDL_HideWindow( glw_state.sdl_window );
		SDL_DestroyWindow( glw_state.sdl_window );
		glw_state.sdl_window = NULL;
	}

//#ifdef WITH_UTF8
//	UnregisterClassW( glw_state.windowClassNameW, glw_state.hInstance );
//#else
//	UnregisterClass( glw_state.windowClassName, glw_state.hInstance );
//#endif

	if( glConfig.fullScreen )
	{
#ifdef _WIN32
		ChangeDisplaySettings( 0, 0 );
#endif
		glConfig.fullScreen = qfalse;
	}

	if( glw_state.applicationName )
	{
		free( glw_state.applicationName );
		glw_state.applicationName = NULL;
	}

	glw_state.win_x = 0;
	glw_state.win_y = 0;

	glConfig.width = 0;
	glConfig.height = 0;
}


/*
** GLimp_Init
**
** This routine is responsible for initializing the OS specific portions
** of OpenGL.  Under Win32 this means dealing with the pixelformats and
** doing the wgl interface stuff.
*/
int GLimp_Init( const char *applicationName, void *hinstance, void *wndproc, void *parenthWnd )
{
	// save off hInstance and wndproc
	glw_state.applicationName = (char*)malloc( strlen( applicationName ) + 1 );
	memcpy( glw_state.applicationName, applicationName, strlen( applicationName ) + 1 );
	//glw_state.hInstance = ( HINSTANCE ) hinstance;
	//glw_state.wndproc = wndproc;
	//glw_state.parenthWnd = ( HWND )parenthWnd;

	return qtrue;
}

static int GLimp_InitGL( void )
{
	int colorBits, depthBits, stencilBits;
	
	cvar_t *stereo;
	stereo = ri.Cvar_Get( "cl_stereo", "0", 0 );

//	pfd.cStencilBits = max( 0, r_stencilbits->integer );
//
//	if( pfd.cStencilBits != 0 )
//		glConfig.stencilEnabled = qtrue;
//	else
//		glConfig.stencilEnabled = qfalse;

	if( max( 0, r_stencilbits->integer ) != 0 )
		glConfig.stencilEnabled = qtrue;
	else
		glConfig.stencilEnabled = qfalse;

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, max( 0, r_stencilbits->integer ));

/*
//	** set PFD_STEREO if necessary
//	*/
//	if( stereo->integer != 0 )
//	{
//		ri.Com_DPrintf( "...attempting to use stereo\n" );
//		pfd.dwFlags |= PFD_STEREO;
//		glConfig.stereoEnabled = qtrue;
//	}
//	else
//	{
//		glConfig.stereoEnabled = qfalse;
//	}

	if( stereo->integer != 0)
	{
		ri.Com_DPrintf( "...attempting to use stereo\n" );
		SDL_GL_SetAttribute(SDL_GL_STEREO, 1);
		glConfig.stereoEnabled = qtrue;
	}
	else
	{
		glConfig.stereoEnabled = qfalse;
	}

//	/*
//	** startup the OpenGL subsystem by creating a context and making
//	** it current
//	*/
//	if( ( glw_state.hGLRC = qwglCreateContext( glw_state.hDC ) ) == 0 )
//	{
//		ri.Com_Printf( "GLimp_Init() - qwglCreateContext failed\n" );
//		goto fail;
//	}
//
//	if( !qwglMakeCurrent( glw_state.hDC, glw_state.hGLRC ) )
//	{
//		ri.Com_Printf( "GLimp_Init() - qwglMakeCurrent failed\n" );
//		goto fail;
//	}

	glw_state.sdl_glcontext = SDL_GL_CreateContext(glw_state.sdl_window);
	if (glw_state.sdl_glcontext == 0) {
		ri.Com_Printf( "GLimp_Init() - SDL_GL_CreateContext failed: \"%s\"\n", SDL_GetError() );
		goto fail;
	}

	if (SDL_GL_MakeCurrent(glw_state.sdl_window, glw_state.sdl_glcontext)) {
		ri.Com_Printf( "GLimp_Init() - SDL_GL_MakeCurrent failed: \"%s\"\n", SDL_GetError() );
		goto fail;
	}

	/*
	** print out PFD specifics
	*/
	SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, &colorBits);
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthBits);
	SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencilBits);
	
	ri.Com_Printf( "GL PFD: color(%d-bits) Z(%d-bit) stencil(%d-bits)\n", colorBits, depthBits, stencilBits);

	return qtrue;

fail:
	return qfalse;

//	PIXELFORMATDESCRIPTOR pfd =
//	{
//		sizeof( PIXELFORMATDESCRIPTOR ), // size of this pfd
//		1,                      // version number
//		PFD_DRAW_TO_WINDOW |    // support window
//		PFD_SUPPORT_OPENGL |    // support OpenGL
//		PFD_DOUBLEBUFFER,       // double buffered
//		PFD_TYPE_RGBA,          // RGBA type
//		32,                     // 32-bit color depth
//		0, 0, 0, 0, 0, 0,       // color bits ignored
//		0,                      // no alpha buffer
//		0,                      // shift bit ignored
//		0,                      // no accumulation buffer
//		0, 0, 0, 0,             // accum bits ignored
//		24,                     // 32-bit z-buffer
//		0,                      // no stencil buffer
//		0,                      // no auxiliary buffer
//		PFD_MAIN_PLANE,         // main layer
//		0,                      // reserved
//		0, 0, 0                 // layer masks ignored
//	};
//	int pixelformat;
//	cvar_t *stereo;
//
//	stereo = ri.Cvar_Get( "cl_stereo", "0", 0 );
//
//	pfd.cStencilBits = max( 0, r_stencilbits->integer );
//
//	if( pfd.cStencilBits != 0 )
//		glConfig.stencilEnabled = qtrue;
//	else
//		glConfig.stencilEnabled = qfalse;
//
//	/*
//	** set PFD_STEREO if necessary
//	*/
//	if( stereo->integer != 0 )
//	{
//		ri.Com_DPrintf( "...attempting to use stereo\n" );
//		pfd.dwFlags |= PFD_STEREO;
//		glConfig.stereoEnabled = qtrue;
//	}
//	else
//	{
//		glConfig.stereoEnabled = qfalse;
//	}
//
//	/*
//	** Get a DC for the specified window
//	*/
//	if( glw_state.hDC != NULL )
//		ri.Com_Printf( "GLimp_Init() - non-NULL DC exists\n" );
//
//	if( ( glw_state.hDC = GetDC( glw_state.hWnd ) ) == NULL )
//	{
//		ri.Com_Printf( "GLimp_Init() - GetDC failed\n" );
//		return qfalse;
//	}
//
//	if( ( pixelformat = ChoosePixelFormat( glw_state.hDC, &pfd ) ) == 0 )
//	{
//		ri.Com_Printf( "GLimp_Init() - ChoosePixelFormat failed\n" );
//		return qfalse;
//	}
//	if( SetPixelFormat( glw_state.hDC, pixelformat, &pfd ) == FALSE )
//	{
//		ri.Com_Printf( "GLimp_Init() - SetPixelFormat failed\n" );
//		return qfalse;
//	}
//	DescribePixelFormat( glw_state.hDC, pixelformat, sizeof( pfd ), &pfd );
//
//	/*
//	** report if stereo is desired but unavailable
//	*/
//	if( !( pfd.dwFlags & PFD_STEREO ) && ( stereo->integer != 0 ) )
//	{
//		ri.Com_Printf( "...failed to select stereo pixel format\n" );
//		ri.Cvar_SetValue( "cl_stereo", 0 );
//		glConfig.stereoEnabled = qfalse;
//	}
//
//	/*
//	** startup the OpenGL subsystem by creating a context and making
//	** it current
//	*/
//	if( ( glw_state.hGLRC = qwglCreateContext( glw_state.hDC ) ) == 0 )
//	{
//		ri.Com_Printf( "GLimp_Init() - qwglCreateContext failed\n" );
//		goto fail;
//	}
//
//	if( !qwglMakeCurrent( glw_state.hDC, glw_state.hGLRC ) )
//	{
//		ri.Com_Printf( "GLimp_Init() - qwglMakeCurrent failed\n" );
//		goto fail;
//	}
//
//	/*
//	** print out PFD specifics
//	*/
//	ri.Com_Printf( "GL PFD: color(%d-bits) Z(%d-bit) stencil(%d-bits)\n", ( int ) pfd.cColorBits, ( int ) pfd.cDepthBits, ( int )pfd.cStencilBits );
//
//	return qtrue;
//
//fail:
//	if( glw_state.hGLRC )
//	{
//		qwglDeleteContext( glw_state.hGLRC );
//		glw_state.hGLRC = NULL;
//	}
//
//	if( glw_state.hDC )
//	{
//		ReleaseDC( glw_state.hWnd, glw_state.hDC );
//		glw_state.hDC = NULL;
//	}
//	return qfalse;
}

/*
** GLimp_UpdateGammaRamp
*/
qboolean GLimp_GetGammaRamp( size_t stride, unsigned short *ramp )
{
	if( stride != 256 )
	{
		// SDL only supports gamma ramps with 256 mappings per channel
		return qfalse;
	}
	return SDL_GetWindowGammaRamp(glw_state.sdl_window, ramp, ramp+stride, ramp+( stride<<1 ) ) == -1 ? qfalse : qtrue;
}

/*
** GLimp_SetGammaRamp
*/
void GLimp_SetGammaRamp( size_t stride, unsigned short *ramp )
{
	if( stride != 256 )
	{
		// SDL only supports gamma ramps with 256 mappings per channel
		return;
	}
	if( SDL_SetWindowGammaRamp(glw_state.sdl_window, ramp, ramp+stride, ramp+( stride<<1 ) ) == -1 )
	{
		Com_Printf( "SDL_SetGammaRamp(...) failed: ", SDL_GetError() );
	}
}

/*
** GLimp_BeginFrame
*/
void GLimp_BeginFrame( void )
{

}

/*
** GLimp_EndFrame
**
** Responsible for doing a swapbuffers and possibly for other stuff
** as yet to be determined.  Probably better not to make this a GLimp
** function and instead do a call to GLimp_SwapBuffers.
*/
void GLimp_EndFrame( void )
{
	int err;

	err = qglGetError();
	assert( err == GL_NO_ERROR );

	if( Q_stricmp( gl_drawbuffer->string, "GL_BACK" ) == 0 )
	{
		//if( !qwglSwapBuffers( glw_state.hDC ) )
		//	Sys_Error( "GLimp_EndFrame() - SwapBuffers() failed!" );
		SDL_GL_SwapWindow(glw_state.sdl_window);
	}
}

/*
** GLimp_AppActivate
*/
void GLimp_AppActivate( qboolean active, qboolean destroy )
{
	if( active )
	{
		ri.Cvar_Set( "gl_drawbuffer", "GL_BACK" );
		SDL_MaximizeWindow( glw_state.sdl_window );
		SDL_ShowWindow( glw_state.sdl_window );
		//SetForegroundWindow( glw_state.hWnd );
		//ShowWindow( glw_state.hWnd, SW_RESTORE );
	}
	else
	{
		if( glConfig.fullScreen )
		{
			ri.Cvar_Set( "gl_drawbuffer", "GL_NONE" );
			SDL_HideWindow( glw_state.sdl_window );
			//ShowWindow( glw_state.hWnd, SW_MINIMIZE );
		}
		else
		{
			if( destroy )
				ri.Cvar_Set( "gl_drawbuffer", "GL_NONE" );
		}
	}
}
