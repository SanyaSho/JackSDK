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
#include <pybind11/native_enum.h>

#include "pythonhost.h"

namespace py = pybind11;

REGISTER( PluginFaces )
{
	py::class_<qPlane_s> plane( m, "Plane" );
	{
		plane.def_readwrite( "normal", &qPlane_s::normal );
		plane.def_readwrite( "dist", &qPlane_s::dist );
		plane.def_readwrite( "alignedaxis", &qPlane_s::alignedAxis );

		plane.def( "__repr__",
			[]( const qPlane_s &plane )
			{
				std::string normal = py::repr( py::cast( plane.normal ) ).cast<std::string>();
				std::string dist = std::to_string( plane.dist );
				std::string alignedAxis = std::to_string( plane.alignedAxis );

				return "<Plane normal=" + normal + ", dist=" + dist + ", alignedAxis=" + alignedAxis + ">";
			}
		);

		plane.def( "__str__",
			[]( const qPlane_s &plane )
			{
				std::string normal = py::str( py::cast( plane.normal ) ).cast<std::string>();
				std::string dist = std::to_string( plane.dist );
				std::string alignedAxis = std::to_string( plane.alignedAxis );

				return "normal: " + normal + ", dist: " + dist + ", alignedAxis: " + alignedAxis;
			}
		);
	}

	py::class_<qTexDef_s> texDef( m, "TexDef" );
	{
		texDef.def( py::init( []() { qTexDef_s texDef; memset( &texDef, 0, sizeof( qTexDef_s ) ); return texDef; } ) );

		texDef.def_readwrite( "uaxis", &qTexDef_s::m_UAxis );
		texDef.def_readwrite( "xshift", &qTexDef_s::m_xShift );

		texDef.def_readwrite( "vaxis", &qTexDef_s::m_VAxis );
		texDef.def_readwrite( "yshift", &qTexDef_s::m_yShift );

		texDef.def_readwrite( "scale", &qTexDef_s::m_scale );

		texDef.def_readwrite( "rotate", &qTexDef_s::m_rotate );

		texDef.def_readwrite( "texalign", &qTexDef_s::m_textureAlignment );

		texDef.def_readwrite( "value", &qTexDef_s::m_value );
		texDef.def_readwrite( "lightmapscale", &qTexDef_s::m_lightmapScale );
		texDef.def_readwrite( "surfaceflags", &qTexDef_s::m_surfaceFlags );
		texDef.def_readwrite( "contentflags", &qTexDef_s::m_contentFlags );

		texDef.def_property( "texturename",
			[]( const qTexDef_s &texDef ) -> std::string
			{
				return std::string( texDef.m_textureName );
			},
			[]( qTexDef_s &texDef, const char *textureName )
			{
				strncpy( texDef.m_textureName, textureName, sizeof( texDef.m_textureName ) );
				texDef.m_textureName[sizeof( texDef.m_textureName ) - 1] = '\0';
			}
		);

		texDef.def( "__str__",
			[]( const qTexDef_s &texDef )
			{
				std::string texturename = std::string( texDef.m_textureName );

				std::string uaxis = py::str( py::cast( texDef.m_UAxis ) ).cast<std::string>();
				std::string xshift = std::to_string( texDef.m_xShift );

				std::string vaxis = py::str( py::cast( texDef.m_VAxis ) ).cast<std::string>();
				std::string yshift = std::to_string( texDef.m_yShift );

				std::string rotate = std::to_string( texDef.m_rotate );

				std::string scale = py::str( py::cast( texDef.m_scale ) ).cast<std::string>();

				// Output as Valve220
				return texturename + " [ " + uaxis + " " + xshift + " ] [ " + vaxis + " " + yshift + " ] " + rotate + " " + scale;
			}
		);
	}

	py::class_<qVertex_s> vertex( m, "Vertex" );
	{
		vertex.def_readwrite( "coords", &qVertex_s::coords );
		vertex.def_readwrite( "uv", &qVertex_s::uv );

		vertex.def( "__repr__",
			[]( const qVertex_s &v )
			{
				std::string coords = py::repr( py::cast( v.coords ) ).cast<std::string>();
				std::string uv = py::repr( py::cast( v.uv ) ).cast<std::string>();

				return "<Vertex coords=" + coords + ", uv=" + uv + ">";
			}
		);

		vertex.def( "__str__",
			[]( const qVertex_s &v )
			{
				std::string coords = py::str( py::cast( v.coords ) ).cast<std::string>();

				// Output as Valve220
				return "( " + coords + " )";
			}
		);
	}

	py::class_<qFace_s> faceDef( m, "Face" );
	{
		faceDef.def_property_readonly( "next", []( qFace_s &f ) { return f.next; }, py::return_value_policy::reference );
		faceDef.def_property_readonly( "prev", []( qFace_s &f ) { return f.prev; }, py::return_value_policy::reference );

		faceDef.def_property_readonly( "selectedNext", []( qFace_s &f ) { return f.selectedNext; }, py::return_value_policy::reference );
		faceDef.def_property_readonly( "selectedPrev", []( qFace_s &f ) { return f.selectedPrev; }, py::return_value_policy::reference );

		faceDef.def_property_readonly( "ownerbrush", []( const qFace_s &f ) { return f.m_ownerBrush; }, py::return_value_policy::reference_internal );
		faceDef.def_readonly( "shader", &qFace_s::m_shaderInfo );

		faceDef.def_readonly( "editorflags", &qFace_s::m_editorFlags );
		faceDef.def_readonly( "editorid", &qFace_s::m_editorId );

		faceDef.def_readonly( "plane", &qFace_s::m_plane );

		faceDef.def_readonly( "texdef", &qFace_s::m_texDef );

		faceDef.def_readonly( "bboxmin", &qFace_s::m_bboxMin );
		faceDef.def_readonly( "bboxmax", &qFace_s::m_bboxMax );

		faceDef.def_readonly( "vertexcount", &qFace_s::m_vertexCount );

		faceDef.def( "__len__", []( const qFace_s &f ) { return f.m_vertexCount; } );

		faceDef.def( "__setitem__", []( qFace_s &f, size_t idx, qVertex_s &v ) { if ( idx >= static_cast<size_t>( f.m_vertexCount ) ) throw py::index_error(); f.m_vertices[idx] = v; } );
		faceDef.def( "__getitem__", []( qFace_s &f, size_t idx ) { if ( idx >= static_cast<size_t>( f.m_vertexCount ) ) throw py::index_error(); return f.m_vertices[idx]; } );

		faceDef.def( "__iter__", []( const qFace_s &f ) { return py::make_iterator( f.m_vertices, f.m_vertices + f.m_vertexCount ); }, py::keep_alive<0, 1>() );
	}
}
