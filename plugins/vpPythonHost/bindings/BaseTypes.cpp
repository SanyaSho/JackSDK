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

REGISTER( BaseTypes )
{
	py::class_<rgba_t> rgba( m, "rgba_t" );
	{
		rgba.def( py::init( []( byte r, byte g, byte b, byte a ) { rgba_t rgba{}; rgba.r = r;  rgba.g = g; rgba.b = b; rgba.a = a; return rgba; } ) );

		rgba.def( "__getitem__",
			[]( const rgba_t &rgba, size_t i )
			{
				switch ( i )
				{
				case 0:
					return rgba.r;
				case 1:
					return rgba.g;
				case 2:
					return rgba.b;
				case 3:
					return rgba.a;
				default:
					throw py::index_error();
				};
			}
		);
		rgba.def( "__setitem__",
			[]( rgba_t &rgba, size_t i, byte value )
			{
				switch ( i )
				{
				case 0:
					rgba.r = value;
					break;
				case 1:
					rgba.g = value;
					break;
				case 2:
					rgba.b = value;
					break;
				case 3:
					rgba.a = value;
					break;
				default:
					throw py::index_error();
				};
			}
		);

		rgba.def( "__repr__",
			[]( const rgba_t &rgba ) -> std::string
			{
				std::string r = std::to_string( rgba.r );
				std::string g = std::to_string( rgba.g );
				std::string b = std::to_string( rgba.b );
				std::string a = std::to_string( rgba.a );

				return "<rgba_t r=" + r + ", g=" + g + ", b=" + b + ", a=" + a + ">";
			}
		);

		rgba.def( "__str__",
			[]( const rgba_t &rgba ) -> std::string
			{
				std::string r = std::to_string( rgba.r );
				std::string g = std::to_string( rgba.g );
				std::string b = std::to_string( rgba.b );
				std::string a = std::to_string( rgba.a );

				return r + " " + g + " " + b + " " + a;
			}
		);
	}
}
