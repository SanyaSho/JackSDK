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

#endif // !VSTDLIB_STATIC_H