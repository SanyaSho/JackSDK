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

#include "PluginRender.h"

#include "PluginWorld.h"

extern plugin_funcs_t gEditorfuncs;

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include "pythonhost.h"

namespace py = pybind11;


// clang-format off

/*
 LoggingAPI
*/
static void Sys_Printf( const char *str )																															{ gEditorfuncs.pfnSys_Printf( "%s", str ); }
static void Sys_DPrintf( const char *str )																															{ gEditorfuncs.pfnSys_DPrintf( "%s", str ); }
static void Sys_Warning( const char *str )																															{ gEditorfuncs.pfnSys_Warning( "%s", str ); }
static void Sys_Error( const char *str )																															{ gEditorfuncs.pfnSys_Error( "%s", str ); }


/*
 Misc API
*/
static float Sys_FloatTime()																																		{ return gEditorfuncs.pfnSys_FloatTime(); }

static void Sys_SetOption( int option, int value )																													{ gEditorfuncs.pfnSys_SetOption( option, value ); }
static int Sys_GetOption( int option )																																{ return gEditorfuncs.pfnSys_GetOption( option ); }


/*
 Steam API
*/
static void Steam_SetAchievemnt( int achIdx )																														{ gEditorfuncs.pfnSteam_SetAchievemnt( achIdx ); }


/*
 Rendering API
*/
static void PR_PointSize( float size )																																{ gEditorfuncs.renderingfuncs.pfnPR_PointSize( size ); }
static void PR_LineWidth( float width )																																{ gEditorfuncs.renderingfuncs.pfnPR_LineWidth( width ); }
static void PR_BindShader( qShader_s *shaderDef )																													{ gEditorfuncs.renderingfuncs.pfnPR_BindShader( shaderDef ); }
static void PR_BindTexture( qTexture_s *textureDef )																												{ gEditorfuncs.renderingfuncs.pfnPR_BindTexture( textureDef ); }
static void PR_Begin( primType_e primType )																															{ gEditorfuncs.renderingfuncs.pfnPR_Begin( primType ); }
static void PR_End()																																				{ gEditorfuncs.renderingfuncs.pfnPR_End(); }
static void PR_Color4ub( byte r, byte g, byte b, byte a )																											{ gEditorfuncs.renderingfuncs.pfnPR_Color4ub( r, g, b, a ); }
static void PR_Color4ubv( const byte *cbColor )																														{ gEditorfuncs.renderingfuncs.pfnPR_Color4ubv( cbColor ); }
static void PR_TexCoord2f( float s, float t )																														{ gEditorfuncs.renderingfuncs.pfnPR_TexCoord2f( s, t ); }
static void PR_TexCoord2fv( const vec2_t &rgflTexCoord )																											{ gEditorfuncs.renderingfuncs.pfnPR_TexCoord2fv( rgflTexCoord.Base() ); }
static void PR_Normal3fv( const vec3_t &rgflNormal )																												{ gEditorfuncs.renderingfuncs.pfnPR_Normal3fv( rgflNormal.Base() ); }
static void PR_Vertex3fv( const vec3_t &rgflVertex )																												{ gEditorfuncs.renderingfuncs.pfnPR_Vertex3fv( rgflVertex.Base() ); }
static unsigned int PR_GetState()																																	{ return gEditorfuncs.renderingfuncs.pfnPR_GetState(); }
static void PR_SetState( glStateBits_e stateBits )																													{ gEditorfuncs.renderingfuncs.pfnPR_SetState( stateBits ); }
static viewInfo_s PR_GetViewInfo()																																	{ viewInfo_s viewInfo; gEditorfuncs.renderingfuncs.pfnPR_GetViewInfo( &viewInfo ); return viewInfo; }
static float PR_GetMinAlpha()																																		{ return gEditorfuncs.renderingfuncs.pfnPR_GetMinAlpha(); }
static float PR_CalcLighting( const vec3_t &rgflNormal )																											{ return gEditorfuncs.renderingfuncs.pfnPR_CalcLighting( rgflNormal.Base() ); }


