//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <stdarg.h>

#if defined( WIN32 )
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif // WIN32

/*
===============
Sys_Printf
===============
*/
void Sys_Printf( const char *format, ... )
{
	if ( !format )
	{
		return;
	}

	static char buf[2048] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( buf, sizeof( buf ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	OutputDebugString( buf );
	OutputDebugString( "\n" );
#else
	printf( "%s\n", buf );
#endif // WIN32
}

/*
===============
Sys_DPrintf
===============
*/
void Sys_DPrintf( const char *format, ... )
{
	if ( !format )
	{
		return;
	}

	static char buf[2048] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( buf, sizeof( buf ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	OutputDebugString( buf );
	OutputDebugString( "\n" );
#else
	printf( "%s\n", buf );
#endif // WIN32
}

/*
===============
Sys_Warning
===============
*/
void Sys_Warning( const char *format, ... )
{
	if ( !format )
	{
		return;
	}

	static char buf[2048] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( buf, sizeof( buf ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	OutputDebugString( "WARNING: " );
	OutputDebugString( buf );
	OutputDebugString( "\n" );
#else
	printf( "WARNING: %s\n", buf );
#endif // WIN32
}

/*
===============
Sys_Error
===============
*/
void Sys_Error( const char *format, ... )
{
	if ( !format )
	{
		return;
	}

	static char buf[2048] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( buf, sizeof( buf ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	OutputDebugString( "ERROR: " );
	OutputDebugString( buf );
	OutputDebugString( "\n" );
#else
	printf( "ERROR: %s\n", buf );
#endif // WIN32
}
