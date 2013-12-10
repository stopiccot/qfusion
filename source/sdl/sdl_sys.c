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
// sys_win.h

#include "../qcommon/qcommon.h"
//#include "../win32/winquake.h"
//#include "../win32/resource.h"
#include <errno.h>
#include <float.h>
#include <fcntl.h>
#include <stdio.h>
//#include <io.h>
//#include <conio.h>
#include <limits.h>

#ifndef _WIN32
    #include "SDL2/SDL.h"
#else
    #include "SDL.h"
#endif

//#include "../win32/conproc.h"

#include "../client/client.h"

#define MINIMUM_WIN_MEMORY  0x0a00000
#define MAXIMUM_WIN_MEMORY  0x1000000

int starttime;
int ActiveApp;
int Minimized;
int AppFocused;

//static HANDLE hinput, houtput;

unsigned sys_msg_time;
unsigned sys_frame_time;


//static HANDLE qwclsemaphore;

#define	MAX_NUM_ARGVS	128
int argc;
char *argv[MAX_NUM_ARGVS];

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_Error( const char *format, ... )
{
	va_list	argptr;
	char msg[1024];

	CL_Shutdown();

	va_start( argptr, format );
	Q_vsnprintfz( msg, sizeof( msg ), format, argptr );
	va_end( argptr );

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "qfusion", msg, NULL);

//	if( qwclsemaphore )
//		CloseHandle( qwclsemaphore );

	// shut down QHOST hooks if necessary
	//DeinitConProc();

	Qcommon_Shutdown();

	exit( 1 );
}

void Sys_Quit( void )
{
//	timeEndPeriod( 1 );

	CL_Shutdown();

//	CloseHandle( qwclsemaphore );
//	if( dedicated && dedicated->integer )
//		FreeConsole();

	// shut down QHOST hooks if necessary
	//DeinitConProc();

	Qcommon_Shutdown();

	exit( 0 );
}

//================================================================

/*
* Sys_Milliseconds
*/
static qint64 freq;

static void Sys_InitTime( void )
{
	freq = SDL_GetPerformanceFrequency();
}

unsigned int Sys_Milliseconds( void )
{
	return Sys_Microseconds() / 1000;
}

quint64 Sys_Microseconds( void )
{
	return 1000000 * SDL_GetPerformanceCounter() / freq;
}

void Sys_Sleep( unsigned int millis )
{
	SDL_Delay( millis );
}

/*
* Sys_GetSymbol
*/
#ifdef SYS_SYMBOL
void *Sys_GetSymbol( const char *moduleName, const char *symbolName )
{
	HMODULE module = GetModuleHandle( moduleName );
	return module
		? (void *) GetProcAddress( module, symbolName )
		: NULL;
}
#endif // SYS_SYMBOL

//===============================================================================

/*
* Sys_Init
*/
void Sys_Init( void )
{
//	OSVERSIONINFO vinfo;

//	timeBeginPeriod( 1 );
	Sys_InitTime();

//	vinfo.dwOSVersionInfoSize = sizeof( vinfo );
//
//	if( !GetVersionEx( &vinfo ) )
//		Sys_Error( "Couldn't get OS info" );
//
//	if( vinfo.dwMajorVersion < 4 )
//		Sys_Error( "%s requires windows version 4 or greater", APPLICATION );
//	if( vinfo.dwPlatformId == VER_PLATFORM_WIN32s )
//		Sys_Error( "%s doesn't run on Win32s", APPLICATION );
//	else if( vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
//		s_win95 = qtrue;

//	if( dedicated->integer )
//	{
//		SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
//
//		if( !AllocConsole() )
//			Sys_Error( "Couldn't create dedicated server console" );
//		hinput = GetStdHandle( STD_INPUT_HANDLE );
//		houtput = GetStdHandle( STD_OUTPUT_HANDLE );
//
//		// let QHOST hook in
//		//InitConProc( argc, argv );
//	}
}

#define MAX_CONSOLETEXT 256
static char console_text[MAX_CONSOLETEXT];
static int console_textlen;