/*
 Math API
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
static char *Sys_PrintValue( float value )																															{ return gEditorfuncs.mathfuncs.pfnSys_PrintValue( value ); }
static char *Sys_PrintMapCoord( float coord )																														{ return gEditorfuncs.mathfuncs.pfnSys_PrintMapCoord( coord ); }
static char *Sys_PrintAxis( float axis )																															{ return gEditorfuncs.mathfuncs.pfnSys_PrintAxis( axis ); }
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
static void Sys_SnapVertex( vec3_t &rgflVertex )																													{ gEditorfuncs.mathfuncs.pfnSys_SnapVertex( rgflVertex.Base() ); }
static void Sys_SnapAxis( int num, vec3_t &rgflAxis )																												{ gEditorfuncs.mathfuncs.pfnSys_SnapAxis( num, rgflAxis.Base() ); }
static void Sys_SnapVertexToGrid( vec3_t &rgflVertex )																												{ gEditorfuncs.mathfuncs.pfnSys_SnapVertexToGrid( rgflVertex.Base() ); }
static void Sys_SnapMapVertex( vec3_t &rgflVertex )																													{ gEditorfuncs.mathfuncs.pfnSys_SnapMapVertex( rgflVertex.Base() ); }


/*
 
*/
static const char *V_VersionString()																																{ return gEditorfuncs.pfnV_VersionString(); }

static float Sys_GetTextureGamma()																																	{ return gEditorfuncs.pfnSys_GetTextureGamma(); }

static qWorld_s *Global_GetCurrentWorld()																															{ return gEditorfuncs.pfnGlobal_GetCurrentWorld(); }

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
static qEntity_s *Entity_Create( qWorld_s *worldDef, const char *classname, const vec3_t &rgflOrigin, int editorFlags )												{ return gEditorfuncs.entityfuncs.pfnEntity_Create( worldDef, classname, rgflOrigin.Base(), editorFlags ); }
static void Entity_Build( qEntity_s *entityDef, int entityBuildFlags )																								{ gEditorfuncs.entityfuncs.pfnEntity_Build( entityDef, entityBuildFlags ); }
static void Entity_SetColor( qEntity_s *entityDef, const rgba_t &cbColor )																							{ gEditorfuncs.entityfuncs.pfnEntity_SetColor( entityDef, cbColor.data() ); }
static rgba_t Entity_GetColor( qEntity_s *entityDef )																												{ rgba_t entityColor; gEditorfuncs.entityfuncs.pfnEntity_GetColor( entityDef, entityColor.data() ); return entityColor; }
static void Entity_AddToVisGroup( qWorld_s *worldDef, qEntity_s *entityDef, unsigned int visGroupId )																{ gEditorfuncs.entityfuncs.pfnEntity_AddToVisGroup( worldDef, entityDef, visGroupId ); }
static void Entity_RemoveFromVisGroup( qWorld_s *worldDef, qEntity_s *entityDef, unsigned int visGroupId )															{ gEditorfuncs.entityfuncs.pfnEntity_RemoveFromVisGroup( worldDef, entityDef, visGroupId ); }
static int Entity_GetVisGroupCount( qEntity_s *entityDef )																											{ return gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupCount( entityDef ); }
static int Entity_GetVisGroupIdent( qEntity_s *entityDef, int visGroupId )																							{ return gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupIdent( entityDef, visGroupId ); }
static qEntity_s *Entity_FindByClassname( qEntity_s *entityDef, const char *classname )																				{ return gEditorfuncs.entityfuncs.pfnEntity_FindByClassname( entityDef, classname ); }
static qEntity_s *Entity_FindByTargetname( qEntity_s *entityDef, const char *targetname )																			{ return gEditorfuncs.entityfuncs.pfnEntity_FindByTargetname( entityDef, targetname ); }
static qEntity_s *Entity_FindByKeyValue( qEntity_s *entityDef, const char *key, const char *value )																	{ return gEditorfuncs.entityfuncs.pfnEntity_FindByKeyValue( entityDef, key, value ); }


/*
 Brush API
 */
static qBrush_s *Brush_Create( qWorld_s *worldDef, qEntity_s *entityDef )																							{ return gEditorfuncs.brushfuncs.pfnBrush_Create( worldDef, entityDef ); }
static void Brush_Destroy( qWorld_s *worldDef, qBrush_s *brushDef )																									{ gEditorfuncs.brushfuncs.pfnBrush_Destroy( worldDef, brushDef ); }
static void Brush_SetColor( qBrush_s *brushDef, const rgba_t &cbColor )																								{ gEditorfuncs.brushfuncs.pfnBrush_SetColor( brushDef, cbColor.data() ); }
static rgba_t Brush_GetColor( qBrush_s *brushDef )																													{ rgba_t entityColor; gEditorfuncs.brushfuncs.pfnBrush_GetColor( brushDef, entityColor.data() ); return entityColor; }
static void Brush_AddToVisGroup( qWorld_s *worldDef, qBrush_s *brushDef, unsigned int visGroupId )																	{ gEditorfuncs.brushfuncs.pfnBrush_AddToVisGroup( worldDef, brushDef, visGroupId ); }
static void Brush_RemoveFromVisGroup( qWorld_s *worldDef, qBrush_s *brushDef, unsigned int visGroupId )																{ gEditorfuncs.brushfuncs.pfnBrush_RemoveFromVisGroup( worldDef, brushDef, visGroupId ); }
static int Brush_GetVisGroupCount( qBrush_s *brushDef )																												{ return gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupCount( brushDef ); }
static int Brush_GetVisGroupIdent( qBrush_s *brushDef, int visGroupId )																								{ return gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupIdent( brushDef, visGroupId ); }


