#include "../qcommon/qcommon.h"
#include <locale.h>

void Sys_InitDynvars()
{
    
}
/*
* Sys_OpenURLInBrowser
*/
void Sys_OpenURLInBrowser( const char *url )
{
	//...
}

/*
* Sys_GetPreferredLanguage
* Get the preferred language through the MUI API. Works on Vista and newer.
*/
const char *Sys_GetPreferredLanguage( void )
{
	static char lang[10];
    const char *locale;
    char *p;

    setlocale( LC_ALL, "" );
    locale = setlocale( LC_ALL, NULL );

    Q_strncpyz( lang, locale, sizeof( lang ) ); 

    p = strchr( lang, '-' );
    if( p ) { *p = '\0'; }
    p = strchr( lang, '_' );
    if( p ) { *p = '\0'; }
    p = strchr( lang, '.' );
    if( p ) { *p = '\0'; }

    if( !lang[0] ) {
        return APP_DEFAULT_LANGUAGE;
    }
    if( !Q_stricmp( lang, "C" ) ) {
        return APP_DEFAULT_LANGUAGE;
    }
    return Q_strlwr( lang );
}