//static char *OEM_to_utf8( const char *str )
//{
//	WCHAR wstr[MAX_CONSOLETEXT];
//	static char utf8str[MAX_CONSOLETEXT*4]; /* longest valid utf8 sequence is 4 bytes */
//
//	MultiByteToWideChar( CP_OEMCP, 0, str, -1, wstr, sizeof( wstr )/sizeof( WCHAR ) );
//	wstr[sizeof( wstr )/sizeof( wstr[0] ) - 1] = 0;
//	WideCharToMultiByte( CP_UTF8, 0, wstr, -1, utf8str, sizeof( utf8str ), NULL, NULL );
//	utf8str[sizeof( utf8str ) - 1] = 0;
//
//	return utf8str;
//}
//
//static char *utf8_to_OEM( const char *utf8str )
//{
//	WCHAR wstr[MAX_PRINTMSG];
//	static char oemstr[MAX_PRINTMSG];
//
//	MultiByteToWideChar( CP_UTF8, 0, utf8str, -1, wstr, sizeof( wstr )/sizeof( WCHAR ) );
//	wstr[sizeof( wstr )/sizeof( wstr[0] ) - 1] = 0;
//	WideCharToMultiByte( CP_OEMCP, 0, wstr, -1, oemstr, sizeof( oemstr ), "?", NULL );
//	oemstr[sizeof( oemstr ) - 1] = 0;
//
//	return oemstr;
//}
//
///*
//* Sys_ConsoleInput
//*/
char *Sys_ConsoleInput( void )
{
//	INPUT_RECORD rec;
//	int ch;
//	DWORD dummy;
//	DWORD numread, numevents;
//
//	if( !dedicated || !dedicated->integer )
//		return NULL;
//
//	for(;; )
//	{
//		if( !GetNumberOfConsoleInputEvents( hinput, &numevents ) )
//			Sys_Error( "Error getting # of console events" );
//
//		if( numevents <= 0 )
//			break;
//
//		if( !ReadConsoleInput( hinput, &rec, 1, &numread ) )
//			Sys_Error( "Error reading console input" );
//
//		if( numread != 1 )
//			Sys_Error( "Couldn't read console input" );
//
//		if( rec.EventType == KEY_EVENT )
//		{
//			if( !rec.Event.KeyEvent.bKeyDown )
//			{
//				ch = rec.Event.KeyEvent.uChar.AsciiChar;
//
//				switch( ch )
//				{
//				case '\r':
//					WriteFile( houtput, "\r\n", 2, &dummy, NULL );
//
//					if( console_textlen )
//					{
//						console_text[console_textlen] = 0;
//						console_textlen = 0;
//						return OEM_to_utf8( console_text );
//					}
//					break;
//
//				case '\b':
//					if( console_textlen )
//					{
//						console_textlen--;
//						WriteFile( houtput, "\b \b", 3, &dummy, NULL );
//					}
//					break;
//
//				default:
//					if( ( unsigned char )ch >= ' ' )
//					{
//						if( console_textlen < sizeof( console_text )-2 )
//						{
//							WriteFile( houtput, &ch, 1, &dummy, NULL );
//							console_text[console_textlen] = ch;
//							console_textlen++;
//						}
//					}
//					break;
//				}
//			}
//		}
//	}
//
	return NULL;
}
//
//static void PrintColoredText( const char *s )
//{
//	char c;
//	int colorindex;
//	DWORD dummy;
//
//	while( *s )
//	{
//		int gc = Q_GrabCharFromColorString( &s, &c, &colorindex );
//		if( gc == GRABCHAR_CHAR )
//		{
//			if( c == '\n' )
//				SetConsoleTextAttribute( houtput, 7 );
//			// I hope it's not too slow to output char by char
//			WriteFile( houtput, &c, 1, &dummy, NULL );
//		}
//		else if( gc == GRABCHAR_COLOR )
//		{
//			switch( colorindex )
//			{
//			case 0: colorindex = 3; break;	// dark cyan instead of black to keep it visible
//			case 1: colorindex = 12; break;
//			case 2: colorindex = 10; break;
//			case 3: colorindex = 14; break;
//			case 4: colorindex = 9; break;
//			case 5: colorindex = 11; break;	// note that cyan and magenta are 
//			case 6: colorindex = 13; break;	// not where one might expect
//			case 8: colorindex = 6; break;
//			case 9: colorindex = 8; break;
//			default:
//			case 7: colorindex = 7; break;	// 15 would be bright white
//			};
//			SetConsoleTextAttribute( houtput, colorindex );
//		}
//		else if( gc == GRABCHAR_END )
//			break;
//		else
//			assert( 0 );
//	}
//}
//
///*
//* Sys_ConsoleOutput
//* 
//* Print text to the dedicated console
//*/
void Sys_ConsoleOutput( char *string )
{
//	DWORD dummy;
//	char text[MAX_CONSOLETEXT+2];	/* need 2 chars for the \r's */
//
//	if( !dedicated || !dedicated->integer )
//		return;
//
//	if( console_textlen )
//	{
//		text[0] = '\r';
//		memset( &text[1], ' ', console_textlen );
//		text[console_textlen+1] = '\r';
//		text[console_textlen+2] = 0;
//		WriteFile( houtput, text, console_textlen+2, &dummy, NULL );
//	}
//
//	string = utf8_to_OEM( string );
//
//#if 0
//	WriteFile( houtput, string, (unsigned)strlen( string ), &dummy, NULL );
//#else
//	PrintColoredText( string );
//#endif
//
//	if( console_textlen )
//		WriteFile( houtput, console_text, console_textlen, &dummy, NULL );
}


