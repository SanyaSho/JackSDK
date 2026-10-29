//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string.h>

/*static*/ void *Sys_Malloc( size_t size )
{
	void *ptr = calloc( 1, size );
	if ( !ptr )
	{
		//CFloatWinding::cleanup();
		//CDoubleWinding::cleanup();
		//TempBuffer_Flush();
		return calloc( 1, size );
	}

	return ptr;
}

/*static*/ void Sys_Free( void *ptr )
{
	if ( ptr )
	{
		free( ptr );
	}
}

/*static*/ char *Sys_AllocString( const char *src )
{
	if ( !src )
	{
		return NULL;
	}

	char *dest = (char *)Sys_Malloc( strlen( src ) + 1 );
	strcat( dest, src );

	return dest;
}
