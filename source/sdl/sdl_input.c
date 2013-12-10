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
// in_win.c -- windows mouse and joystick code
// 02/21/97 JCB Added extended DirectInput code to support external controllers.

#include "../client/client.h"
//#include "../win32/winquake.h"
//
////#ifdef __GNUC__
//#define	DIRECTINPUT_VERSION 0x0700 // Could use dx9, but older is more frequently used
////#else
////#define	DIRECTINPUT_VERSION 0x0800
////#endif
//
//#include <dinput.h>
//
//#define DINPUT_BUFFERSIZE           64 // http://www.esreality.com/?a=post&id=905276#pid905330
//#define iDirectInputCreate( a, b, c, d ) pDirectInputCreate( a, b, c, d )
//
//static HRESULT ( WINAPI *pDirectInputCreate )( HINSTANCE hinst, DWORD dwVersion,
//									   LPDIRECTINPUT * lplpDirectInput, LPUNKNOWN punkOuter );
//
//// raw input specific defines
//#define MAX_RI_DEVICE_SIZE 128
//#define INIT_RIBUFFER_SIZE (sizeof(RAWINPUTHEADER)+sizeof(RAWMOUSE))
//
//#define RI_RAWBUTTON_MASK 0x000003E0
//#define RI_INVALID_POS    0x80000000
//
//// raw input dynamic functions
//typedef int (WINAPI *pGetRawInputDeviceList)		(OUT PRAWINPUTDEVICELIST pRawInputDeviceList, IN OUT PINT puiNumDevices, IN UINT cbSize);
//typedef int (WINAPI *pGetRawInputData)				(IN HRAWINPUT hRawInput, IN UINT uiCommand, OUT LPVOID pData, IN OUT PINT pcbSize, IN UINT cbSizeHeader);
//typedef int (WINAPI *pGetRawInputDeviceInfoA)		(IN HANDLE hDevice, IN UINT uiCommand, OUT LPVOID pData, IN OUT PINT pcbSize);
//typedef BOOL (WINAPI *pRegisterRawInputDevices)		(IN PCRAWINPUTDEVICE pRawInputDevices, IN UINT uiNumDevices, IN UINT cbSize);
//
//pGetRawInputDeviceList		_GRIDL;
//pGetRawInputData			_GRID;
//pGetRawInputDeviceInfoA		_GRIDIA;
//pRegisterRawInputDevices	_RRID;
//
//typedef struct
//{
//	HANDLE			rawinputhandle; // raw input, identify particular mice
//
//	int				numbuttons;
//	volatile int	buttons;
//
//	volatile int	delta[2];
//	int				pos[2];
//} rawmouse_t;
//
//static rawmouse_t	*rawmice = NULL;
//static int			rawmicecount = 0;
//static RAWINPUT		*raw = NULL;
//static int			ribuffersize = 0;
//static qboolean		rawinput_initialized = qfalse;
//
//static qboolean	IN_RawInput_Init( void );
//static void		IN_RawInput_Shutdown( void );
//static int		IN_RawInput_Register( void );
//static void		IN_RawInput_DeRegister( void );
//
//extern unsigned	sys_msg_time;
//
//// joystick defines and variables
//// where should defines be moved?
//#define JOY_ABSOLUTE_AXIS   0x00000000      // control like a joystick
//#define JOY_RELATIVE_AXIS   0x00000010      // control like a mouse, spinner, trackball
//#define	JOY_MAX_AXES	    6               // X, Y, Z, R, U, V
//#define JOY_AXIS_X	    0
//#define JOY_AXIS_Y	    1
//#define JOY_AXIS_Z	    2
//#define JOY_AXIS_R	    3
//#define JOY_AXIS_U	    4
//#define JOY_AXIS_V	    5
//
//enum _ControlList
//{
//	AxisNada = 0, AxisForward, AxisLook, AxisSide, AxisTurn, AxisUp
//};
//
//DWORD dwAxisFlags[JOY_MAX_AXES] =
//{
//	JOY_RETURNX, JOY_RETURNY, JOY_RETURNZ, JOY_RETURNR, JOY_RETURNU, JOY_RETURNV
//};
//
//DWORD dwAxisMap[JOY_MAX_AXES];
//DWORD dwControlMap[JOY_MAX_AXES];
//PDWORD pdwRawValue[JOY_MAX_AXES];
//
cvar_t *in_mouse;
cvar_t *in_grabinconsole;
//cvar_t *in_joystick;


