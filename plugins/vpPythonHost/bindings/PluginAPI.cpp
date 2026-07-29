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
#define PLUGINEDITORFUNCTIONS_H
#include "PluginAPI.h"

#define ENTITYAPI_NO_INLINEFUNCS
#include "PluginEntity.h"

#include "PluginWorld.h"

extern plugin_funcs_t gEditorfuncs;

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include "pythonhost.h"

namespace py = pybind11;


/*
 LoggingAPI
*/
static void Sys_Printf( const char *str )
{
	gEditorfuncs.pfnSys_Printf( "%s", str );
}
static void Sys_DPrintf( const char *str )
{
	gEditorfuncs.pfnSys_DPrintf( "%s", str );
}
static void Sys_Warning( const char *str )
{
	gEditorfuncs.pfnSys_Warning( "%s", str );
}
static void Sys_Error( const char *str )
{
	gEditorfuncs.pfnSys_Error( "%s", str );
}


/*
 Misc API
*/
static float Sys_FloatTime()
{
	return gEditorfuncs.pfnSys_FloatTime();
}

static void Sys_SetOption( int option, int value )
{
	gEditorfuncs.pfnSys_SetOption( option, value );
}

static int Sys_GetOption( int option )
{
	return gEditorfuncs.pfnSys_GetOption( option );
}


/*
 Steam API
*/

static void Steam_SetAchievemnt( int achIdx )
{
	gEditorfuncs.pfnSteam_SetAchievemnt( achIdx );
}


/*
 
*/

static const char *V_VersionString()
{
	return gEditorfuncs.pfnV_VersionString();
}

static float Sys_GetTextureGamma()
{
	return gEditorfuncs.pfnSys_GetTextureGamma();
}

static qWorld_s *Global_GetCurrentWorld()
{
	return gEditorfuncs.pfnGlobal_GetCurrentWorld();
}

static std::string BuildPackageList( qWorld_s *worldDef, char listSeparator, int removeVolumePrefix )
{
	char *outBuf = NULL;

	bool ret = gEditorfuncs.pfnBuildPackageList( worldDef, &outBuf, listSeparator, removeVolumePrefix );
	if ( !ret || !outBuf )
	{
		return std::string {};
	}

	std::string packageList( outBuf );

	gEditorfuncs.pfnSys_Free( outBuf );

	return packageList;
}


/*
 Entity API
*/
static qEntity_s *Entity_Create( qWorld_s *worldDef, const char *classname, const vec3_t &rgflOrigin, int editorFlags )
{
	return gEditorfuncs.entityfuncs.pfnEntity_Create( worldDef, classname, rgflOrigin.Base(), editorFlags );
}

static void Entity_Build( qEntity_s *entityDef, int entityBuildFlags )
{
	gEditorfuncs.entityfuncs.pfnEntity_Build( entityDef, entityBuildFlags );
}

static void Entity_SetColor( qEntity_s *entityDef, const rgba_t &cbColor )
{
	gEditorfuncs.entityfuncs.pfnEntity_SetColor( entityDef, cbColor.data() );
}

static rgba_t Entity_GetColor( qEntity_s *entityDef )
{
	rgba_t entityColor;
	gEditorfuncs.entityfuncs.pfnEntity_GetColor( entityDef, entityColor.data() );
	return entityColor;
}

#define Entity_AddToVisGroup	  ( gEditorfuncs.entityfuncs.pfnEntity_AddToVisGroup )
#define Entity_RemoveFromVisGroup ( gEditorfuncs.entityfuncs.pfnEntity_RemoveFromVisGroup )
#define Entity_GetVisGroupCount	  ( gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupCount )
#define Entity_GetVisGroupIdent	  ( gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupIdent )

static qEntity_s *Entity_FindByClassname( qEntity_s *entityDef, const char *classname )
{
	return gEditorfuncs.entityfuncs.pfnEntity_FindByClassname( entityDef, classname );
}

static qEntity_s *Entity_FindByTargetname( qEntity_s *entityDef, const char *targetname )
{
	return gEditorfuncs.entityfuncs.pfnEntity_FindByTargetname( entityDef, targetname );
}

static qEntity_s *Entity_FindByKeyValue( qEntity_s *entityDef, const char *key, const char *value )
{
	return gEditorfuncs.entityfuncs.pfnEntity_FindByKeyValue( entityDef, key, value );
}


/*
 Brush API
 */
static qBrush_s *Brush_Create( qWorld_s *worldDef, qEntity_s *entityDef )
{
	return gEditorfuncs.brushfuncs.pfnBrush_Create( worldDef, entityDef );
}

static void Brush_Destroy( qWorld_s *worldDef, qBrush_s *brushDef )
{
	gEditorfuncs.brushfuncs.pfnBrush_Destroy( worldDef, brushDef );
}

static void Brush_SetColor( qBrush_s *brushDef, const rgba_t &cbColor )
{
	gEditorfuncs.brushfuncs.pfnBrush_SetColor( brushDef, cbColor.data() );
}