/*
 Face API
 */
static qFace_s *Face_Create( qWorld_s *worldDef, qBrush_s *brushDef, const qTexDef_s &texDef, int vertexCount )														{ return gEditorfuncs.facefuncs.pfnFace_Create( worldDef, brushDef, &texDef, vertexCount ); }
static void Face_Destroy( qWorld_s *worldDef, qFace_s *faceDef )																									{ gEditorfuncs.facefuncs.pfnFace_Destroy( worldDef, faceDef ); }


/*
 DialogAPI
*/
static bool Dialog_MessageBox( const char *title, const char *text, int flags )																						{ return gEditorfuncs.dialogfuncs.pfnDialog_MessageBox( title, text, flags ); }
static int Dialog_CheckOptions( const char *title, const char *text, const char *options )																			{ return gEditorfuncs.dialogfuncs.pfnDialog_CheckOptions( title, text, options ); }
static void Dialog_Begin( const char *title )																														{ gEditorfuncs.dialogfuncs.pfnDialog_Begin( title ); }
static void Dialog_InitExternalCommand( const char *buttonText, const char *command )																				{ gEditorfuncs.dialogfuncs.pfnDialog_InitExternalCommand( buttonText, command ); }
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
static void Dialog_SetProgress( int value, int maxValue )																											{ gEditorfuncs.dialogfuncs.pfnDialog_SetProgress( value, maxValue ); }
static void Dialog_AddTextEdit( const char *controlName, const char *title, const char *defaultValue, int flags )													{ gEditorfuncs.dialogfuncs.pfnDialog_AddTextEdit( controlName, title, defaultValue, flags ); }
static void Dialog_AddCheckBox( const char *controlName, const char *title, bool defaultValue, int flags )															{ gEditorfuncs.dialogfuncs.pfnDialog_AddCheckBox( controlName, title, defaultValue, flags ); }
static void Dialog_AddRadioBox( const char *controlName, const char *title, bool defaultValue, int flags )															{ gEditorfuncs.dialogfuncs.pfnDialog_AddRadioBox( controlName, title, defaultValue, flags ); }
static void Dialog_AddSpinBox( const char *controlName, const char *title, int defaultValue, int minValue, int maxValue, int stepCount, int flags )					{ gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBox( controlName, title, defaultValue, minValue, maxValue, stepCount, flags ); }
static void Dialog_AddSpinBoxFloat( const char *controlName, const char *title, float defaultValue, float minValue, float maxValue, float stepCount, int flags )	{ gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBoxFloat( controlName, title, defaultValue, minValue, maxValue, stepCount, flags ); }
static void Dialog_AddComboBox( const char *controlName, const char *title, int selectedIndex, const char *optionsList, int flags )									{ gEditorfuncs.dialogfuncs.pfnDialog_AddComboBox( controlName, title, selectedIndex, optionsList, flags ); }
static void Dialog_AddFileEdit( const char *controlName, const char *title, const char *defaultValue, const char *extensionList, int flags )						{ gEditorfuncs.dialogfuncs.pfnDialog_AddFileEdit( controlName, title, defaultValue, extensionList, flags ); }
static void Dialog_AddFileList( const char *controlName, const char *title, const char *fileList, const char *extensionList, int flags )							{ gEditorfuncs.dialogfuncs.pfnDialog_AddFileList( controlName, title, fileList, extensionList, flags ); }
static void Dialog_AddDirectoryEdit( const char *controlName, const char *title, const char *defaultValue, int flags )												{ gEditorfuncs.dialogfuncs.pfnDialog_AddDirectoryEdit( controlName, title, defaultValue, flags ); }
static std::string Dialog_QueryArgument( const char *controlName )																									{ char buf[4096] = { 0 }; gEditorfuncs.dialogfuncs.pfnDialog_QueryArgument( controlName, buf, sizeof( buf ) ); return std::string( buf ); }
static int Dialog_QueryArgumentInt( const char *controlName )																										{ return gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentInt( controlName ); }
static float Dialog_QueryArgumentFloat( const char *controlName )																									{ return gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentFloat( controlName ); }
static void Dialog_End()																																			{ gEditorfuncs.dialogfuncs.pfnDialog_End(); }
static void Dialog_Printf( const char *str )																														{ gEditorfuncs.dialogfuncs.pfnDialog_Printf( "%s", str ); }
static void Dialog_BeginWait()																																		{ gEditorfuncs.dialogfuncs.pfnDialog_BeginWait(); }
static void Dialog_EndWait()																																		{ gEditorfuncs.dialogfuncs.pfnDialog_EndWait(); }

