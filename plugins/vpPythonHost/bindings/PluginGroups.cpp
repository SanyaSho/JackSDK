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

REGISTER( PluginGroups )
{
	py::class_<qGroup_s> groupDef( m, "Group" );
	{
		groupDef.def_property_readonly( "next", []( qGroup_s &g ) { return g.next; }, py::return_value_policy::reference );
		groupDef.def_property_readonly( "prev", []( qGroup_s &g ) { return g.prev; }, py::return_value_policy::reference );

		groupDef.def_property_readonly( "selectedNext", []( qGroup_s &g ) { return g.selectedNext; }, py::return_value_policy::reference );
		groupDef.def_property_readonly( "selectedPrev", []( qGroup_s &g ) { return g.selectedPrev; }, py::return_value_policy::reference );

		groupDef.def_readonly( "parent", &qGroup_s::m_parent );

		groupDef.def_property_readonly( "ownerworld", []( const qGroup_s &g ) { return g.m_ownerWorld; }, py::return_value_policy::reference_internal );

		groupDef.def_readonly( "editorflags", &qGroup_s::m_editorFlags );
		groupDef.def_readonly( "editorid", &qGroup_s::m_editorId );

		groupDef.def_readonly( "objectcount", &qGroup_s::m_objectCount );

		groupDef.def( "__repr__",
			[]( const qGroup_s &g )
			{
				std::string editorid = std::to_string( g.m_editorId );
				std::string objectcount = std::to_string( g.m_objectCount );

				return "<Group id=" + editorid + ", objectcount=" + objectcount + ">";
			}
		);
	}
}
