//=====================================================================================//
//
// Purpose: A part of vstdlib.dll used by J.A.C.K. and it's plugins
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( VSTDLIB_STATIC_H )
#define VSTDLIB_STATIC_H

extern int V_Hash( const char *src, int tableSize );

extern void V_SinCosPrecise( float *sinOut, float *cosOut, float angle );

extern char *V_Stristr( const char *haystack, const char *needle );

extern void V_StripExtension( char *in );

#define V_FixSlashes( in, oldsep, newsep )          \
	do                                              \
	{                                               \
		for ( int i = 0; i < ARRAYSIZE( in ); i++ ) \
		{                                           \
			if ( in[i] == 0 )                       \
				break;                              \
			if ( in[i] == oldsep )                  \
				in[i] = newsep;                     \
		}                                           \
	} while ( 0 )

#endif // !VSTDLIB_STATIC_H