// clang-format on


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

	// FileSystemAPI

	/* Math API */
	py::module math = m.def_submodule( "math", "MathAPI" );
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	math.def( "PrintValue", &Sys_PrintValue );
	math.def( "PrintMapCoord", &Sys_PrintMapCoord );
	math.def( "PrintAxis", &Sys_PrintAxis );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	math.def( "SnapVertex", &Sys_SnapVertex );
	math.def( "SnapAxis", &Sys_SnapAxis );
	math.def( "SnapVertexToGrid", &Sys_SnapVertexToGrid );
	math.def( "SnapMapVertex", &Sys_SnapMapVertex );

	misc.def( "VersionString", &V_VersionString ); // Will be a part of MiscAPI
	//m.def( "GetTextureGamma", &Sys_GetTextureGamma ); // Moved to the RenderingAPI
	m.def( "GetCurrentWorld", &Global_GetCurrentWorld, py::return_value_policy::reference );
	misc.def( "BuildPackageList", &BuildPackageList ); // Will be a part of MiscAPI

	/* Rendering API */
	py::module rendering = m.def_submodule( "rendering", "RenderingAPI" );
	rendering.def( "PointSize", &PR_PointSize );
	rendering.def( "LineWidth", &PR_LineWidth );
	rendering.def( "BindShader", &PR_BindShader );
	rendering.def( "BindTexture", &PR_BindTexture );
	rendering.def( "Begin", &PR_Begin );
	rendering.def( "End", &PR_End );
	rendering.def( "Color4ub", &PR_Color4ub );
	rendering.def( "Color4ubv", &PR_Color4ubv );
	rendering.def( "TexCoord2f", &PR_TexCoord2f );
	rendering.def( "TexCoord2fv", &PR_TexCoord2fv );
	rendering.def( "Normal3fv", &PR_Normal3fv );
	rendering.def( "Vertex3fv", &PR_Vertex3fv );
	rendering.def( "GetState", &PR_GetState );
	rendering.def( "SetState", &PR_SetState );
	rendering.def( "GetViewInfo", &PR_GetViewInfo );
	rendering.def( "GetMinAlpha", &PR_GetMinAlpha );
	rendering.def( "CalcLighting", &PR_CalcLighting );
	rendering.def( "GetTextureGamma", &Sys_GetTextureGamma );

	// ParserAPI

	/* Entity API */
	py::module entity = m.def_submodule( "entity", "EntityAPI" );
	entity.def( "Create", &Entity_Create, py::return_value_policy::reference );
	entity.def( "Build", &Entity_Build );
	entity.def( "SetColor", &Entity_SetColor );
	entity.def( "GetColor", &Entity_GetColor );
	entity.def( "AddToVisGroup", &Entity_AddToVisGroup );
	entity.def( "RemoveFromVisGroup", &Entity_RemoveFromVisGroup );
	entity.def( "GetVisGroupCount", &Entity_GetVisGroupCount );
	entity.def( "GetVisGroupIdent", &Entity_GetVisGroupIdent );
	entity.def( "FindByClassname", &Entity_FindByClassname, py::return_value_policy::reference );
	entity.def( "FindByTargetname", &Entity_FindByTargetname, py::return_value_policy::reference );
	entity.def( "FindByKeyValue", &Entity_FindByKeyValue, py::return_value_policy::reference );

	/* Brush API */
	py::module brush = m.def_submodule( "brush", "BrushAPI" );
	brush.def( "Create", &Brush_Create, py::return_value_policy::reference );
	brush.def( "Destroy", &Brush_Destroy );
	brush.def( "SetColor", &Brush_SetColor );
	brush.def( "GetColor", &Brush_GetColor );
	brush.def( "AddToVisGroup", &Brush_AddToVisGroup );
	brush.def( "RemoveFromVisGroup", &Brush_RemoveFromVisGroup );
	brush.def( "GetVisGroupCount", &Brush_GetVisGroupCount );
	brush.def( "GetVisGroupIdent", &Brush_GetVisGroupIdent );

	/* Face API */
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