static rgba_t Brush_GetColor( qBrush_s *brushDef )
{
	rgba_t entityColor;
	gEditorfuncs.brushfuncs.pfnBrush_GetColor( brushDef, entityColor.data() );
	return entityColor;
}

#define Brush_AddToVisGroup		 ( gEditorfuncs.brushfuncs.pfnBrush_AddToVisGroup )
#define Brush_RemoveFromVisGroup ( gEditorfuncs.brushfuncs.pfnBrush_RemoveFromVisGroup )
#define Brush_GetVisGroupCount	 ( gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupCount )
#define Brush_GetVisGroupIdent	 ( gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupIdent )


/*
 Face API
 */
static qFace_s *Face_Create( qWorld_s *worldDef, qBrush_s *brushDef, const qTexDef_s &texDef, int vertexCount )
{
	return gEditorfuncs.facefuncs.pfnFace_Create( worldDef, brushDef, &texDef, vertexCount );
}

static void Face_Destroy( qWorld_s *worldDef, qFace_s *faceDef )
{
	gEditorfuncs.facefuncs.pfnFace_Destroy( worldDef, faceDef );
}


/*
 DialogAPI
*/
static bool Dialog_MessageBox( const char *title, const char *text, int flags )
{
	return gEditorfuncs.dialogfuncs.pfnDialog_MessageBox( title, text, flags );
}

static int Dialog_CheckOptions( const char *title, const char *text, const char *options )
{
	return gEditorfuncs.dialogfuncs.pfnDialog_CheckOptions( title, text, options );
}

static void Dialog_Begin( const char *title )
{
	gEditorfuncs.dialogfuncs.pfnDialog_Begin( title );
}

static void Dialog_InitExternalCommand( const char *buttonText, const char *command )
{
	gEditorfuncs.dialogfuncs.pfnDialog_InitExternalCommand( buttonText, command );
}

static void Dialog_InitInternalCommand( const char *buttonText, py::function pfnCommand )
{
	static py::function s_internalCommand;
	s_internalCommand = std::move( pfnCommand );

	gEditorfuncs.dialogfuncs.pfnDialog_InitInternalCommand( buttonText, []()
		{
			py::gil_scoped_acquire gil;

			try
			{
				s_internalCommand();
			}
			catch( std::exception &e )
			{
				Sys_Error( e.what() );
			}
		}
	);
}

static void Dialog_SetProgress( int value, int maxValue )
{
	gEditorfuncs.dialogfuncs.pfnDialog_SetProgress( value, maxValue );
}

static void Dialog_AddTextEdit( const char *controlName, const char *title, const char *defaultValue, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddTextEdit( controlName, title, defaultValue, flags );
}

static void Dialog_AddCheckBox( const char *controlName, const char *title, bool defaultValue, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddCheckBox( controlName, title, defaultValue, flags );
}

static void Dialog_AddRadioBox( const char *controlName, const char *title, bool defaultValue, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddRadioBox( controlName, title, defaultValue, flags );
}

static void Dialog_AddSpinBox( const char *controlName, const char *title, int defaultValue, int minValue, int maxValue, int stepCount, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBox( controlName, title, defaultValue, minValue, maxValue, stepCount, flags );
}

static void Dialog_AddSpinBoxFloat( const char *controlName, const char *title, float defaultValue, float minValue, float maxValue, float stepCount, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBoxFloat( controlName, title, defaultValue, minValue, maxValue, stepCount, flags );
}

static void Dialog_AddComboBox( const char *controlName, const char *title, int selectedIndex, const char *optionsList, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddComboBox( controlName, title, selectedIndex, optionsList, flags );
}

static void Dialog_AddFileEdit( const char *controlName, const char *title, const char *defaultValue, const char *extensionList, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddFileEdit( controlName, title, defaultValue, extensionList, flags );
}

static void Dialog_AddFileList( const char *controlName, const char *title, const char *fileList, const char *extensionList, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddFileList( controlName, title, fileList, extensionList, flags );
}

static void Dialog_AddDirectoryEdit( const char *controlName, const char *title, const char *defaultValue, int flags )
{
	gEditorfuncs.dialogfuncs.pfnDialog_AddDirectoryEdit( controlName, title, defaultValue, flags );
}

static std::string Dialog_QueryArgument( const char *controlName )
{
	char buf[4096] = { 0 };
	gEditorfuncs.dialogfuncs.pfnDialog_QueryArgument( controlName, buf, sizeof( buf ) );

	return std::string( buf );
}

static int Dialog_QueryArgumentInt( const char *controlName )
{
	return gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentInt( controlName );
}

static float Dialog_QueryArgumentFloat( const char *controlName )
{
	return gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentFloat( controlName );
}

static void Dialog_End()
{
	gEditorfuncs.dialogfuncs.pfnDialog_End();
}

static void Dialog_Printf( const char *str )
{
	gEditorfuncs.dialogfuncs.pfnDialog_Printf( "%s", str );
}

static void Dialog_BeginWait()
{
	gEditorfuncs.dialogfuncs.pfnDialog_BeginWait();
}

