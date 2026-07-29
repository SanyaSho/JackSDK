//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>
#include <vector>

// Plugin API
#include "PluginMeta.h"

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "pythonhost.h"

namespace py = pybind11;

REGISTER( PluginMath )
{
	py::class_<vec2_t> vec2( m, "vec2_t" );
	{
		vec2.def( py::init<>() );
		vec2.def( py::init<float, float>() );
		vec2.def( py::init<const float *>() );

		//FORCEINLINE vec_t *Base() { return (vec_t *)this; }
		//FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

		vec2.def( "__getitem__", []( const vec2_t &v, size_t i ) { if ( i >= 2 ) throw py::index_error(); return v[i]; } );
		vec2.def( "__setitem__", []( const vec2_t &v, size_t i, float value ) { if ( i >= 2 ) throw py::index_error(); v[i] = value; } );

		vec2.def( py::self += py::self );
		vec2.def( py::self -= py::self );
		vec2.def( py::self *= py::self );
		vec2.def( py::self /= py::self );

		vec2.def( py::self += float() );
		vec2.def( py::self -= float() );
		vec2.def( py::self *= float() );
		vec2.def( py::self /= float() );

		vec2.def( py::self + py::self );
		vec2.def( py::self - py::self );
		vec2.def( py::self * py::self );
		vec2.def( py::self / py::self );

		vec2.def( py::self + float() );
		vec2.def( py::self - float() );
		vec2.def( py::self * float() );
		vec2.def( py::self / float() );

		//FORCEINLINE vec2_t &operator= ( const vec2_t &v ) { x = v.x; y = v.y; return *this; }

		vec2.def( -py::self );

		vec2.def_readwrite( "x", &vec2_t::x );
		vec2.def_readwrite( "y", &vec2_t::y );

		vec2.def( "__repr__",
			[]( const vec2_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );

				return "<vec2_t x=" + x + ", y=" + y + ">";
			}
		);

		vec2.def( "__str__",
			[]( const vec2_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );

				return x + " " + y;
			}
		);
	}

	py::class_<vec3_t> vec3( m, "vec3_t" );
	{
		vec3.def( py::init<>() );
		vec3.def( py::init<float, float, float>() );
		vec3.def( py::init<const float *>() );

		//FORCEINLINE vec_t *Base() { return (vec_t *)this; }
		//FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

		vec3.def( "__getitem__", []( const vec3_t &v, size_t i ) { if ( i >= 3 ) throw py::index_error(); return v[i]; } );
		vec3.def( "__setitem__", []( const vec3_t &v, size_t i, float value ) { if ( i >= 3 ) throw py::index_error(); v[i] = value; } );

		vec3.def( py::self += py::self );
		vec3.def( py::self -= py::self );
		vec3.def( py::self *= py::self );
		vec3.def( py::self /= py::self );

		vec3.def( py::self += float() );
		vec3.def( py::self -= float() );
		vec3.def( py::self *= float() );
		vec3.def( py::self /= float() );

		vec3.def( py::self + py::self );
		vec3.def( py::self - py::self );
		vec3.def( py::self * py::self );
		vec3.def( py::self / py::self );

		vec3.def( py::self + float() );
		vec3.def( py::self - float() );
		vec3.def( py::self * float() );
		vec3.def( py::self / float() );

		//FORCEINLINE vec3_t &operator= ( const vec3_t &v ) { x = v.x; y = v.y; z = v.z; return *this; }

		vec3.def( -py::self );

		vec3.def_readwrite( "x", &vec3_t::x );
		vec3.def_readwrite( "y", &vec3_t::y );
		vec3.def_readwrite( "z", &vec3_t::z );

		vec3.def( "__repr__",
			[]( const vec3_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );
				std::string z = std::to_string( v.z );

				return "<vec3_t x=" + x + ", y=" + y + ", z=" + z + ">";
			}
		);

		vec3.def( "__str__",
			[]( const vec3_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );
				std::string z = std::to_string( v.z );

				return x + " " + y + " " + z;
			}
		);
	}

	py::class_<vec4_t> vec4( m, "vec4_t" );
	{
		vec4.def( py::init<>() );
		vec4.def( py::init<float, float, float, float>() );
		vec4.def( py::init<const float *>() );

		//FORCEINLINE vec_t *Base() { return (vec_t *)this; }
		//FORCEINLINE const vec_t *Base() const { return (vec_t *)this; }

		vec4.def( "__getitem__", []( const vec4_t &v, size_t i ) { if ( i >= 4 ) throw py::index_error(); return v[i]; } );
		vec4.def( "__setitem__", []( const vec4_t &v, size_t i, float value ) { if ( i >= 4 ) throw py::index_error(); v[i] = value; } );

		vec4.def( py::self += py::self );
		vec4.def( py::self -= py::self );
		vec4.def( py::self *= py::self );
		vec4.def( py::self /= py::self );

		vec4.def( py::self += float() );
		vec4.def( py::self -= float() );
		vec4.def( py::self *= float() );
		vec4.def( py::self /= float() );

		vec4.def( py::self + py::self );
		vec4.def( py::self - py::self );
		vec4.def( py::self * py::self );
		vec4.def( py::self / py::self );

		vec4.def( py::self + float() );
		vec4.def( py::self - float() );
		vec4.def( py::self * float() );
		vec4.def( py::self / float() );

		//FORCEINLINE vec4_t &operator= ( const vec4_t &v ) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

		vec4.def( -py::self );

		vec4.def_readwrite( "x", &vec4_t::x );
		vec4.def_readwrite( "y", &vec4_t::y );
		vec4.def_readwrite( "z", &vec4_t::z );
		vec4.def_readwrite( "w", &vec4_t::w );

		vec4.def( "__repr__",
			[]( const vec4_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );
				std::string z = std::to_string( v.z );
				std::string w = std::to_string( v.w );

				return "<vec4_t x=" + x + ", y=" + y + ", z=" + z + ", w=" + w + ">";
			}
		);

		vec4.def( "__str__",
			[]( const vec4_t &v ) -> std::string
			{
				std::string x = std::to_string( v.x );
				std::string y = std::to_string( v.y );
				std::string z = std::to_string( v.z );
				std::string w = std::to_string( v.w );

				return x + " " + y + " " + z + " " + w;
			}
		);
	}
}
