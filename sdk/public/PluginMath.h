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

/* 2D Vector */
struct vec2_t
{
	vec_t x, y;

	vec2_t() { x = y = 0.f; }
	vec2_t( vec_t x, vec_t y ) { this->x = x; this->y = y; }

	// clang-format off
	inline vec_t  &operator[]( int i ) { return ((vec_t *)this)[i]; }
	inline vec_t  &operator[]( int i ) const { return ((vec_t *)this)[i]; }

	inline vec2_t &operator+=( const vec2_t &v ) { x+=v.x; y+=v.y; return *this; }
	inline vec2_t &operator-=( const vec2_t &v ) { x-=v.x; y-=v.y; return *this; }
	inline vec2_t &operator*=( const vec2_t &v ) { x+=v.x; y*=v.y; return *this; }
	inline vec2_t &operator/=( const vec2_t &v ) { x/=v.x; y/=v.y; return *this; }

	inline vec2_t &operator+=( float fl ) { x+=fl; y+=fl; return *this; }
	inline vec2_t &operator-=( float fl ) { x-=fl; y-=fl; return *this; }
	inline vec2_t &operator*=( float fl ) { x*=fl; y*=fl; return *this; }
	inline vec2_t &operator/=( float fl ) { float f = 1.f/fl; x*=f; y*=f; return *this; }

	inline vec2_t  operator+ ( const vec2_t &v ) { vec2_t res; res.x = x + v.x; res.y = y + v.y; return res; }
	inline vec2_t  operator- ( const vec2_t &v ) { vec2_t res; res.x = x - v.x; res.y = y - v.y; return res; }

	inline vec2_t &operator= ( const vec2_t &v ) { x = v.x; y = v.y; return *this; }

	inline vec2_t  operator- () const { return vec2_t( -x, -y ); }
	// clang-format on
};

/* 3D Vector */
struct vec3_t
{
	vec_t x, y, z;

	vec3_t() { x = y = z = 0.f; }
	vec3_t( vec_t x, vec_t y, vec_t z ) { this->x = x; this->y = y; this->z = z; }

	// clang-format off
	inline vec_t  &operator[]( int i ) { return ((vec_t *)this)[i]; }
	inline vec_t  &operator[]( int i ) const { return ((vec_t *)this)[i]; }

	inline vec3_t &operator+=( const vec3_t &v ) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	inline vec3_t &operator-=( const vec3_t &v ) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline vec3_t &operator*=( const vec3_t &v ) { x+=v.x; y*=v.y; z*=v.z; return *this; }
	inline vec3_t &operator/=( const vec3_t &v ) { x/=v.x; y/=v.y; z/=v.z; return *this; }

	inline vec3_t &operator+=( float fl ) { x+=fl; y+=fl; z+=fl; return *this; }
	inline vec3_t &operator-=( float fl ) { x-=fl; y-=fl; z-=fl; return *this; }
	inline vec3_t &operator*=( float fl ) { x*=fl; y*=fl; z*=fl; return *this; }
	inline vec3_t &operator/=( float fl ) { float f = 1.f/fl; x*=f; y*=f; z*=f; return *this; }

	inline vec3_t  operator+ ( const vec3_t &v ) { vec3_t res; res.x = x + v.x; res.y = y + v.y; res.z = z + v.z; return res; }
	inline vec3_t  operator- ( const vec3_t &v ) { vec3_t res; res.x = x - v.x; res.y = y - v.y; res.z = z - v.z; return res; }

	inline vec3_t &operator= ( const vec3_t &v ) { x = v.x; y = v.y; z = v.z; return *this; }

	inline vec3_t  operator- () const { return vec3_t( -x, -y, -z ); }
	// clang-format on
};

inline void CrossProduct( const vec3_t &a, const vec3_t &b, vec3_t &result )
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

inline vec_t DotProduct( const vec3_t &a, const vec3_t &b )
{
	return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

inline float VectorNormalize( vec3_t &v )
{
	float rad = sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
	v *= 1.f / ( rad + FLT_EPSILON );
	return rad;
}

#endif // !PLUGINMATH_H