static void Dialog_EndWait()
{
	gEditorfuncs.dialogfuncs.pfnDialog_EndWait();
}


REGISTER( PluginAPI )
{
	// 1 << 0
	m.attr( "EFL_SELECTED" ) = EFL_SELECTED;
	m.attr( "EFL_DIRTY" ) = EFL_DIRTY;
	m.attr( "EFL_HIDDEN" ) = EFL_HIDDEN;
	m.attr( "EFL_TRANSPARENT" ) = EFL_TRANSPARENT;
	m.attr( "EFL_WORLDSPAWN" ) = EFL_WORLDSPAWN;
	m.attr( "EFL_INVENTORYITEM" ) = EFL_INVENTORYITEM;
	m.attr( "EFL_PATHWAY" ) = EFL_PATHWAY;
	// 1 << 8
	m.attr( "EFL_IGNORE" ) = EFL_IGNORE;
	// 1 << 20
	m.attr( "EFL_CORDON" ) = EFL_CORDON;

	py::module log = m.def_submodule( "log", "LoggingAPI" );
	log.def( "Printf", &Sys_Printf );
	log.def( "DPrintf", &Sys_DPrintf );
	log.def( "Warning", &Sys_Warning );
	log.def( "Error", &Sys_Error );

	py::module misc = m.def_submodule( "misc", "MiscAPI" );
	misc.def( "FloatTime", &Sys_FloatTime );
	misc.def( "SetOption", &Sys_SetOption );
	misc.def( "GetOption", &Sys_GetOption );

	py::module steam = m.def_submodule( "steam", "SteamAPI" );
	steam.def( "SetAchievemnt", &Steam_SetAchievemnt );

	m.def( "VersionString", &V_VersionString );
	m.def( "GetTextureGamma", &Sys_GetTextureGamma );
	m.def( "GetCurrentWorld", &Global_GetCurrentWorld, py::return_value_policy::reference );
	m.def( "BuildPackageList", &BuildPackageList );

	// RenderingAPI
	// ParserAPI
	// FileSystemAPI
	// MathAPI

	/* Entity API */
	py::module entity = m.def_submodule( "entity", "EntityAPI" );
	entity.def( "Create", &Entity_Create, py::return_value_policy::reference );
	entity.def( "Build", &Entity_Build );
	entity.def( "SetColor", &Entity_SetColor );
	entity.def( "GetColor", &Entity_GetColor );
	entity.def( "FindByClassname", &Entity_FindByClassname, py::return_value_policy::reference );
	entity.def( "FindByTargetname", &Entity_FindByTargetname, py::return_value_policy::reference );
	entity.def( "FindByKeyValue", &Entity_FindByKeyValue, py::return_value_policy::reference );

	py::module brush = m.def_submodule( "brush", "BrushAPI" );
	brush.def( "Create", &Brush_Create, py::return_value_policy::reference );
	brush.def( "Destroy", &Brush_Destroy );
	brush.def( "SetColor", &Brush_SetColor );
	brush.def( "GetColor", &Brush_GetColor );

	py::module face = m.def_submodule( "face", "FaceAPI" );
	face.def( "Create", &Face_Create, py::return_value_policy::reference );
	face.def( "Destroy", &Face_Destroy );

	/* Dialog API */
	py::module dialog = m.def_submodule( "dialog", "DialogAPI" );
	dialog.def( "MessageBox", &Dialog_MessageBox );
	dialog.def( "CheckOptions", &Dialog_CheckOptions );
	dialog.def( "Begin", &Dialog_Begin );
	dialog.def( "InitExternalCommand", &Dialog_InitExternalCommand );
	dialog.def( "InitInternalCommand", &Dialog_InitInternalCommand );
	dialog.def( "SetProgress", &Dialog_SetProgress );
	dialog.def( "AddTextEdit", &Dialog_AddTextEdit );
	dialog.def( "AddCheckBox", &Dialog_AddCheckBox );
	dialog.def( "AddRadioBox", &Dialog_AddRadioBox );
	dialog.def( "AddSpinBox", &Dialog_AddSpinBox );
	dialog.def( "AddSpinBoxFloat", &Dialog_AddSpinBoxFloat );
	dialog.def( "AddComboBox", &Dialog_AddComboBox );
	dialog.def( "AddFileEdit", &Dialog_AddFileEdit );
	dialog.def( "AddFileList", &Dialog_AddFileList );
	dialog.def( "AddDirectoryEdit", &Dialog_AddDirectoryEdit );
	dialog.def( "QueryArgument", &Dialog_QueryArgument );
	dialog.def( "QueryArgumentInt", &Dialog_QueryArgumentInt );
	dialog.def( "QueryArgumentFloat", &Dialog_QueryArgumentFloat );
	dialog.def( "End", &Dialog_End );
	dialog.def( "Printf", &Dialog_Printf );
	dialog.def( "BeginWait", &Dialog_BeginWait );
	dialog.def( "EndWait", &Dialog_EndWait );
}
