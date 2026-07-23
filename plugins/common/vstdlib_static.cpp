//=====================================================================================//
//
// Purpose: A part of vstdlib.dll used by J.A.C.K. and it's plugins
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <math.h>

#if 0
#include <immintrin.h>
#endif

#include "PluginMeta.h"

#include "vstdlib_static.h"

/*
================
V_Hash
================
*/
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

/*
================
V_SinCosPrecise
================
*/
void V_SinCosPrecise( float *sinOut, float *cosOut, float angle )
{
	*sinOut = sin( angle );
	*cosOut = cos( angle );
}

/*
================
V_Stristr
https://stackoverflow.com/a/27305359
================
*/
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

/*
================
V_StripExtension
================
*/
void V_StripExtension( char *in )
{
	if ( !in || !in[0] )
		return;

	size_t end = strlen( in ) - 1;

	while ( end > 0 && in[end] != '.' && !( in[end] == '/' || in[end] == '\\' ) )
	{
		--end;
	}

	if ( end != 0 )
		in[end] = '\0';
}

/*
================
V_ExtractFileBase
================
*/
int V_ExtractFileBase( const char *source, char *destination, size_t size )
{
	if ( !source || !source[0] || !destination || size == 0 )
		return 0;

	const char *start = source;
	const char *end = source + strlen( source );

	// Find the last path separator or extension separator
	const char *dot = end;
	const char *slash = end;

	while ( slash > source )
	{
		char c = slash[-1];

		if ( c == '/' || c == '\\' )
			break;

		if ( c == '.' )
			dot = slash - 1;

		slash--;
	}

	// If we found a path separator, the basename starts after it
	start = slash;

	if ( *start == '/' || *start == '\\' )
		start++;

	// If there is no filename
	if ( start >= end )
	{
		destination[0] = '\0';
		return 0;
	}

	// Remove extension
	size_t length;

	if ( dot > start )
		length = dot - start;
	else
		length = end - start;

	if ( length >= size )
		length = size - 1;

	strncpy( destination, start, length );
	destination[length] = '\0';

	return (int)length;
}

/*
================
V_ConcatTransforms
================
*/
void V_ConcatTransforms (const float in1[3][4], const float in2[3][4], float out[3][4])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
				in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
				in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
				in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
				in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
				in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
				in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
				in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
				in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
				in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
				in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
				in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
				in1[2][2] * in2[2][3] + in1[2][3];
}

/*
================
V_BuildTransformStudioMatrix
================
*/
void V_BuildTransformStudioMatrix( float yaw, float pitch, float roll, float *rgflOrigin, float scale, float matrix[3][4] )
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = ( roll * M_PI ) / 180.f;
	sr = sin( angle );
	cr = cos( angle );
	angle = ( pitch * M_PI ) / 180.f;
	sp = sin( angle );
	cp = cos( angle );
	angle = ( yaw * M_PI ) / 180.f;
	sy = sin( angle );
	cy = cos( angle );

	matrix[0][0] = (cp*cy)*scale;
	matrix[1][0] = (cp*sy)*scale;
	matrix[2][0] = (-sp)*scale;

	float crcy = (cr*cy);
	float crsy = (cr*sy);
	float srcy = (sr*cy);
	float srsy = (sr*sy);
	matrix[0][1] = (sp*srcy-crsy)*scale;
	matrix[1][1] = (sp*srsy+crcy)*scale;
	matrix[2][1] = (sr*cp)*scale;

	matrix[0][2] = (sp*crcy+srsy)*scale;
	matrix[1][2] = (sp*crsy-srcy)*scale;
	matrix[2][2] = (cr*cp)*scale;

	matrix[0][3] = rgflOrigin[0];
	matrix[1][3] = rgflOrigin[1];
	matrix[2][3] = rgflOrigin[2];
}

/*
================
V_AngleQuaternion
================
*/
void V_AngleQuaternion (const vec3_t angles, vec4_t &quaternion)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	// FIXME: rescale the inputs to 1/2 angle
	angle = ((angles[2] * M_PI)/180.f) * 0.5;
	sy = sin(angle);
	cy = cos(angle);
	angle = ((angles[1] * M_PI)/180.f) * 0.5;
	sp = sin(angle);
	cp = cos(angle);
	angle = ((angles[0] * M_PI)/180.f) * 0.5;
	sr = sin(angle);
	cr = cos(angle);

	quaternion[0] = sr*cp*cy-cr*sp*sy; // X
	quaternion[1] = cr*sp*cy+sr*cp*sy; // Y
	quaternion[2] = cr*cp*sy-sr*sp*cy; // Z
	quaternion[3] = cr*cp*cy+sr*sp*sy; // W
}

