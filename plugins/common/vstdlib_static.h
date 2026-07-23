//=====================================================================================//
//
// Purpose: A part of vstdlib.dll used by J.A.C.K. and it's plugins
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( VSTDLIB_STATIC_H )
#define VSTDLIB_STATIC_H

struct vec3_t;
struct vec4_t;

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

extern int V_ExtractFileBase( const char *source, char *destination, size_t size );

extern void V_ConcatTransforms (const float in1[3][4], const float in2[3][4], float out[3][4]);

extern void V_BuildTransformStudioMatrix( float yaw, float pitch, float roll, float *rgflOrigin, float scale, float matrix[3][4] );

extern void V_AngleQuaternion (const vec3_t angles, vec4_t &quaternion);

extern void V_QuaternionMatrix (const vec4_t quaternion, float (*matrix)[4]);

extern void V_QuaternionSlerp (const vec4_t p, vec4_t q, float t, vec4_t &qt);

#if 0
extern float V_HalfToFloat( short h );
#endif

extern void V_ExtractFileExtension( const char *path, char *dest, int destSize );

extern char *V_Strupr( char *start );

extern int V_Atoi( const char *string );
extern float V_Atof( const char *string );

#endif // !VSTDLIB_STATIC_H
