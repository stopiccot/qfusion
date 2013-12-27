#include "../qcommon/qcommon.h"

void Sys_InitDynvars()
{
    
}
/*
* Sys_OpenURLInBrowser
*/
void Sys_OpenURLInBrowser( const char *url )
{
	int r;

    r = system( va( "xdg-open \"%s\"", url ) );
    if( r == 0 ) {
        // FIXME: XIconifyWindow does minimize the window, however
        // it seems that FocusIn even which follows grabs the input afterwards
        // XIconifyWindow( x11display.dpy, x11display.win, x11display.scr );
    }
}