/*
* myTranslateMessage
* A wrapper around TranslateMessage to avoid garbage if the toggleconsole
* key happens to be a dead key (like in the German layout)
*/
#ifdef DEDICATED_ONLY
#define myTranslateMessage(msg) TranslateMessage(msg)
#else
int IN_MapKey( int key );
qboolean Key_IsNonPrintable( int key );
//static BOOL myTranslateMessage (MSG *msg)
//{
//	if (msg->message == WM_KEYDOWN) {
//		if (Key_IsNonPrintable(IN_MapKey(msg->lParam)))
//			return TRUE;
//		else
//			return TranslateMessage(msg);
//	}
//	return TranslateMessage(msg);
//}
#endif

/*
* Sys_SendKeyEvents
* 
* Send Key_Event calls
*/
void Sys_SendKeyEvents( void )
{
//	MSG msg;
//
//	while( PeekMessageW( &msg, NULL, 0, 0, PM_NOREMOVE ) )
//	{
//		if( !GetMessageW( &msg, NULL, 0, 0 ) )
//			Sys_Quit();
//		sys_msg_time = msg.time;
//		myTranslateMessage( &msg );
//		DispatchMessageW( &msg );
//	}
//
//	// grab frame time
//	sys_frame_time = timeGetTime(); // FIXME: should this be at start?
}

/*
* Sys_GetClipboardData
*/
char *Sys_GetClipboardData( qboolean primary )
{
	return SDL_GetClipboardText();
}

/*
* Sys_SetClipboardData
*/
qboolean Sys_SetClipboardData( char *data )
{
	return (SDL_SetClipboardText(data) == 0) ? qtrue : qfalse;
}

/*
* Sys_FreeClipboardData
*/
void Sys_FreeClipboardData( char *data )
{
	SDL_free( data );
}

/*
==============================================================================

WINDOWS CRAP

==============================================================================
*/

/*
* Sys_AppActivate
*/
void Sys_AppActivate( void )
{
#ifndef DEDICATED_ONLY
//	ShowWindow( cl_hwnd, SW_RESTORE );
//	SetForegroundWindow( cl_hwnd );
#endif
}

//========================================================================

/*
* WinMain
*/
//HINSTANCE global_hInstance;

