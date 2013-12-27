#include "../qcommon/qcommon.h"
#include <locale.h>

cvar_t *nostdout;
static qboolean nostdout_backup_val = qfalse;

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

static void Sys_AnsiColorPrint( const char *msg )
{
    static char buffer[2096];
    int         length = 0;
    static int  q3ToAnsi[ 8 ] =
    {
        30, // COLOR_BLACK
        31, // COLOR_RED
        32, // COLOR_GREEN
        33, // COLOR_YELLOW
        34, // COLOR_BLUE
        36, // COLOR_CYAN
        35, // COLOR_MAGENTA
        0   // COLOR_WHITE
    };

    while( *msg )
    {
        char c = *msg;
        int colorindex;

        int gc = Q_GrabCharFromColorString( &msg, &c, &colorindex );
        if( gc == GRABCHAR_COLOR || (gc == GRABCHAR_CHAR && c == '\n') )
        {
            // First empty the buffer
            if( length > 0 )
            {
                buffer[length] = '\0';
                fputs( buffer, stdout );
                length = 0;
            }

            if( c == '\n' )
            {
                // Issue a reset and then the newline
                fputs( "\033[0m\n", stdout );
            }
            else
            {
                // Print the color code
                Q_snprintfz( buffer, sizeof( buffer ), "\033[%dm", q3ToAnsi[ colorindex ] );
                fputs( buffer, stdout );
            }
        }
        else if( gc == GRABCHAR_END )
            break;
        else
        {
            if( length >= sizeof( buffer ) - 1 )
                break;
            buffer[length++] = c;
        }
    }

    // Empty anything still left in the buffer
    if( length > 0 )
    {
        buffer[length] = '\0';
        fputs( buffer, stdout );
    }
}

void Sys_ConsoleOutput( char *string )
{
    if( nostdout && nostdout->integer )
        return;
    if( nostdout_backup_val )
        return;

#if 0
    fputs( string, stdout );
#else
    Sys_AnsiColorPrint( string );
#endif
}