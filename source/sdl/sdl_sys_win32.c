#include "../qcommon/qcommon.h"
#include "../win32/winquake.h"

static dynvar_get_status_t Sys_GetAffinity_f( void **affinity )
{
    static qboolean affinityAutoSet = qfalse;
    static char affinityString[33];
    DWORD_PTR procAffinity, sysAffinity;
    HANDLE proc = GetCurrentProcess();

    if( GetProcessAffinityMask( proc, &procAffinity, &sysAffinity ) )
    {
        SYSTEM_INFO sysInfo;
        DWORD i;

        CloseHandle( proc );

        assert( affinity );

        GetSystemInfo( &sysInfo );
        for( i = 0; i < sysInfo.dwNumberOfProcessors && i < 33; ++i )
        {
            affinityString[i] = '0' + ( ( procAffinity & sysAffinity ) & 1 );
            procAffinity >>= 1;
            sysAffinity >>= 1;
        }
        affinityString[i] = '\0';

        if( !affinityAutoSet )
        {
            // set the affinity string to something like 0001
            const char *lastBit = strrchr( affinityString, '1' );
            if( lastBit )
            {   // Vic: FIXME??
                for( i = 0; i < (DWORD)( lastBit - affinityString ); i++ )
                    affinityString[i] = '0';
            }
            affinityAutoSet = qtrue;
        }

        *affinity = affinityString;
        return DYNVAR_GET_OK;
    }

    CloseHandle( proc );
    *affinity = NULL;
    return DYNVAR_GET_TRANSIENT;
}

static dynvar_set_status_t Sys_SetAffinity_f( void *affinity )
{
    dynvar_set_status_t result = DYNVAR_SET_INVALID;
    SYSTEM_INFO sysInfo;
    DWORD_PTR procAffinity = 0, i;
    HANDLE proc = GetCurrentProcess();
    char minValid[33], maxValid[33];
    const size_t len = strlen( (char *) affinity );

    // create range of valid values for error printing
    GetSystemInfo( &sysInfo );
    for( i = 0; i < sysInfo.dwNumberOfProcessors; ++i )
    {
        minValid[i] = '0';
        maxValid[i] = '1';
    }
    minValid[i] = '\0';
    maxValid[i] = '\0';

    if( len == sysInfo.dwNumberOfProcessors )
    {
        // string is of valid length, parse in reverse direction
        const char *c;
        for( c = ( (char *) affinity ) + len - 1; c >= (char *) affinity; --c )
        {
            // parse binary digit
            procAffinity <<= 1;
            switch( *c )
            {
            case '0':
                // nothing to do
                break;
            case '1':
                // at least one digit must be 1
                result = DYNVAR_SET_OK;
                procAffinity |= 1;
                break;
            default:
                // invalid character found
                result = DYNVAR_SET_INVALID;
                goto abort;
            }
        }

        SetProcessAffinityMask( proc, procAffinity );
        //if (len > 1)
        //SetPriorityClass(proc, HIGH_PRIORITY_CLASS);
    }

abort:
    if( result != DYNVAR_SET_OK )
        Com_Printf( "\"sys_affinity\" must be a non-zero bitmask between \"%s\" and \"%s\".\n", minValid, maxValid );

    CloseHandle( proc );
    return result;
}

/*
* Sys_InitDynvars
*/
void Sys_InitDynvars( void )
{
    char *dummyStr;
    dynvar_t *affinity_var;

    affinity_var = Dynvar_Create( "sys_affinity", qtrue, Sys_GetAffinity_f, Sys_SetAffinity_f );
    assert( affinity_var );
    Dynvar_GetValue( affinity_var, (void **)&dummyStr );
    assert( dummyStr );
    Dynvar_SetValue( affinity_var, dummyStr );
}

/*
* Sys_OpenURLInBrowser
*/
void Sys_OpenURLInBrowser( const char *url )
{
	ShellExecute( NULL, "open", url, NULL, NULL, SW_SHOWNORMAL );
}

/*
* Sys_GetPreferredLanguage
* Get the preferred language through the MUI API. Works on Vista and newer.
*/
const char *Sys_GetPreferredLanguage( void )
{
	typedef BOOL (WINAPI *GetUserPreferredUILanguages_t)(DWORD, PULONG, PWSTR, PULONG);
	BOOL hr;
	ULONG numLanguages = 0;
	DWORD cchLanguagesBuffer = 0;
	HINSTANCE kernel32Dll;
	GetUserPreferredUILanguages_t GetUserPreferredUILanguages_f;
	static char lang[10];

// mingw doesn't define this
#ifndef MUI_LANGUAGE_NAME
# define MUI_LANGUAGE_NAME 0x8
#endif

	lang[0] = '\0';

	kernel32Dll = LoadLibrary( "kernel32.dll" );

	hr = FALSE;
	GetUserPreferredUILanguages_f = (void *)GetProcAddress( kernel32Dll, "GetUserPreferredUILanguages" );
	if( GetUserPreferredUILanguages_f ) {
		hr = GetUserPreferredUILanguages_f( MUI_LANGUAGE_NAME, &numLanguages, NULL, &cchLanguagesBuffer );
	}

	if( hr ) {
		WCHAR *pwszLanguagesBuffer;
		
		pwszLanguagesBuffer = Q_malloc( sizeof( WCHAR ) * cchLanguagesBuffer );
		hr = GetUserPreferredUILanguages_f( MUI_LANGUAGE_NAME, &numLanguages, pwszLanguagesBuffer, &cchLanguagesBuffer );

		if( hr ) {
			char *p;

			WideCharToMultiByte( CP_ACP, 0, pwszLanguagesBuffer, cchLanguagesBuffer, lang, sizeof(lang), NULL, NULL );
			lang[sizeof(lang)-1] = '\0';

			p = strchr( lang, '-' );
			if( p ) { *p = '\0'; }
			p = strchr( lang, '_' );
			if( p ) { *p = '\0'; }
		}

		Q_free( pwszLanguagesBuffer );	
	}

	FreeLibrary( kernel32Dll );

	if( !lang[0] ) {
		return APP_DEFAULT_LANGUAGE;
	}
	return Q_strlwr( lang );
}