static int TranslateSDLKey(int charkey)
{
	switch(charkey)
	{
		case SDL_SCANCODE_TAB : charkey = K_TAB; break;
		//case SDLK_TAB:			charkey = K_TAB;		break;
		case SDL_SCANCODE_RETURN : charkey = K_ENTER; break;
		//case SDLK_RETURN:		charkey = K_ENTER;		break;
		case SDL_SCANCODE_ESCAPE: charkey = K_ESCAPE; break;
		//case SDLK_ESCAPE:		charkey = K_ESCAPE;		break;
		//case SDLK_SPACE:		charkey = K_SPACE;		break;
		//case SDLK_CAPSLOCK:		charkey = K_CAPSLOCK;	break;
		//case SDLK_SCROLLLOCK:	charkey = K_SCROLLLOCK;	break;
		//case SDLK_NUMLOCKCLEAR:	charkey = K_NUMLOCK;	break;
		case SDL_SCANCODE_BACKSPACE : charkey = K_BACKSPACE; break;
		//case SDLK_BACKSPACE:	charkey = K_BACKSPACE;	break;
		//case SDLK_UP:			charkey = K_UPARROW;	break;
		//case SDLK_DOWN:			charkey = K_DOWNARROW;	break;
		//case SDLK_LEFT:			charkey = K_FTARROW;	break;
		//case SDLK_RIGHT:		charkey = K_RIGHTARROW;	break;
		//case SDLK_LALT:
		//case SDLK_RALT:			charkey = K_OPTION;		break;
		//case SDLK_LCTRL:
		//case SDLK_RCTRL:		charkey = K_CTRL;		break;
		//case SDLK_LSHIFT:
		//case SDLK_RSHIFT:		charkey = K_SHIFT;		break;			
		//case SDLK_F1:			charkey = K_F1;			break;
		//case SDLK_F2:			charkey = K_F2;			break;
		//case SDLK_F3:			charkey = K_F3;			break;
		//case SDLK_F4:			charkey = K_F4;			break;
		//case SDLK_F5:			charkey = K_F5;			break;
		//case SDLK_F6:			charkey = K_F6;			break;
		//case SDLK_F7:			charkey = K_F7;			break;
		//case SDLK_F8:			charkey = K_F8;			break;
		//case SDLK_F9:			charkey = K_F9;			break;
		//case SDLK_F10:			charkey = K_F10;		break;
		//case SDLK_F11:			charkey = K_F11;		break;
		//case SDLK_F12:			charkey = K_F12;		break;
		//case SDLK_F13:			charkey = K_F13;		break;
		//case SDLK_F14:			charkey = K_F14;		break;
		//case SDLK_F15:			charkey = K_F15;		break;
		//case SDLK_INSERT:		charkey = K_INS;		break;
		//case SDLK_DELETE:		charkey = K_BACKSPACE;	break;
		//case SDLK_PAGEUP:		charkey = K_PGDN;		break;			
		//case SDLK_PAGEDOWN:		charkey = K_PGUP;		break;
		//case SDLK_HOME:			charkey = K_HOME;		break;
		//case SDLK_END:			charkey = K_END;		break;
		case SDL_SCANCODE_GRAVE: charkey = '~';          break;
		//case SDLK_q:            charkey = 'q';          break;
		//case SDLK_WORLD_0:		charkey = '~';			break;
		//case SDLK_LMETA:
		//case SDLK_RMETA:		charkey = K_COMMAND;	break;
	}
	return charkey;
}

int mx = 0, my = 0;

