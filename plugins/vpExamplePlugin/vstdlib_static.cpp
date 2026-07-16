//=====================================================================================//
//
// Purpose: A part of vstdlib.dll used by J.A.C.K. and it's plugins
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <math.h>

#include "vstdlib_static.h"

int V_Hash( const char *src, int tableSize )
{
	if ( !src[0] )
		return 0;

	int hash = 0;

	for ( int i = 0; src[i] != NULL; i++ )
	{
		hash += ( 119 + i ) * (unsigned char)tolower( (unsigned char)src[i] );
	}

	return hash & ( tableSize - 1 );
}

void V_SinCosPrecise( float *sinOut, float *cosOut, float angle )
{
	*sinOut = sin( angle );
	*cosOut = cos( angle );
}

// https://stackoverflow.com/a/27305359
char *V_Stristr( const char *haystack, const char *needle )
{
	do
	{
		const char *h = haystack;
		const char *n = needle;
		while ( tolower( (unsigned char)*h ) == tolower( (unsigned char)*n ) && *n )
		{
			h++;
			n++;
		}
		if ( *n == 0 )
		{
			return (char *)haystack;
		}
	} while ( *haystack++ );
	return 0;
}

void V_StripExtension( char *in )
{
	if ( !in || !in[0] )
		return;

	size_t end = strlen( in ) - 1;

	while ( end > 0 && in[end] != '.' && !( in[end] == '/' || in[end] == '\\' ) )
	{
		--end;
	}

	if ( in != 0 )
		in[end] = '\0';
}
