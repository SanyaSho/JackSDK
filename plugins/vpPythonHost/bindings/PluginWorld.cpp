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

#include "iterator.h"

#include "pythonhost.h"

namespace py = pybind11;

REGISTER( PluginWorld )
{
	py::class_<qOverlay_s> overlayDef( m, "Overlay" );
	{
		overlayDef.def_property_readonly( "ownerbrush", []( const qOverlay_s &o ) { return o.m_ownerBrush; }, py::return_value_policy::reference_internal );
		overlayDef.def_readonly( "shader", &qOverlay_s::m_shaderInfo );

		overlayDef.def_readonly( "editorflags", &qOverlay_s::m_editorFlags );

		overlayDef.def_readwrite( "plane", &qOverlay_s::m_plane );

		overlayDef.def_readwrite( "texdef", &qOverlay_s::m_texDef );

		overlayDef.def_readonly( "bboxmin", &qOverlay_s::m_bboxMin );
		overlayDef.def_readonly( "bboxmax", &qOverlay_s::m_bboxMax );

		overlayDef.def_property_readonly( "data",
			[]( qOverlay_s &overlayDef )
			{
				py::list dataList;
				for ( auto &v : overlayDef.m_data )
				{
					dataList.append( py::cast( &v, py::return_value_policy::reference ) );
				}

				return dataList;
			}
		);
	}

	py::class_<qPatchData_s> patchData( m, "PatchData" );
	{
		patchData.def_readwrite( "position", &qPatchData_s::position );
		patchData.def_readwrite( "normal", &qPatchData_s::normal );
		patchData.def_readwrite( "uv", &qPatchData_s::uv );

		patchData.def_readonly( "flags", &qPatchData_s::m_flags );
	}

	py::class_<qPatch_s> patchDef( m, "Patch" );
	{
		patchDef.def_property_readonly( "ownerbrush", []( const qPatch_s &p ) { return p.m_ownerBrush; }, py::return_value_policy::reference_internal );
		patchDef.def_readonly( "shader", &qPatch_s::m_shaderInfo );

		patchDef.def_readwrite( "texdef", &qPatch_s::m_texDef );

		patchDef.def_readonly( "bboxmin", &qPatch_s::m_bboxMin );
		patchDef.def_readonly( "bboxmax", &qPatch_s::m_bboxMax );

		patchDef.def_readonly( "columns", &qPatch_s::m_numColumns );
		patchDef.def_readonly( "rows", &qPatch_s::m_numRows );
		patchDef.def( "__getitem__",
			[]( qPatch_s &p, py::tuple idx ) -> qPatchData_s &
			{
				if ( idx.size() != 2 )
				{
					throw py::index_error();
				}

				auto col = idx[0].cast<size_t>();
				auto row = idx[1].cast<size_t>();

				if ( col >= p.m_numColumns || row >= p.m_numRows )
				{
					throw py::index_error();
				}

				return p.m_data[col][row];
			}, py::return_value_policy::reference_internal
		);

		patchDef.def_readonly( "editorflags", &qPatch_s::m_editorFlags );
	}

	py::class_<qBrush_s> brushDef( m, "Brush" );
	{
		brushDef.def_property_readonly( "next", []( qBrush_s &b ) { return b.next; }, py::return_value_policy::reference );
		brushDef.def_property_readonly( "prev", []( qBrush_s &b ) { return b.prev; }, py::return_value_policy::reference );

		brushDef.def_property_readonly( "selectedNext", []( qBrush_s &b ) { return b.selectedNext; }, py::return_value_policy::reference );
		brushDef.def_property_readonly( "selectedPrev", []( qBrush_s &b ) { return b.selectedPrev; }, py::return_value_policy::reference );

		EXPOSE_ITERATOR( brushDef, qBrush_s, qGroup_s, brushDef.m_groupList, brushDef.m_lastGroup, "groups" );
		EXPOSE_ITERATOR( brushDef, qBrush_s, qFace_s, brushDef.m_faceList, brushDef.m_lastFace, "faces" );

		brushDef.def_property_readonly( "ownerentity", []( const qBrush_s &b ) { return b.m_ownerEntity; }, py::return_value_policy::reference_internal );

		brushDef.def_readonly( "patch", &qBrush_s::m_patch );

		brushDef.def_readonly( "overlay", &qBrush_s::m_overlay );

		EXPOSE_ITERATOR( brushDef, qBrush_s, qDecalFragment_s, brushDef.m_decalFragmentList, NULL, "decals" );

		brushDef.def_readonly( "render_unkint", &qBrush_s::render_unkint );

		brushDef.def_readonly( "editorflags", &qBrush_s::m_editorFlags );
		brushDef.def_readonly( "editorid", &qBrush_s::m_editorId );

		brushDef.def_readonly( "extradatatype", &qBrush_s::m_extraDataType );

		brushDef.def_readonly( "bboxmin", &qBrush_s::m_bboxMin );
		brushDef.def_readonly( "bboxmax", &qBrush_s::m_bboxMax );

		brushDef.def( "__repr__",
			[]( const qBrush_s &b )
			{
				std::string editorid = std::to_string( b.m_editorId );
				std::string bboxmin  = py::repr( py::cast( b.m_bboxMin ) ).cast<std::string>();
				std::string bboxmax  = py::repr( py::cast( b.m_bboxMax ) ).cast<std::string>();

				return "<Brush id=" + editorid + ", bboxmin=" + bboxmin + ", bboxmax=" + bboxmax + ">";
			}
		);
	}

	// qTrans_s

	// qSky_s

	py::class_<qSkyData_s> skyData( m, "SkyData" );
	{
		skyData.def_readonly( "shader", &qSkyData_s::m_skyShader );
		skyData.def_readonly( "axis", &qSkyData_s::m_skyAxis );
		skyData.def_readonly( "rotate", &qSkyData_s::m_skyRotate );
	}

	py::class_<qWorld_s> worldDef( m, "World" );
	{
		EXPOSE_ITERATOR( worldDef, qWorld_s, qGroup_s, worldDef.m_groupList, NULL, "groups" );
		EXPOSE_ITERATOR( worldDef, qWorld_s, qEntity_s, worldDef.m_entityList, worldDef.m_lastEntity, "entities" );
		EXPOSE_ITERATOR( worldDef, qWorld_s, qPath_s, worldDef.m_pathList, worldDef.m_lastPath, "paths" );
		EXPOSE_ITERATOR( worldDef, qWorld_s, qCamera_s, worldDef.m_cameraList, worldDef.m_lastCamera, "cameras" );

		worldDef.def_readonly( "selected_camera", &qWorld_s::m_selCamera );
		EXPOSE_SELECTEDITERATOR( worldDef, qWorld_s, qGroup_s, worldDef.m_selGroupList, NULL, "selected_groups" );
		EXPOSE_SELECTEDITERATOR( worldDef, qWorld_s, qEntity_s, worldDef.m_selEntityList, NULL, "selected_entities" );
		EXPOSE_SELECTEDITERATOR( worldDef, qWorld_s, qBrush_s, worldDef.m_selBrushList, NULL, "selected_brushes" );
		EXPOSE_SELECTEDITERATOR( worldDef, qWorld_s, qFace_s, worldDef.m_selFaceList, NULL, "selected_faces" );
		EXPOSE_SELECTEDITERATOR( worldDef, qWorld_s, qNode_s, worldDef.m_selNodeList, NULL, "selected_nodes" );

		// m_trans

		// m_sky

		worldDef.def_readonly( "editorflags", &qWorld_s::m_editorFlags );

		worldDef.def_readwrite( "cordonmin", &qWorld_s::m_vecCordonMin );
		worldDef.def_readwrite( "cordonmax", &qWorld_s::m_vecCordonMax );

		worldDef.def_readonly( "skydata", &qWorld_s::m_skyData );
	}
}
