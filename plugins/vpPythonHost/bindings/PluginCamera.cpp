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

REGISTER( PluginCamera )
{
	py::class_<qCamera_s> cameraDef( m, "Camera" );
	{
		cameraDef.def_property_readonly( "next", []( qCamera_s &e ) { return e.next; }, py::return_value_policy::reference );
		cameraDef.def_property_readonly( "prev", []( qCamera_s &e ) { return e.prev; }, py::return_value_policy::reference );

		cameraDef.def_property_readonly( "ownerworld", []( const qCamera_s &c ) { return c.m_ownerWorld; }, py::return_value_policy::reference_internal );

		cameraDef.def_readonly( "editorflags", &qCamera_s::m_editorFlags );
		cameraDef.def_readonly( "editorid", &qCamera_s::m_editorId );

		cameraDef.def_readonly( "origin", &qCamera_s::m_vecOrigin );
		cameraDef.def_readonly( "angles", &qCamera_s::m_vecAngles );
	}
}
