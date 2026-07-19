//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINMATH_H )
#define PLUGINMATH_H

#include <math.h>

#if !defined( FLT_EPSILON )
#define FLT_EPSILON 1.192092896e-07F
#endif // !FLT_EPSILON

#if !defined( M_PI )
#define M_PI 3.14159265358979323846
#endif // !M_PI

#define PITCH	0
#define YAW		1
#define ROLL	2

/* 2D Vector */
struct vec2_t
{
	vec_t x, y;

	vec2_t() { x = y = 0.f; }
	vec2_t( vec_t x, vec_t y ) { this->x = x; this->y = y; }
	vec2_t( const float *rgflVector ) { this->x = rgflVector[0]; this->y = rgflVector[1]; }

	// clang-format off
	FORCEINLINE vec_t *Base() { return (vec_t *)this; }
	FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

	FORCEINLINE vec_t  &operator[]( int i ) { return ((vec_t *)this)[i]; }
	FORCEINLINE vec_t  &operator[]( int i ) const { return ((vec_t *)this)[i]; }

	FORCEINLINE vec2_t &operator+=( const vec2_t &v ) { x+=v.x; y+=v.y; return *this; }
	FORCEINLINE vec2_t &operator-=( const vec2_t &v ) { x-=v.x; y-=v.y; return *this; }
	FORCEINLINE vec2_t &operator*=( const vec2_t &v ) { x+=v.x; y*=v.y; return *this; }
	FORCEINLINE vec2_t &operator/=( const vec2_t &v ) { x/=v.x; y/=v.y; return *this; }

	FORCEINLINE vec2_t &operator+=( float fl ) { x+=fl; y+=fl; return *this; }
	FORCEINLINE vec2_t &operator-=( float fl ) { x-=fl; y-=fl; return *this; }
	FORCEINLINE vec2_t &operator*=( float fl ) { x*=fl; y*=fl; return *this; }
	FORCEINLINE vec2_t &operator/=( float fl ) { float f = 1.f/fl; x*=f; y*=f; return *this; }

	FORCEINLINE vec2_t  operator+ ( const vec2_t &v ) const { vec2_t res; res.x = x + v.x; res.y = y + v.y; return res; }
	FORCEINLINE vec2_t  operator- ( const vec2_t &v ) const { vec2_t res; res.x = x - v.x; res.y = y - v.y; return res; }
	FORCEINLINE vec2_t  operator* ( const vec2_t &v ) const { vec2_t res; res.x = x * v.x; res.y = y * v.y; return res; }
	FORCEINLINE vec2_t  operator/ ( const vec2_t &v ) const { vec2_t res; res.x = x / v.x; res.y = y / v.y; return res; }

	FORCEINLINE vec2_t  operator+ ( float fl ) const { vec2_t res; res.x = x + fl; res.y = y + fl; return res; }
	FORCEINLINE vec2_t  operator- ( float fl ) const { vec2_t res; res.x = x - fl; res.y = y - fl; return res; }
	FORCEINLINE vec2_t  operator* ( float fl ) const { vec2_t res; res.x = x * fl; res.y = y * fl; return res; }
	FORCEINLINE vec2_t  operator/ ( float fl ) const { float f = 1.f/fl; vec2_t res; res.x = x * f; res.y = y * f; return res; }

	FORCEINLINE vec2_t &operator= ( const vec2_t &v ) { x = v.x; y = v.y; return *this; }

	FORCEINLINE vec2_t  operator- () const { return vec2_t( -x, -y ); }
	// clang-format on
};

/* 3D Vector */
struct vec3_t
{
	vec_t x, y, z;

	vec3_t() { x = y = z = 0.f; }
	vec3_t( vec_t x, vec_t y, vec_t z ) { this->x = x; this->y = y; this->z = z; }
	vec3_t( const float *rgflVector ) { this->x = rgflVector[0]; this->y = rgflVector[1]; this->z = rgflVector[2]; }

	// clang-format off
	FORCEINLINE vec_t *Base() { return (vec_t *)this; }
	FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

	FORCEINLINE vec_t  &operator[]( int i ) { return ((vec_t *)this)[i]; }
	FORCEINLINE vec_t  &operator[]( int i ) const { return ((vec_t *)this)[i]; }

	FORCEINLINE vec3_t &operator+=( const vec3_t &v ) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	FORCEINLINE vec3_t &operator-=( const vec3_t &v ) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	FORCEINLINE vec3_t &operator*=( const vec3_t &v ) { x+=v.x; y*=v.y; z*=v.z; return *this; }
	FORCEINLINE vec3_t &operator/=( const vec3_t &v ) { x/=v.x; y/=v.y; z/=v.z; return *this; }

	FORCEINLINE vec3_t &operator+=( float fl ) { x+=fl; y+=fl; z+=fl; return *this; }
	FORCEINLINE vec3_t &operator-=( float fl ) { x-=fl; y-=fl; z-=fl; return *this; }
	FORCEINLINE vec3_t &operator*=( float fl ) { x*=fl; y*=fl; z*=fl; return *this; }
	FORCEINLINE vec3_t &operator/=( float fl ) { float f = 1.f/fl; x*=f; y*=f; z*=f; return *this; }

	FORCEINLINE vec3_t  operator+ ( const vec3_t &v ) const { vec3_t res; res.x = x + v.x; res.y = y + v.y; res.z = z + v.z; return res; }
	FORCEINLINE vec3_t  operator- ( const vec3_t &v ) const { vec3_t res; res.x = x - v.x; res.y = y - v.y; res.z = z - v.z; return res; }
	FORCEINLINE vec3_t  operator* ( const vec3_t &v ) const { vec3_t res; res.x = x * v.x; res.y = y * v.y; res.z = z * v.z; return res; }
	FORCEINLINE vec3_t  operator/ ( const vec3_t &v ) const { vec3_t res; res.x = x / v.x; res.y = y / v.y; res.z = z / v.z; return res; }