void ProcessEvents()
{
	SDL_Event event;
	int key = 0;
	int charkey = 0;
	Uint16* sdl_string;

	SDL_PumpEvents();

	SDL_StartTextInput();
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN: 
			{
				charkey = TranslateSDLKey(event.key.keysym.scancode);
				Key_Event( charkey, qtrue, sys_msg_time );

				// Emulate Ctrl+V
				if (event.key.keysym.sym == SDLK_v)
				{
					if (event.key.keysym.mod & KMOD_CTRL)
					{
						Key_CharEvent(22, 22);
					}
				}

				break;
			}
			case SDL_KEYUP:
			{
				charkey = TranslateSDLKey(event.key.keysym.scancode);
				Key_Event( charkey,	qfalse, sys_msg_time );
				break;
			}
			case SDL_TEXTINPUT:
			{
				sdl_string = SDL_iconv_utf8_ucs2(event.text.text);
				if (sdl_string) {
					charkey = sdl_string[0];
					key = (charkey <= 255) ? charkey : 0;
					Key_CharEvent(key, charkey);
					SDL_free(sdl_string);
				}
				break;
			}
			case SDL_MOUSEMOTION:
			{
				mx += event.motion.xrel;
				my += event.motion.yrel;
				break;
			}
			case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			{
				qboolean state = (event.button.state == SDL_PRESSED) ? qtrue : qfalse;

				switch (event.button.button) {
					case SDL_BUTTON_LEFT: Key_MouseEvent(K_MOUSE1, state, Sys_Milliseconds());
				}
				break;
			}
			/*
			/**
// * Function which is called whenever a mouse button is pressed or released.
// * @param ev the SDL event object containing the button number et all
// * @param state either qtrue if it is a keydown event or qfalse otherwise
// */
//static void _mouse_button_event( SDL_MouseButtonEvent *event, qboolean state )
//{
//	Uint8 button = event->button;
//	if( button <= 5 )
//	{
//		switch( button )
//		{
//		case SDL_BUTTON_LEFT: Key_MouseEvent( K_MOUSE1, state, Sys_Milliseconds() ); break;
//		case SDL_BUTTON_MIDDLE: Key_MouseEvent( K_MOUSE3, state, Sys_Milliseconds() ); break;
//		case SDL_BUTTON_RIGHT: Key_MouseEvent( K_MOUSE2, state, Sys_Milliseconds() ); break;
//		case SDL_BUTTON_WHEELUP: Key_Event( K_MWHEELUP, state, Sys_Milliseconds() ); break;
//		case SDL_BUTTON_WHEELDOWN: Key_Event( K_MWHEELDOWN, state, Sys_Milliseconds() ); break;
//		}
//	}
//	else if( button <= 10 )
//	{
//		// The engine only supports up to 8 buttons plus the mousewheel.
//		Key_MouseEvent( K_MOUSE1 + button - 3, state, Sys_Milliseconds() );
//	}
//	else
//		Com_Printf( "sdl_input.c: Unsupported mouse button (button = %u)\n", button );
//}
		}
	}
}

void IN_MouseMove( usercmd_t *cmd )
{
	//if( ( mx || my ) && mouse_active )
	//{
	//	CL_MouseMove( cmd, mx, my );
	//	mx = my = 0;
	//}
	CL_MouseMove( cmd, mx, my );
	mx = my= 0;
}

int main( int argc, char **argv )
{
	unsigned int oldtime, newtime, time;

	SDL_Init(SDL_INIT_VIDEO);

	Qcommon_Init( argc, argv );

	oldtime = Sys_Milliseconds();

	/* main window message loop */
	while( 1 )
	{
		// if at a full screen console, don't update unless needed
		if( Minimized || ( dedicated && dedicated->integer ) )
		{
			SDL_Delay( 1 );
		}

		ProcessEvents();

		do
		{
			newtime = Sys_Milliseconds();
			time = newtime - oldtime; // no warp problem as unsigned
			if( time > 0 )
				break;
#ifdef PUTCPU2SLEEP
			Sys_Sleep( 0 );
#endif
		}
		while( 1 );
		//Com_Printf ("time:%5.2u - %5.2u = %5.2u\n", newtime, oldtime, time);
		oldtime = newtime;

		// do as q3 (use the default floating point precision)
		//	_controlfp( ~( _EM_ZERODIVIDE /*| _EM_INVALID*/ ), _MCW_EM );
		//_controlfp( _PC_24, _MCW_PC );
		Qcommon_Frame( time );
	}

	SDL_Quit();

	// never gets here
	return 0;
}