/*
================
V_QuaternionMatrix
================
*/
void V_QuaternionMatrix (const vec4_t quaternion, float (*matrix)[4])
{
	matrix[0][0] = 1.0 - 2.0 * quaternion[1] * quaternion[1] - 2.0 * quaternion[2] * quaternion[2];
	matrix[1][0] = 2.0 * quaternion[0] * quaternion[1] + 2.0 * quaternion[3] * quaternion[2];
	matrix[2][0] = 2.0 * quaternion[0] * quaternion[2] - 2.0 * quaternion[3] * quaternion[1];

	matrix[0][1] = 2.0 * quaternion[0] * quaternion[1] - 2.0 * quaternion[3] * quaternion[2];
	matrix[1][1] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[2] * quaternion[2];
	matrix[2][1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[3] * quaternion[0];

	matrix[0][2] = 2.0 * quaternion[0] * quaternion[2] + 2.0 * quaternion[3] * quaternion[1];
	matrix[1][2] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[3] * quaternion[0];
	matrix[2][2] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[1] * quaternion[1];
}

/*
================
V_QuaternionSlerp
================
*/
void V_QuaternionSlerp (const vec4_t p, vec4_t q, float t, vec4_t &qt)
{
	int i;
	static float omega, cosom, sinom, sclp, sclq;

	// decide if one of the quaternions is backwards
	float a = 0;
	float b = 0;
	for (i = 0; i < 4; i++) {
		a += (p[i]-q[i])*(p[i]-q[i]);
		b += (p[i]+q[i])*(p[i]+q[i]);
	}
	if (a > b) {
		for (i = 0; i < 4; i++) {
			q[i] = -q[i];
		}
	}

	cosom = p[0]*q[0] + p[1]*q[1] + p[2]*q[2] + p[3]*q[3];

	if ((1.0 + cosom) > 0.00000001) {
		if ((1.0 - cosom) > 0.00000001) {
			omega = acos( cosom );
			sinom = sin( omega );
			sclp = sin( (1.0 - t)*omega) / sinom;
			sclq = sin( t*omega ) / sinom;
		}
		else {
			sclp = 1.0 - t;
			sclq = t;
		}
		for (i = 0; i < 4; i++) {
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}
	else {
		qt[0] = -p[1];
		qt[1] = p[0];
		qt[2] = -p[3];
		qt[3] = p[2];
		sclp = sin( (1.0 - t) * 0.5 * M_PI);
		sclq = sin( t * 0.5 * M_PI);
		for (i = 0; i < 3; i++) {
			qt[i] = sclp * p[i] + sclq * qt[i];
		}
	}
}

#if 0
/*
================
V_HalfToFloat
================
*/
float V_HalfToFloat( short h )
{
	__m128i v = _mm_cvtsi32_si128( h );
	__m128 result = _mm_cvtph_ps( v );

	return _mm_cvtss_f32( result );
}
#endif

/*
================
V_ExtractFileExtension
================
*/
void V_ExtractFileExtension( const char *path, char *dest, int destSize )
{
	const char *src = path + strlen( path ) - 1;

	while ( src != path && *( src - 1 ) != '.' )
		src--;

	if ( src == path )
	{
		*dest = '\0';
		return;
	}

	src--;

	strncpy( dest, src, destSize );
	dest[destSize - 1] = '\0';
}

/*
================
V_Strupr
================
*/
char *V_Strupr( char *start )
{
	unsigned char *str = (unsigned char *)start;
	while ( *str )
	{
		if ( (unsigned char)( *str - 'a' ) <= ( 'z' - 'a' ) )
			*str -= 'a' - 'A';
		else if ( (unsigned char)*str >= 0x80 ) // non-ascii, fall back to CRT
			*str = toupper( *str );
		str++;
	}
	return start;
}

/*
================
V_Atoi
================
*/
int V_Atoi( const char *string )
{
	return atoi( string );
}

/*
================
V_Atof
================
*/
float V_Atof( const char *string )
{
	return (float)atof( string );
}