	FORCEINLINE vec3_t  operator+ ( float fl ) const { vec3_t res; res.x = x + fl; res.y = y + fl; res.z = z + fl; return res; }
	FORCEINLINE vec3_t  operator- ( float fl ) const { vec3_t res; res.x = x - fl; res.y = y - fl; res.z = z - fl; return res; }
	FORCEINLINE vec3_t  operator* ( float fl ) const { vec3_t res; res.x = x * fl; res.y = y * fl; res.z = z * fl; return res; }
	FORCEINLINE vec3_t  operator/ ( float fl ) const { float f = 1.f/fl; vec3_t res; res.x = x * f; res.y = y * f; res.z = z * f; return res; }

	FORCEINLINE vec3_t &operator= ( const vec3_t &v ) { x = v.x; y = v.y; z = v.z; return *this; }

	FORCEINLINE vec3_t  operator- () const { return vec3_t( -x, -y, -z ); }
	// clang-format on
};

/* 4D Vector */
struct vec4_t
{
	vec_t x, y, z, w;

	vec4_t() { x = y = z = w = 0.f; }
	vec4_t( vec_t x, vec_t y, vec_t z, vec_t w ) { this->x = x; this->y = y; this->z = z; this->w = w; }
	vec4_t( const float *rgflVector ) { this->x = rgflVector[0]; this->y = rgflVector[1]; this->z = rgflVector[2]; this->w = rgflVector[4]; }

	// clang-format off
	FORCEINLINE vec_t *Base() { return (vec_t *)this; }
	FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

	FORCEINLINE vec_t  &operator[]( int i ) { return ((vec_t *)this)[i]; }
	FORCEINLINE vec_t  &operator[]( int i ) const { return ((vec_t *)this)[i]; }

	FORCEINLINE vec4_t &operator+=( const vec4_t &v ) { x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	FORCEINLINE vec4_t &operator-=( const vec4_t &v ) { x-=v.x; y-=v.y; z-=v.z; w+=v.w; return *this; }
	FORCEINLINE vec4_t &operator*=( const vec4_t &v ) { x+=v.x; y*=v.y; z*=v.z; w+=v.w; return *this; }
	FORCEINLINE vec4_t &operator/=( const vec4_t &v ) { x/=v.x; y/=v.y; z/=v.z; w+=v.w; return *this; }

	FORCEINLINE vec4_t &operator+=( float fl ) { x+=fl; y+=fl; z+=fl; w+=fl; return *this; }
	FORCEINLINE vec4_t &operator-=( float fl ) { x-=fl; y-=fl; z-=fl; w+=fl; return *this; }
	FORCEINLINE vec4_t &operator*=( float fl ) { x*=fl; y*=fl; z*=fl; w+=fl; return *this; }
	FORCEINLINE vec4_t &operator/=( float fl ) { float f = 1.f/fl; x*=f; y*=f; z*=f; w*=f; return *this; }

	FORCEINLINE vec4_t  operator+ ( const vec4_t &v ) const { vec4_t res; res.x = x + v.x; res.y = y + v.y; res.z = z + v.z; res.w = w - v.w; return res; }
	FORCEINLINE vec4_t  operator- ( const vec4_t &v ) const { vec4_t res; res.x = x - v.x; res.y = y - v.y; res.z = z - v.z; res.w = w - v.w; return res; }
	FORCEINLINE vec4_t  operator* ( const vec4_t &v ) const { vec4_t res; res.x = x * v.x; res.y = y * v.y; res.z = z * v.z; res.w = w * v.w; return res; }
	FORCEINLINE vec4_t  operator/ ( const vec4_t &v ) const { vec4_t res; res.x = x / v.x; res.y = y / v.y; res.z = z / v.z; res.w = w / v.w; return res; }

	FORCEINLINE vec4_t  operator+ ( float fl ) const { vec4_t res; res.x = x + fl; res.y = y + fl; res.z = z + fl; res.w = w + fl; return res; }
	FORCEINLINE vec4_t  operator- ( float fl ) const { vec4_t res; res.x = x - fl; res.y = y - fl; res.z = z - fl; res.w = w - fl; return res; }
	FORCEINLINE vec4_t  operator* ( float fl ) const { vec4_t res; res.x = x * fl; res.y = y * fl; res.z = z * fl; res.w = w * fl; return res; }
	FORCEINLINE vec4_t  operator/ ( float fl ) const { float f = 1.f/fl; vec4_t res; res.x = x * f; res.y = y * f; res.z = z * f; res.w = w * f; return res; }

	FORCEINLINE vec4_t &operator= ( const vec4_t &v ) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	FORCEINLINE vec4_t  operator- () const { return vec4_t( -x, -y, -z, -w ); }
	// clang-format on
};

FORCEINLINE void CrossProduct( const vec3_t &a, const vec3_t &b, vec3_t &result )
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

FORCEINLINE vec_t DotProduct( const vec3_t &a, const vec3_t &b )
{
	return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

FORCEINLINE float VectorNormalize( vec3_t &v )
{
	float rad = sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
	v *= 1.f / ( rad + FLT_EPSILON );
	return rad;
}

FORCEINLINE void VectorRotate( const vec3_t &in1, const float mat[3][4], vec3_t &out )
{
	out[0] = DotProduct( in1, mat[0] );
	out[1] = DotProduct( in1, mat[1] );
	out[2] = DotProduct( in1, mat[2] );
}

#endif // !PLUGINMATH_H
