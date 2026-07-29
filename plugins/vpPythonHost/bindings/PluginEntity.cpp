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

REGISTER( PluginEntity )
{
	m.attr( "ENT_BLDFLG_FULLBUILD" ) = ENT_BLDFLG_FULLBUILD;
	m.attr( "ENT_BLDFLG_BIT1" ) = ENT_BLDFLG_BIT1;
	m.attr( "ENT_BLDFLG_BRUSH_FACESNADOVERLAYS" ) = ENT_BLDFLG_BRUSH_FACESNADOVERLAYS;
	m.attr( "ENT_BLDFLG_BIT3" ) = ENT_BLDFLG_BIT3;
	m.attr( "ENT_BLDFLG_BIT4" ) = ENT_BLDFLG_BIT4;
	m.attr( "ENT_BLDFLG_PARSE_RENDERMODE" ) = ENT_BLDFLG_PARSE_RENDERMODE;
	m.attr( "ENT_BLDFLG_PARSE_ENTITYSTATE" ) = ENT_BLDFLG_PARSE_ENTITYSTATE;
	m.attr( "ENT_BLDFLG_UPDATE_SKY_PARAMS" ) = ENT_BLDFLG_UPDATE_SKY_PARAMS;
	m.attr( "ENT_BLDFLG_UPDATE_CLASSINFO" ) = ENT_BLDFLG_UPDATE_CLASSINFO;
	m.attr( "ENT_BLDFLG_BRUSH_DONTSORTFACES" ) = ENT_BLDFLG_BRUSH_DONTSORTFACES;
	m.attr( "ENT_BLDFLG_PATCH_CALCANDOPTIMIZE" ) = ENT_BLDFLG_PATCH_CALCANDOPTIMIZE;
	m.attr( "ENT_BLDFLG_BIT12" ) = ENT_BLDFLG_BIT12;
	m.attr( "ENT_BLDFLG_BIT13" ) = ENT_BLDFLG_BIT13;
	m.attr( "ENT_BLDFLG_BRUSH_UPDATESHADER" ) = ENT_BLDFLG_BRUSH_UPDATESHADER;

	py::class_<epair_s> epair( m, "EPair" );
	{
		epair.def_property( "key",
			[]( const epair_s &e )
			{
				return e.key;
			},
			[]( epair_s &e, const char *key )
			{
				if ( !key )
					throw py::value_error();

				if ( e.key )
					Sys_Free( e.key );

				e.key = Sys_AllocString( key );
			}
		);
		epair.def_property( "value",
			[]( const epair_s &e )
			{
				return e.value;
			},
			[]( epair_s &e, const char *value )
			{
				if ( !value )
					throw py::value_error();

				if ( e.value )
					Sys_Free( e.value );
				
				e.value = Sys_AllocString( value );
			}
		);

		epair.def( "__repr__",
			[]( const epair_s &e ) -> std::string
			{
				std::string key = std::string( e.key );
				std::string value = std::string( e.value );

				return "<epair_s key=" + key + ", value=" + value + ">";
			}
		);

		epair.def( "__str__",
			[]( const epair_s &e ) -> std::string
			{
				std::string key = std::string( e.key );
				std::string value = std::string( e.value );

				return "\"" + key + "\" \"" + value + "\"";
			}
		);
	}

	py::class_<qEntityRenderMode_s> entityRenderMode( m, "EntityRenderMode" );
	{
		entityRenderMode.def_readonly( "renderflags", &qEntityRenderMode_s::m_renderFlags );
		entityRenderMode.def_readonly( "rendercolor", &qEntityRenderMode_s::m_renderColor );
		entityRenderMode.def_readonly( "rendermode", &qEntityRenderMode_s::m_renderMode );
		entityRenderMode.def_readonly( "renderfx", &qEntityRenderMode_s::m_renderFx );

		entityRenderMode.def( "__repr__",
			[]( const qEntityRenderMode_s &inRenderMode ) -> std::string
			{
				std::string renderFlags = std::to_string( inRenderMode.m_renderFlags );
				std::string renderColor = py::repr( py::cast( inRenderMode.m_renderColor ) ).cast<std::string>();
				std::string renderMode = std::to_string( inRenderMode.m_renderMode );
				std::string renderFx = std::to_string( inRenderMode.m_renderFx );

				return "<EntityRenderMode renderFlags=" + renderFlags + ", renderColor=" + renderColor + ", renderMode=" + renderMode + ", renderFx=" + renderFx + ">";
			}
		);
	}

	py::class_<qEntityState_s> entityState( m, "EntityState" );
	{
		entityState.def_readonly( "body", &qEntityState_s::m_body );
		entityState.def_readonly( "skin", &qEntityState_s::m_skin );
		entityState.def_readonly( "sequence", &qEntityState_s::m_sequence );
		entityState.def_readonly( "framerate", &qEntityState_s::m_framerate );
		entityState.def_readonly( "scale", &qEntityState_s::m_scale );
		entityState.def_readonly( "radius", &qEntityState_s::m_radius );

		entityState.def( "__repr__",
			[]( const qEntityState_s &inEntityState ) -> std::string
			{
				std::string body = std::to_string( inEntityState.m_body );
				std::string skin = std::to_string( inEntityState.m_skin );
				std::string sequence = std::to_string( inEntityState.m_sequence );
				std::string framerate = std::to_string( inEntityState.m_framerate );
				std::string scale = std::to_string( inEntityState.m_scale );
				std::string radius = std::to_string( inEntityState.m_radius );

				return "<EntityState body=" + body + ", skin=" + skin + ", sequence=" + sequence + ", framerate=" + framerate + ", scale=" + scale + ", radius=" + radius + ">";
			}
		);
	}

	py::native_enum<modtype_s> modType( m, "ModelType", "enum.IntEnum" );
	{
		modType.value( "brush", modtype_s::mod_brush );
		modType.value( "sprite", modtype_s::mod_sprite );
		modType.value( "decal", modtype_s::mod_decal );
		modType.value( "studio", modtype_s::mod_studio );
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		modType.value( "particles", modtype_s::mod_particles );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

		modType.export_values();
		modType.finalize();
	}

	py::class_<qEntity_s> entityDef( m, "Entity" );
	{
		entityDef.def_property_readonly( "next", []( qEntity_s &e ) { return e.next; }, py::return_value_policy::reference );
		entityDef.def_property_readonly( "prev", []( qEntity_s &e ) { return e.prev; }, py::return_value_policy::reference );

		entityDef.def_property_readonly( "selectedNext", []( qEntity_s &e ) { return e.selectedNext; }, py::return_value_policy::reference );
		entityDef.def_property_readonly( "selectedPrev", []( qEntity_s &e ) { return e.selectedPrev; }, py::return_value_policy::reference );

		EXPOSE_ITERATOR( entityDef, qEntity_s, qGroup_s, entityDef.m_groupList, entityDef.m_lastGroup, "groups" );

		EXPOSE_ITERATOR( entityDef, qEntity_s, qBrush_s, entityDef.m_brushList, entityDef.m_lastBrush, "brushes" );

		entityDef.def_property_readonly( "ownerworld", []( const qEntity_s &e ) { return e.m_ownerWorld; }, py::return_value_policy::reference_internal );

		EXPOSE_ITERATOR( entityDef, qEntity_s, epair_s, entityDef.epairs, NULL, "epairs" );

		// splinenode

		entityDef.def_readonly( "classname", &qEntity_s::m_className );
		entityDef.def_readonly( "targetname", &qEntity_s::m_targetName );

		entityDef.def_readonly( "render_unkint", &qEntity_s::render_unkint );
		entityDef.def_readonly( "render_chromeage_unkint", &qEntity_s::render_chromeage_unkint );

		entityDef.def_readwrite( "spawnflags", &qEntity_s::m_spawnflags );

		entityDef.def_readonly( "editorflags", &qEntity_s::m_editorFlags );
		entityDef.def_readonly( "editorid", &qEntity_s::m_editorId );

		entityDef.def_readonly( "modeltype", &qEntity_s::m_modelType );

		entityDef.def_readwrite( "origin", &qEntity_s::m_vecOrigin );
		entityDef.def_readwrite( "angles", &qEntity_s::m_vecAngles );

		entityDef.def_readonly( "bboxmin", &qEntity_s::m_bboxMin );
		entityDef.def_readonly( "bboxmax", &qEntity_s::m_bboxMax );

		entityDef.def_readonly( "rendermode", &qEntity_s::m_renderMode );

		entityDef.def_readonly( "state", &qEntity_s::m_entityState );

		// m_entityKeys

		// m_drawData
	}
}