// none of these cvars are saved over a session
// this means that advanced controller configuration needs to be executed
// each time.  this avoids any problems with getting back to a default usage
// or when changing from one controller to another.  this way at least something
// works.
//cvar_t *joy_name;
//cvar_t *joy_advanced;
//cvar_t *joy_advaxisx;
//cvar_t *joy_advaxisy;
//cvar_t *joy_advaxisz;
//cvar_t *joy_advaxisr;
//cvar_t *joy_advaxisu;
//cvar_t *joy_advaxisv;
//cvar_t *joy_forwardthreshold;
//cvar_t *joy_sidethreshold;
//cvar_t *joy_pitchthreshold;
//cvar_t *joy_yawthreshold;
//cvar_t *joy_forwardsensitivity;
//cvar_t *joy_sidesensitivity;
//cvar_t *joy_pitchsensitivity;
//cvar_t *joy_yawsensitivity;
//cvar_t *joy_upthreshold;
//cvar_t *joy_upsensitivity;
//cvar_t *joy_freelook;
//cvar_t *joy_lookspring;
//cvar_t *joy_lookstrafe;
//
//static qboolean	jlooking = qfalse;
//qboolean joy_avail, joy_advancedinit, joy_haspov;
//DWORD joy_oldbuttonstate, joy_oldpovstate;
//
//int joy_id;
//DWORD joy_flags;
//DWORD joy_numbuttons;
//
//static JOYINFOEX ji;
//
//qboolean in_appactive;

// forward-referenced functions
//static void IN_StartupJoystick( void );
//static void Joy_AdvancedUpdate_f( void );

/*
============================================================

MOUSE CONTROL

============================================================
*/

// used by win_vid.c
int mouse_buttons;
int mouse_wheel_type;

static int mouse_oldbuttonstate;
//static POINT current_pos;
static int mx, my;
static qboolean	mouseactive;    // qfalse when not focus app
static qboolean	restore_spi;
static qboolean	mouseinitialized;
static int originalmouseparms[3], newmouseparms[3] = { 0, 0, 0 };
static qboolean	mouseparmsvalid;
static unsigned int mstate_di;

static int window_center_x, window_center_y;
//static RECT window_rect;


/*
* IN_StartupMouse
*/
static void IN_StartupMouse( void )
{
	//cvar_t *cv;

	//cv = Cvar_Get( "in_initmouse", "1", CVAR_NOSET );
	//if( !cv->integer )
	//	return;

	//dinput_initialized = qfalse;
	//rawinput_initialized = qfalse;

	//cv = Cvar_Get( "m_raw", "0", CVAR_ARCHIVE );
	//if( cv->integer )
	//	rawinput_initialized = IN_RawInput_Init();

	//if( !rawinput_initialized )
	//{
	//	cv = Cvar_Get( "in_dinput", "1", CVAR_ARCHIVE );
	//	if( cv->integer )
	//		dinput_initialized = IN_InitDInput();
	//}

	//if( rawinput_initialized )
	//	Com_Printf( "Raw input initialized with %i mice\n", rawmicecount );
	//else if( dinput_initialized )
	//	Com_Printf( "DirectInput initialized\n" );
	//else
	//	Com_Printf( "DirectInput not initialized, using standard input\n" );

	//mouseinitialized = qtrue;
	//mouseparmsvalid = SystemParametersInfo( SPI_GETMOUSE, 0, originalmouseparms, 0 );
	//mouse_buttons = 8;
	//mouse_wheel_type = MWHEEL_UNKNOWN;
}

