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
