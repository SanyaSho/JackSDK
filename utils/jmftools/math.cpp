//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <memory>

// Plugin API
#include "BaseTypes.h"

vec3_t vec3_origin( 0.f, 0.f, 0.f );

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
===============
FormatFloat
===============
*/
static const char *FormatFloat( int precision, bool forceDecimal, float value )
{
	static char buffers[32][32];
	static unsigned currentBuffer = 0;

	char *out = buffers[currentBuffer];
	currentBuffer = ( currentBuffer + 1 ) & 31;

	memset( out, 0, 32 );

	if ( precision >= 0 )
	{
		char format[8];

		strcpy( format, "%.0f" );

		if ( precision <= 8 )
		{
			format[2] = char( '0' + precision );
			sprintf( out, format, value );

			if ( precision == 0 )
				return out;
		}
		else
		{
			format[2] = '9';
			sprintf( out, format, value );
		}

		int len = (int)strlen( out ) - 1;

		if ( len > 0 )
		{
			char *p = &out[len];

			while ( len > 0 && *p == '0' )
			{
				*p-- = '\0';
				--len;
			}

			if ( *p == '.' )
			{
				if ( forceDecimal )
				{
					if ( (int)strlen( out ) - 1 > len )
					{
						p[1] = '0';
						p[2] = '\0';
					}
				}
				else
				{
					*p = '\0';
				}
			}
		}

		return out;
	}

	sprintf( out, "%g", value );
	return out;
}

/*
===============
Sys_PrintValue
===============
*/
/*static*/ const char *Sys_PrintValue( float value )
{
	if ( value == 0.f )
	{
		return "0";
	}

	return FormatFloat( 6, false, value );
}

/*
===============
Sys_PrintMapCoord
===============
*/
/*static*/ const char *Sys_PrintMapCoord( float coord )
{
	if ( coord == 0.f )
	{
		return "0";
	}

	return FormatFloat( 6, false, coord );
}

/*
===============
Sys_PrintAxis
===============
*/
/*static*/ const char *Sys_PrintAxis( float axis )
{
	if ( axis == 0.f )
	{
		return "0";
	}

	return FormatFloat( 6, false, axis );
}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

/*
===============
Sys_SnapVertex
===============
*/
/*static*/ void Sys_SnapVertex( float *rgflVertex )
{
}

/*
===============
Sys_SnapAxis
===============
*/
/*static*/ void Sys_SnapAxis( int num, float *rgflAxis )
{
}

/*
===============
Sys_SnapVertexToGrid
===============
*/
/*static*/ void Sys_SnapVertexToGrid( float *rgflVertex )
{
}

/*
===============
Sys_SnapMapVertex
===============
*/
/*static*/ void Sys_SnapMapVertex( float *rgflVertex )
{
}