/*
* IN_MouseEvent
*/
void IN_MouseEvent( int mstate )
{
	//int i;

	//if( !mouseinitialized || dinput_initialized )
	//	return;
	//if( ( cls.key_dest == key_console ) && !in_grabinconsole->integer )
	//	return;

	//// perform button actions
	//for( i = 0; i < mouse_buttons; i++ )
	//{
	//	if( ( mstate & ( 1<<i ) ) &&
	//		!( mouse_oldbuttonstate & ( 1<<i ) ) )
	//		Key_MouseEvent( K_MOUSE1 + i, qtrue, sys_msg_time );

	//	if( !( mstate & ( 1<<i ) ) &&
	//		( mouse_oldbuttonstate & ( 1<<i ) ) )
	//		Key_MouseEvent( K_MOUSE1 + i, qfalse, sys_msg_time );
	//}

	//mouse_oldbuttonstate = mstate;
}

/*
=========================================================================

VIEW CENTERING

=========================================================================
*/
cvar_t *v_centermove;
cvar_t *v_centerspeed;


/*
* IN_Init
*/
void IN_Init( void )
{
	Com_Printf( "\n------- SDL2 input initialization -------\n" );

	// mouse variables
	in_mouse	    = Cvar_Get( "in_mouse", "1", CVAR_ARCHIVE );
	in_grabinconsole	= Cvar_Get( "in_grabinconsole",	"0", CVAR_ARCHIVE );

	IN_StartupMouse();

	Com_Printf( "------------------------------------\n" );
}

/*
* IN_Shutdown
*/
void IN_Shutdown( void )
{
	//IN_DeactivateMouse();

	//if( rawinput_initialized )
	//	IN_RawInput_Shutdown();
	//else if( dinput_initialized )
	//	IN_ShutdownDInput();

	//Cmd_RemoveCommand( "+jlook" );
	//Cmd_RemoveCommand( "-jlook" );
	//Cmd_RemoveCommand( "joy_advancedupdate" );

	//dinput_acquired = dinput_initialized = qfalse;
	//rawinput_initialized = qfalse;
}

/*
* IN_Restart
*/
void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init();
}

/*
* IN_Activate
* 
* Called when the main window gains or loses focus.
* The window may have been destroyed and recreated
* between a deactivate and an activate.
*/
void IN_Activate( qboolean active )
{
//	in_appactive = active;
//	mouseactive = !active;  // force a new window check or turn off
}


/*
* IN_Frame
* 
* Called every frame, even if not generating commands
*/
void IN_Frame( void )
{
	extern cvar_t *vid_fullscreen;

	//if( !mouseinitialized )
	//	return;

	//if( vid_fullscreen && (!vid_fullscreen->integer || cl_parent_hwnd) )
	//{
	//	extern cvar_t *in_grabinconsole;

	//	// if we have a parent window (say, a browser plugin window) and
	//	// the window is not focused, deactivate the input
	//	if( cl_parent_hwnd && !AppFocused )
	//	{
	//		if( in_appactive )
	//			IN_Activate( qfalse );
	//	}
	//	else if( in_grabinconsole->integer || cls.key_dest != key_console )
	//	{
	//		if( !in_appactive && ActiveApp )
	//			IN_Activate( qtrue );
	//	}
	//	else
	//	{
	//		if( in_appactive )
	//			IN_Activate( qfalse );
	//	}
	//}

	//if( !in_mouse || !in_appactive )
	//{
	//	IN_DeactivateMouse();
	//	return;
	//}

	//IN_ActivateMouse();
}

/*
=========================================================================

JOYSTICK

=========================================================================
*/

/*
* IN_StartupJoystick
*/
static void IN_StartupJoystick( void )
{
}

/*
* IN_Commands
*/
void IN_Commands( void )
{
}

/*
* IN_ReadJoystick
*/
static qboolean IN_ReadJoystick( void )
{
	return qfalse;
}

/*
* IN_JoyMove
*/
void IN_JoyMove( usercmd_t *cmd )
{
}
