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

#include "iterator.h"

namespace py = pybind11;

REGISTER( PluginNodes )
{
	py::class_<qNode_s> nodeDef( m, "Node" );
	{
		nodeDef.def_property_readonly( "next", []( qNode_s &n ) { return n.next; }, py::return_value_policy::reference );
		nodeDef.def_property_readonly( "prev", []( qNode_s &n ) { return n.prev; }, py::return_value_policy::reference );

		nodeDef.def_property_readonly( "selectedNext", []( qNode_s &n ) { return n.selectedNext; }, py::return_value_policy::reference );
		nodeDef.def_property_readonly( "selectedPrev", []( qNode_s &n ) { return n.selectedPrev; }, py::return_value_policy::reference );

		nodeDef.def_property_readonly( "ownerpath", []( const qNode_s &n ) { return n.m_ownerPath; }, py::return_value_policy::reference_internal );

		EXPOSE_ITERATOR( nodeDef, qNode_s, epair_s, nodeDef.epairs, NULL, "epairs" );

		nodeDef.def_readwrite( "nameoverride", &qNode_s::m_nameOverride );
		nodeDef.def_readwrite( "fireonpass", &qNode_s::m_fireEntityOnPass );

		nodeDef.def_readonly( "render_unkint", &qNode_s::render_unkint );

		nodeDef.def_readonly( "editorflags", &qNode_s::m_editorFlags );

		nodeDef.def_readonly( "nodeidx", &qNode_s::m_nodeIndex );

		nodeDef.def_readonly( "editorid", &qNode_s::m_editorId );

		nodeDef.def_readwrite( "origin", &qNode_s::m_vecOrigin );
		nodeDef.def_readwrite( "angles", &qNode_s::m_vecAngles );

		nodeDef.def_readwrite( "speed", &qNode_s::m_speed );
		nodeDef.def_readwrite( "yaw_speed", &qNode_s::m_yaw_speed );
		nodeDef.def_readwrite( "wait", &qNode_s::m_wait );
		nodeDef.def_readwrite( "fov", &qNode_s::m_fov );

		nodeDef.def_readwrite( "spawnflags", &qNode_s::m_spawnflags );
	}
}
