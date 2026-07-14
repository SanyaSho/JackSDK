//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
// 
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINAPI_H )
#define PLUGINAPI_H

/*
 Plugin API is used to interact with the editor without having to manually find symbols in the stack.
 This API is pretty much based on APIProxy/eiface from GoldSrc.

 Example:
 plugin_funcs_t gEditorfuncs;

 DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
 {
 	if ( editorFuncs->m_intefaceVersion < sizeof( plugin_funcs_t ) )
 		return -1;

 	if ( editorPluginVersion != PLUGIN_VERSION )
 		return PLUGIN_VERSION;

 	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->m_intefaceVersion );
 	setlocale( LC_ALL, "C" );
 	return 0;
 }

 editorFuncs is the pointer to a table of shared functions inside the editor which must be copied and used within the plugin.
 editorFuncs->m_intefaceVersion is set to size of plugin_funcs_t in the editor and must be checked to be sure that we can load this plugin safely.
 editorPluginVersion is set to current API level in the editor.
*/

// 1 << 0
#define FL_SELECTED					( 1 << 1 ) /* Object is selected. This flag cannot be set manually when creating/building entities */
#define FL_DIRTY					( 1 << 2 ) /* Object was modified */
#define FL_HIDDEN					( 1 << 3 ) /* Object is hidden */
#define FL_TRANSPARENT				( 1 << 4 ) /* Entity is transparent */
#define FL_WORLDSPAWN				( 1 << 5 ) /* A worldspawn */
#define FL_INVENTORYITEM			( 1 << 6 ) /* Entity "item_*" / "Item*" or "weapon_*" / "Weapon*" or fgd entity with 0x20000 (CMapEntity::changeClass) */
#define FL_PATHWAY					( 1 << 7 ) /* Entity "path_*" / "*Path*" or an fgd entity with 0x40000 (CMapEntity::changeClass) */
// 1 << 8
#define FL_IGNORE					( 1 << 9 ) /* "Ignore" flag (Brushes and Paths only) */
// 1 << 20
// 1 << 21
// 1 << 23

/*
EDITORFLAGS:

1<<0  - ?
1<<1  - Selected
1<<2  - Dirty
1<<3  - Hidden
1<<4  - Entity is transparent
1<<5  - A worldspawn
1<<6  - Entity "item_*" / "Item*" or "weapon_*" / "Weapon*" or fgd entity with 0x20000 (CMapEntity::changeClass)
1<<7  - Entity "path_*" / "*Path*" or an fgd entity with 0x40000 (CMapEntity::changeClass)
1<<8  - ?
1<<9  - "Ignore" flag (Brushes and Paths only)
1<<20 - Used by ".map" serializer in vpHalfLife
1<<21 - Used by ".map" serializer in vpHalfLife
1<<23 - Used by map serializer and map parser inside some of the plugins. Disables some Sys_Printf calls if set
*/

#include "BaseTypes.h"

struct viewInfo_s;
struct qWorld_s;
struct qEntity_s;
struct qBrush_s;
struct qFace_s;
struct qPatch_s;
struct qOverlay_s;
struct qPath_s;
struct qNode_s;
struct qGroup_s;
struct qCamera_s;
struct qTexDef_s;
struct qTexture_s;
struct qShader_s;
struct qShaderStage_s;

// clang-format off

/* Misc API */

/* String must not contain an ending newline */
typedef void		(*pfnEditor_Sys_Printf)					( const char *format, ... );

/* String must not contain an ending newline */
typedef void		(*pfnEditor_Sys_DPrintf)				( const char *format, ... );

/* String must not contain an ending newline */
typedef void		(*pfnEditor_Sys_Warning)				( const char *format, ... );

/* String must not contain an ending newline */
typedef void		(*pfnEditor_Sys_Error)					( const char *format, ... );

typedef void		(*pfnEditor_Sys_Free)					( void *ptr );
typedef void *		(*pfnEditor_Sys_Malloc)					( size_t size );

typedef void *		(*pfnEditor_TempBuffer_GetSpace)		( int buffer, size_t size );
typedef char *		(*pfnEditor_Sys_AllocString)			( const char *src ); // MUST BE Sys_Free'D

typedef float		(*pfnEditor_Sys_FloatTime)				();

#define SYS_OPTION_SNAPTOGRID		0
#define SYS_OPTION_SHOWGRID			1
#define SYS_OPTION_SHOW3DGRID		2
#define SYS_OPTION_GRIDSPACING		3
#define SYS_OPTION_IGNOREGROUPING	4
#define SYS_OPTION_TOOLSCORDON		5

typedef void		(*pfnEditor_Sys_SetOption)				( int option, int value );
typedef long		(*pfnEditor_Sys_GetOption)				( int option );

typedef void		(*pfnEditor_Steam_SetAchievemnt)		( int achIdx );

/* Parser API */
typedef char *		(*pfnEditor_SC_Token)					();
typedef long		(*pfnEditor_SC_Line)					();
typedef bool		(*pfnEditor_SC_ParseFromFile)			( const char *file, int offset, int size, int parseFlags );
typedef bool		(*pfnEditor_SC_ParseFromMemory)			( const char *file, int offset, int size );
typedef bool		(*pfnEditor_SC_CheckError)				();
typedef void		(*pfnEditor_SC_ParseError)				( const char *format, ... );
typedef void		(*pfnEditor_SC_ResetError)				();
typedef bool		(*pfnEditor_SC_SafeGetToken)			( bool nextLine );
typedef bool		(*pfnEditor_SC_GetToken)				( bool nextLine );
typedef bool		(*pfnEditor_SC_TokenAvailable)			();
typedef void		(*pfnEditor_SC_UnGetToken)				();
typedef void		(*pfnEditor_SC_MatchToken)				( const char *token );
typedef void		(*pfnEditor_SC_SafeMatchToken)			( const char *token, bool nextLine );
typedef void		(*pfnEditor_SC_Parse3DMatrix)			( int depth, int rows, int columns, float *rgflMatrix );
typedef void		(*pfnEditor_SC_Parse2DMatrix)			( int rows, int columns, float *rgflMatrix );
typedef void		(*pfnEditor_SC_Parse1DMatrix)			( int columns, float *rgflMatrix );
typedef bool		(*pfnEditor_SC_SkipRestOfLine)			();
typedef void		(*pfnEditor_SC_EndOfParsing)			();
typedef int			(*pfnEditor_SC_GetParseFlags)			();
typedef void		(*pfnEditor_SC_SetParseFlags)			( int parseFlags );
typedef bool		(*pfnEditor_SC_ShouldQuote)				( const char *token );
typedef char *		(*pfnEditor_SC_CopyBlock)				(); // MUST BE Sys_Free'D
typedef void		(*pfnEditor_SC_SkipBlock)				();
typedef void		(*pfnEditor_SC_SkipLineOrBlock)			();
typedef long		(*pfnEditor_SC_GetBlockSize)			();

typedef struct
{
	pfnEditor_SC_Token pfnSC_Token;
	pfnEditor_SC_Line pfnSC_Line;
	pfnEditor_SC_ParseFromFile pfnSC_ParseFromFile;
	pfnEditor_SC_ParseFromMemory pfnSC_ParseFromMemory;
	pfnEditor_SC_CheckError pfnSC_CheckError;
	pfnEditor_SC_ParseError pfnSC_ParseError;
	pfnEditor_SC_ResetError pfnSC_ResetError;
	pfnEditor_SC_SafeGetToken pfnSC_SafeGetToken;
	pfnEditor_SC_GetToken pfnSC_GetToken;
	pfnEditor_SC_TokenAvailable pfnSC_TokenAvailable;
	pfnEditor_SC_UnGetToken pfnSC_UnGetToken;
	pfnEditor_SC_MatchToken pfnSC_MatchToken;
	pfnEditor_SC_SafeMatchToken pfnSC_SafeMatchToken;
	pfnEditor_SC_Parse3DMatrix pfnSC_Parse3DMatrix;
	pfnEditor_SC_Parse2DMatrix pfnSC_Parse2DMatrix;
	pfnEditor_SC_Parse1DMatrix pfnSC_Parse1DMatrix;
	pfnEditor_SC_SkipRestOfLine pfnSC_SkipRestOfLine;
	pfnEditor_SC_EndOfParsing pfnSC_EndOfParsing;
	pfnEditor_SC_GetParseFlags pfnSC_GetParseFlags;
	pfnEditor_SC_SetParseFlags pfnSC_SetParseFlags;
	pfnEditor_SC_ShouldQuote pfnSC_ShouldQuote;
	pfnEditor_SC_CopyBlock pfnSC_CopyBlock;
	pfnEditor_SC_SkipBlock pfnSC_SkipBlock;
	pfnEditor_SC_SkipLineOrBlock pfnSC_SkipLineOrBlock;
	pfnEditor_SC_GetBlockSize pfnSC_GetBlockSize;
} parser_api_t;

/* Rendering API */
typedef enum
{
	PRIMTYPE_POINTS = 0,		/* GL_POINTS */
	PRIMTYPE_LINES,				/* GL_LINES */
	PRIMTYPE_TRIANGLES,			/* GL_TRIANGLES */
	PRIMTYPE_POLYGON,			/* GL_POLYGON */
	PRIMTYPE_TRIANGLE_STRIP,	/* GL_TRIANGLE_STRIP */
	PRIMTYPE_TRIANGLE_FAN,		/* GL_TRIANGLE_FAN */
} primType_e;

typedef void		(*pfnEditor_PR_BindTexture)				( qTexture_s *textureDef );
typedef void		(*pfnEditor_PR_BindShader)				( qShader_s *shaderDef );
typedef void		(*pfnEditor_PR_LineWidth)				( float width );
typedef void		(*pfnEditor_PR_PointSize)				( float size );
typedef void		(*pfnEditor_PR_Begin)					( primType_e primType );
typedef void		(*pfnEditor_PR_Color4ub)				( byte r, byte g, byte b, byte a );
typedef void		(*pfnEditor_PR_Color4ubv)				( const byte *cbColor );
typedef void		(*pfnEditor_PR_Normal3fv)				( const float *rgflNormal );
typedef void		(*pfnEditor_PR_TexCoord2f)				( float x, float y );
typedef void		(*pfnEditor_PR_TexCoord2fv)				( const float *rgflTexCoord );
typedef void		(*pfnEditor_PR_Vertex3fv)				( const float *rgflVertex );
typedef void		(*pfnEditor_PR_End)						();
typedef unsigned int(*pfnEditor_PR_GetState)				();
typedef void		(*pfnEditor_PR_SetState)				( unsigned int glState );
typedef void		(*pfnEditor_PR_GetViewInfo)				( viewInfo_s *viewInfo );
typedef void		(*pfnEditor_PR_CalcLighting)			( const float *rgfl ); // ???
typedef float		(*pfnEditor_PR_GetMinAlpha)				();

typedef struct
{
	pfnEditor_PR_BindTexture pfnPR_BindTexture;
	pfnEditor_PR_BindShader pfnPR_BindShader;
	pfnEditor_PR_LineWidth pfnPR_LineWidth;
	pfnEditor_PR_PointSize pfnPR_PointSize;
	pfnEditor_PR_Begin pfnPR_Begin;
	pfnEditor_PR_Color4ub pfnPR_Color4ub;
	pfnEditor_PR_Color4ubv pfnPR_Color4ubv;
	pfnEditor_PR_Normal3fv pfnPR_Normal3fv;
	pfnEditor_PR_TexCoord2f pfnPR_TexCoord2f;
	pfnEditor_PR_TexCoord2fv pfnPR_TexCoord2fv;
	pfnEditor_PR_Vertex3fv pfnPR_Vertex3fv;
	pfnEditor_PR_End pfnPR_End;
	pfnEditor_PR_GetState pfnPR_GetState;
	pfnEditor_PR_SetState pfnPR_SetState;
	pfnEditor_PR_GetViewInfo pfnPR_GetViewInfo;
	pfnEditor_PR_CalcLighting pfnPR_CalcLighting;
	pfnEditor_PR_GetMinAlpha pfnPR_GetMinAlpha;
} rendering_api_t;

/* FileSystem API */

/* Get current configuration base directory */
/* Returns true on success, false on failure */
typedef bool		(*pfnEditor_Sys_GetBaseDirectory)		( char *dest, size_t n );

/* Get current configuration mod directory */
/* Mimics Sys_GetBaseDirectory if not set. */
/* Returns true on success, false on failure */
typedef bool		(*pfnEditor_Sys_GetModDirectory)		( char *dest, size_t n );

/* Get current configuration fallback directory */
/* Returns true on success, false on failure */
typedef bool		(*pfnEditor_Sys_GetFallbackDirectory)	( char *dest, size_t n );

typedef void		(*pfnEditor_Sys_ExpandFileName)			( const char *src, char *dest, size_t n );
typedef char *		(*pfnEditor_Sys_MakeLocalFileName)		( const char *filePath );
typedef bool		(*pfnEditor_Sys_FileExists)				( const char *filePath );
typedef byte *		(*pfnEditor_Sys_LoadFile)				( const char *filePath, int *readBytes );
typedef bool		(*pfnEditor_Sys_CreatePath)				( const char *path );

typedef struct
{
	pfnEditor_Sys_GetBaseDirectory pfnSys_GetBaseDirectory;
	pfnEditor_Sys_GetModDirectory pfnSys_GetModDirectory;
	pfnEditor_Sys_GetFallbackDirectory pfnSys_GetFallbackDirectory;
	pfnEditor_Sys_ExpandFileName pfnSys_ExpandFileName;
	pfnEditor_Sys_MakeLocalFileName pfnSys_MakeLocalFileName;
	pfnEditor_Sys_FileExists pfnSys_FileExists;
	pfnEditor_Sys_LoadFile pfnSys_LoadFile;
	pfnEditor_Sys_CreatePath pfnSys_CreatePath;
} filesystem_api_t;

/* Math API */
typedef char *		(*pfnEditor_Sys_PrintValue)				( float value );
typedef char *		(*pfnEditor_Sys_PrintMapCoord)			( float coord );
typedef char *		(*pfnEditor_Sys_PrintAxis)				( float axis );
typedef void		(*pfnEditor_Sys_SnapVertex)				( float *rgflVertex );
typedef void		(*pfnEditor_Sys_SnapAxis)				( int num, float *rgflAxis );
typedef void		(*pfnEditor_Sys_SnapVertexToGrid)		( float *rgflVertex );
typedef void		(*pfnEditor_Sys_SnapMapVertex)			( float *rgflVertex );

typedef struct
{
	pfnEditor_Sys_PrintValue pfnSys_PrintValue;
	pfnEditor_Sys_PrintMapCoord pfnSys_PrintMapCoord;
	pfnEditor_Sys_PrintAxis pfnSys_PrintAxis;
	pfnEditor_Sys_SnapVertex pfnSys_SnapVertex;
	pfnEditor_Sys_SnapAxis pfnSys_SnapAxis;
	pfnEditor_Sys_SnapVertexToGrid pfnSys_SnapVertexToGrid;
	pfnEditor_Sys_SnapMapVertex pfnSys_SnapMapVertex;
} math_api_t;

/* Returns the current version string (J.A.C.K. 1.2.4603) */
typedef char *		(*pfnEditor_V_VersionString)			();

typedef float		(*pfnEditor_Sys_GetTextureGamma)		();

typedef qWorld_s *	(*pfnEditor_Global_GetCurrentWorld)		();

/* outBuf is not an array. It's an allocated string of WADs used on this map separated by listSeparator and it must be free'd */
/* removeVolumePrefix does not work on Linux */
typedef bool		(*pfnEditor_BuildPackageList)			( qWorld_s *worldDef, char **outBuf, char listSeparator, int removeVolumePrefix );

/* Entity API */
typedef qEntity_s *	(*pfnEditor_Entity_Create)				( qWorld_s *worldDef, const char *classname, const float *rgflOrigin, int editorFlags );
typedef void		(*pfnEditor_Entity_Build)				( qEntity_s *entityDef, int entityBuildFlags );
typedef void		(*pfnEditor_Entity_GetColor)			( qEntity_s *entityDef, byte *cbColorOut );
typedef void		(*pfnEditor_Entity_SetColor)			( qEntity_s *entityDef, const byte *cbColor );
typedef void		(*pfnEditor_Entity_AddToVisGroup)		( qWorld_s *worldDef, qEntity_s *entityDef, unsigned int visGroupId );
typedef void		(*pfnEditor_Entity_RemoveFromVisGroup)	( qWorld_s *worldDef, qEntity_s *entityDef, unsigned int visGroupId );
typedef long		(*pfnEditor_Entity_GetVisGroupIdent)	( qEntity_s *entityDef, int visGroupId );
typedef long		(*pfnEditor_Entity_GetVisGroupCount)	( qEntity_s *entityDef );

/* Iterates thru a list of entities (worldDef->m_entityList for example) and checks for epairs */
typedef qEntity_s *	(*pfnEditor_Entity_FindByKeyValue)		( qEntity_s *entityDef, const char *key, const char *value );

/* Iterates thru a list of (worldDef->m_entityList for example) and checks for m_className value */
typedef qEntity_s *	(*pfnEditor_Entity_FindByClassname)		( qEntity_s *entityDef, const char *classname );

/* Iterates thru a list of (worldDef->m_entityList for example) and checks for m_targetName value */
typedef qEntity_s *	(*pfnEditor_Entity_FindByTargetname)	( qEntity_s *entityDef, const char *targetname );

typedef struct
{
	pfnEditor_Entity_Create pfnEntity_Create;
	pfnEditor_Entity_Build pfnEntity_Build;
	pfnEditor_Entity_GetColor pfnEntity_GetColor;
	pfnEditor_Entity_SetColor pfnEntity_SetColor;
	pfnEditor_Entity_AddToVisGroup pfnEntity_AddToVisGroup;
	pfnEditor_Entity_RemoveFromVisGroup pfnEntity_RemoveFromVisGroup;
	pfnEditor_Entity_GetVisGroupIdent pfnEntity_GetVisGroupIdent;
	pfnEditor_Entity_GetVisGroupCount pfnEntity_GetVisGroupCount;
	pfnEditor_Entity_FindByKeyValue pfnEntity_FindByKeyValue;
	pfnEditor_Entity_FindByClassname pfnEntity_FindByClassname;
	pfnEditor_Entity_FindByTargetname pfnEntity_FindByTargetname;
} entity_api_t;

/* Brush API */
typedef qBrush_s *	(*pfnEditor_Brush_Create)				( qWorld_s *worldDef, qEntity_s *entityDef );
typedef void		(*pfnEditor_Brush_Destroy)				( qWorld_s *worldDef, qBrush_s *brushDef );
typedef void		(*pfnEditor_Brush_GetColor)				( qBrush_s *brushDef, byte *cbColorOut );
typedef void		(*pfnEditor_Brush_SetColor)				( qBrush_s *brushDef, const byte *cbColor );
typedef void		(*pfnEditor_Brush_AddToVisGroup)		( qWorld_s *worldDef, qBrush_s *brushDef, unsigned int visGroupId );
typedef void		(*pfnEditor_Brush_RemoveFromVisGroup)	( qWorld_s *worldDef, qBrush_s *brushDef, unsigned int visGroupId );
typedef long		(*pfnEditor_Brush_GetVisGroupIdent)		( qBrush_s *brushDef, int visGroupId );
typedef long		(*pfnEditor_Brush_GetVisGroupCount)		( qBrush_s *brushDef );

typedef struct
{
	pfnEditor_Brush_Create pfnBrush_Create;
	pfnEditor_Brush_Destroy pfnBrush_Destroy;
	pfnEditor_Brush_GetColor pfnBrush_GetColor;
	pfnEditor_Brush_SetColor pfnBrush_SetColor;
	pfnEditor_Brush_AddToVisGroup pfnBrush_AddToVisGroup;
	pfnEditor_Brush_RemoveFromVisGroup pfnBrush_RemoveFromVisGroup;
	pfnEditor_Brush_GetVisGroupIdent pfnBrush_GetVisGroupIdent;
	pfnEditor_Brush_GetVisGroupCount pfnBrush_GetVisGroupCount;
} brush_api_t;

/* Face API */
typedef qFace_s *	(*pfnEditor_Face_Create)				( qWorld_s *worldDef, qBrush_s *brushDef, const qTexDef_s *texDef, int vertexCount );
typedef void		(*pfnEditor_Face_Destroy)				( qWorld_s *worldDef, qFace_s *faceDef );

typedef struct
{
	pfnEditor_Face_Create pfnFace_Create;
	pfnEditor_Face_Destroy pfnFace_Destroy;
} face_api_t;

/* Patch API */
typedef enum
{
	/*
	 Creates a 3×3 cap. The first two rows are collapsed into a single point.

	 AAA
	 AAA
	 BCD
	*/
	PATCHCAP_POINT = 0,

	/*
	 Creates a 3×3 cap. The last two rows are collapsed into the middle control point.

	 ABC
	 BBB
	 BBB
	*/
	PATCHCAP_INVERTED_POINT,

	/*
	 Creates a 3×3 cap. Computes the midpoint between the first and last control points and inserts it into the middle row.

	 ABC
	 MCC
	 DEF

	 M = (A + D) / 2
	*/
	PATCHCAP_BEVEL,

	/*
	 Creates a 5×3 rounded cap. The edge is expanded into a five-column patch.

	 A -> B -> C -> D -> E
	*/
	PATCHCAP_ROUND,

	/*
	 Creates a 3×N bridge patch, where N is half the source edge length. Opposing control points are averaged to form the center column.
	*/
	PATCHCAP_BRIDGE,

	/*
	 Same as PATCHCAP_ROUND, but only samples the first four control points from the source edge before constructing the cap.
	*/
	PATCHCAP_ROUND_SHORT
} PatchCapType_e;

typedef enum
{
	PATCHCAP_END = 0,
	PATCHCAP_START
} PatchCapLocation_e;

typedef qPatch_s *	(*pfnEditor_Patch_Create)				( qWorld_s *worldDef, const qTexDef_s *texDef, int numColumns, int numRows );
typedef qPatch_s *	(*pfnEditor_Patch_CreateCap)			( qBrush_s *brushDef, const qPatch_s *patchDef, PatchCapType_e capType, PatchCapLocation_e capLocation, int );
typedef void		(*pfnEditor_Patch_Destroy)				( qPatch_s *patchDef );
typedef void		(*pfnEditor_Patch_NaturalizeTexture)	( qPatch_s *patchDef );
typedef void		(*pfnEditor_Patch_CapTexture)			( qPatch_s *patchDef, const float *rgflCap );
typedef void		(*pfnEditor_Patch_FitTexture)			( qPatch_s *patchDef, float x, float y, int flags );
typedef void		(*pfnEditor_Patch_InterpolateExteriorPoints)( qPatch_s *patchDef );

typedef struct
{
	pfnEditor_Patch_Create pfnPatch_Create;
	pfnEditor_Patch_CreateCap pfnPatch_CreateCap;
	pfnEditor_Patch_Destroy pfnPatch_Destroy;
	pfnEditor_Patch_NaturalizeTexture pfnPatch_NaturalizeTexture;
	pfnEditor_Patch_CapTexture pfnPatch_CapTexture;
	pfnEditor_Patch_FitTexture pfnPatch_FitTexture;
	pfnEditor_Patch_InterpolateExteriorPoints pfnPatch_InterpolateExteriorPoints;
} patch_api_t;

/* Overlay API */
typedef qOverlay_s *(*pfnEditor_Overlay_Create)				( qWorld_s *worldDef, const qTexDef_s &texDef );
typedef void		(*pfnEditor_Overlay_Destroy)			( qOverlay_s *overlayDef );

typedef struct
{
	pfnEditor_Overlay_Create pfnOverlay_Create;
	pfnEditor_Overlay_Destroy pfnOverlay_Destroy;
} overlay_api_t;

/* Path API */
typedef qPath_s *	(*pfnEditor_Path_Create)				( qWorld_s *worldDef );
typedef void		(*pfnEditor_Path_Destroy)				( qPath_s *path );
typedef void		(*pfnEditor_Path_Build)					( qPath_s *path, int );

typedef struct
{
	pfnEditor_Path_Create pfnPath_Create;
	pfnEditor_Path_Destroy pfnPath_Destroy;
	pfnEditor_Path_Build pfnPath_Build;
} path_api_t;

/* Node API */
typedef qNode_s *	(*pfnEditor_Node_Insert)				( qWorld_s *worldDef, qNode_s *parentNode );
typedef void		(*pfnEditor_Node_Append)				( qWorld_s *worldDef, qPath_s *path );
typedef void		(*pfnEditor_Node_Destroy)				( qNode_s *nodeDef );

typedef struct
{
	pfnEditor_Node_Insert pfnNode_Insert;
	pfnEditor_Node_Append pfnNode_Append;
	pfnEditor_Node_Destroy pfnNode_Destroy;
} node_api_t;

/* Group API */
typedef qGroup_s *	(*pfnEditor_Group_Create)				( qWorld_s *worldDef );
typedef void		(*pfnEditor_Group_Destroy)				( qWorld_s *worldDef, qGroup_s *groupDef );
typedef void		(*pfnEditor_Group_AddGroup)				( qGroup_s *rootGroupDef, qGroup_s *groupDef );
typedef void		(*pfnEditor_Group_AddBrush)				( qGroup_s *rootGroupDef, qBrush_s *brushDef );
typedef void		(*pfnEditor_Group_AddEntity)			( qGroup_s *rootGroupDef, qEntity_s *entityDef );
typedef void		(*pfnEditor_Group_GetColor)				( qGroup_s *groupDef, byte *cbColorOut );
typedef void		(*pfnEditor_Group_SetColor)				( qGroup_s *groupDef, const byte *cbColor );

typedef struct
{
	pfnEditor_Group_Create pfnGroup_Create;
	pfnEditor_Group_Destroy pfnGroup_Destroy;
	pfnEditor_Group_AddGroup pfnGroup_AddGroup;
	pfnEditor_Group_AddBrush pfnGroup_AddBrush;
	pfnEditor_Group_AddEntity pfnGroup_AddEntity;
	pfnEditor_Group_GetColor pfnGroup_GetColor;
	pfnEditor_Group_SetColor pfnGroup_SetColor;
} group_api_t;

/* Camera API*/
typedef qCamera_s *	(*pfnEditor_Camera_Create)				( qWorld_s *worldDef );
typedef void		(*pfnEditor_Camera_Destroy)				( qCamera_s *camera );
typedef void		(*pfnEditor_Camera_GetColor)			( qCamera_s *camera, byte *cbColorOut );
typedef void		(*pfnEditor_Camera_SetColor)			( qCamera_s *camera, const byte *cbColor );
typedef void		(*pfnEditor_Camera_Setup)				( qCamera_s *camera, const float *rgflOrigin, const float *rgflAngles );

typedef struct
{
	pfnEditor_Camera_Create pfnCamera_Create;
	pfnEditor_Camera_Destroy pfnCamera_Destroy;
	pfnEditor_Camera_GetColor pfnCamera_GetColor;
	pfnEditor_Camera_SetColor pfnCamera_SetColor;
	pfnEditor_Camera_Setup pfnCamera_Setup;
} camera_api_t;

/* Shader API */
typedef qShader_s *	(*pfnEditor_Shader_Create)				( const char *shaderName, const char *textureName, int shaderFlags );
typedef qShader_s *	(*pfnEditor_Shader_Lookup)				( const char *shaderName );
typedef void		(*pfnEditor_Shader_Destroy)				( qShader_s *shaderHandle );
typedef void		(*pfnEditor_Shader_AddStage)			( qShader_s *shaderHandle, qShaderStage_s *shaderStage );
typedef void		(*pfnEditor_Shader_RemoveStage)			( qShaderStage_s *shaderStage );
typedef void		(*pfnEditor_Shader_Finish)				( qShader_s *shaderHandle );
typedef qTexture_s *(*pfnEditor_Shader_GetWhiteTexture)		();
typedef qTexture_s *(*pfnEditor_Shader_GetBlackTexture)		();
typedef qTexture_s *(*pfnEditor_Shader_LookupTexture)		( const char *textureName );
typedef qTexture_s *(*pfnEditor_Shader_UploadTexture)		( qShader_s *shaderHandle, const char *textureName, unsigned int pixelFormat, unsigned int textureFormat, int textureNumChannels, int textureWidth, int textureHeight, bool, byte *textureData );
typedef void		(*pfnEditor_Shader_DestroyTexture)		( qTexture_s *textureHandle );

typedef struct
{
	pfnEditor_Shader_Create pfnShader_Create;
	pfnEditor_Shader_Lookup pfnShader_Lookup;
	pfnEditor_Shader_Destroy pfnShader_Destroy;
	pfnEditor_Shader_AddStage pfnShader_AddStage;
	pfnEditor_Shader_RemoveStage pfnShader_RemoveStage;
	pfnEditor_Shader_Finish pfnShader_Finish;
	pfnEditor_Shader_GetWhiteTexture pfnShader_GetWhiteTexture;
	pfnEditor_Shader_GetBlackTexture pfnShader_GetBlackTexture;
	pfnEditor_Shader_LookupTexture pfnShader_LookupTexture;
	pfnEditor_Shader_UploadTexture pfnShader_UploadTexture;
	pfnEditor_Shader_DestroyTexture pfnShader_DestroyTexture;
} shader_api_t;

/* VisGroup API */
typedef unsigned int(*pfnEditor_VisGroup_Add)				( const qWorld_s *worldDef );
typedef void		(*pfnEditor_VisGroup_Remove)			( const qWorld_s *worldDef, unsigned int visGroupIndex );
typedef void		(*pfnEditor_VisGroup_Modify)			( const qWorld_s *worldDef, unsigned int visGroupIndex, const char *visGroupName, unsigned int alterVisGroupIndex, const byte *cbColor );
typedef int			(*pfnEditor_VisGroup_GetCount)			( const qWorld_s *worldDef );
typedef int			(*pfnEditor_VisGroup_GetIndex)			( const qWorld_s *worldDef, unsigned int visGroupIndex );
typedef void		(*pfnEditor_VisGroup_GetData)			( const qWorld_s *worldDef, int visGroupIndex, char *visGroupNameOut, int visGroupNameOutSize, unsigned int *, int *, byte *cbColorOut );
typedef void		(*pfnEditor_VisGroup_RebuildIdents)		( const qWorld_s *worldDef );

typedef struct
{
	pfnEditor_VisGroup_Add pfnVisGroup_Add;
	pfnEditor_VisGroup_Remove pfnVisGroup_Remove;
	pfnEditor_VisGroup_Modify pfnVisGroup_Modify;
	pfnEditor_VisGroup_GetCount pfnVisGroup_GetCount;
	pfnEditor_VisGroup_GetIndex pfnVisGroup_GetIndex;
	pfnEditor_VisGroup_GetData pfnVisGroup_GetData;
	pfnEditor_VisGroup_RebuildIdents pfnVisGroup_RebuildIdents;
} visgroup_api_t;

/* Undo API */
typedef void		(*pfnEditor_Undo_Start)					( const qWorld_s *worldDef, const char * );
typedef void		(*pfnEditor_Undo_End)					( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_AddGroup)				( qGroup_s *groupDef );
typedef void		(*pfnEditor_Undo_AddBrush)				( qBrush_s *brushDef );
typedef void		(*pfnEditor_Undo_AddPath)				( qPath_s *pathDef );
typedef void		(*pfnEditor_Undo_AddNode)				( qNode_s *nodeDef );
typedef void		(*pfnEditor_Undo_AddEntity)				( qEntity_s *entityDef );
typedef void		(*pfnEditor_Undo_DeleteGroup)			( qGroup_s *groupDef );
typedef void		(*pfnEditor_Undo_DeleteBrush)			( qBrush_s *brushDef );
typedef void		(*pfnEditor_Undo_DeletePath)			( qPath_s *pathDef );
typedef void		(*pfnEditor_Undo_DeleteNode)			( qNode_s *nodeDef );
typedef void		(*pfnEditor_Undo_DeleteEntity)			( qEntity_s *entityDef );
typedef void		(*pfnEditor_Undo_StoreFace)				( qFace_s *faceDef );
typedef void		(*pfnEditor_Undo_StoreBrush)			( qBrush_s *brushDef );
typedef void		(*pfnEditor_Undo_StorePath)				( qPath_s *pathDef );
typedef void		(*pfnEditor_Undo_StoreNode)				( qNode_s *nodeDef );
typedef void		(*pfnEditor_Undo_StoreEntity)			( qEntity_s *entityDef );
typedef void		(*pfnEditor_Undo_AddSelectedEntities)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_AddSelectedBrushes)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_AddSelectedNodes)		( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_DeleteSelectedEntities)( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_DeleteSelectedBrushes)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_StoreSelectedEntities)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_DeleteSelectedNodes)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_StoreSelectedBrushes)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_StoreSelectedNodes)	( const qWorld_s *worldDef );
typedef void		(*pfnEditor_Undo_StoreSelectedFaces)	( const qWorld_s *worldDef );

typedef struct
{
	pfnEditor_Undo_Start pfnUndo_Start;
	pfnEditor_Undo_End pfnUndo_End;
	pfnEditor_Undo_AddGroup pfnUndo_AddGroup;
	pfnEditor_Undo_AddBrush pfnUndo_AddBrush;
	pfnEditor_Undo_AddPath pfnUndo_AddPath;
	pfnEditor_Undo_AddNode pfnUndo_AddNode;
	pfnEditor_Undo_AddEntity pfnUndo_AddEntity;
	pfnEditor_Undo_DeleteGroup pfnUndo_DeleteGroup;
	pfnEditor_Undo_DeleteBrush pfnUndo_DeleteBrush;
	pfnEditor_Undo_DeletePath pfnUndo_DeletePath;
	pfnEditor_Undo_DeleteNode pfnUndo_DeleteNode;
	pfnEditor_Undo_DeleteEntity pfnUndo_DeleteEntity;
	pfnEditor_Undo_StoreFace pfnUndo_StoreFace;
	pfnEditor_Undo_StoreBrush pfnUndo_StoreBrush;
	pfnEditor_Undo_StorePath pfnUndo_StorePath;
	pfnEditor_Undo_StoreNode pfnUndo_StoreNode;
	pfnEditor_Undo_StoreEntity pfnUndo_StoreEntity;
	pfnEditor_Undo_AddSelectedEntities pfnUndo_AddSelectedEntities;
	pfnEditor_Undo_AddSelectedBrushes pfnUndo_AddSelectedBrushes;
	pfnEditor_Undo_AddSelectedNodes pfnUndo_AddSelectedNodes;
	pfnEditor_Undo_DeleteSelectedEntities pfnUndo_DeleteSelectedEntities;
	pfnEditor_Undo_DeleteSelectedBrushes pfnUndo_DeleteSelectedBrushes;
	pfnEditor_Undo_StoreSelectedEntities pfnUndo_StoreSelectedEntities;
	pfnEditor_Undo_DeleteSelectedNodes pfnUndo_DeleteSelectedNodes;
	pfnEditor_Undo_StoreSelectedBrushes pfnUndo_StoreSelectedBrushes;
	pfnEditor_Undo_StoreSelectedNodes pfnUndo_StoreSelectedNodes;
	pfnEditor_Undo_StoreSelectedFaces pfnUndo_StoreSelectedFaces;
} undo_api_t;

/* Dialog API */

#define DIALOG_MB_OKCANCEL			( 1 << 0 )
#define DIALOG_MB_YESNO				( 1 << 1 )
#define DIALOG_MB_ICONERROR			( 1 << 2 )
#define DIALOG_MB_ICONWARNING		( 1 << 3 )
#define DIALOG_MB_ICONINFORMATION	( 1 << 4 )
#define DIALOG_MB_ICONQUESTION		( 1 << 5 )

#define DIALOG_FILE_OPEN			( 1 << 1 ) /* Will make a dialog open files instead of saving them */
#define DIALOG_FILE_CONNECT			( 1 << 2 ) /* Will connect internal Qt signals to the panel when used with DIALOG_FILE_OPEN */

/* Check for custom options */
/* title: dialog title */
/* text: dialog text */
/* options: list of options split by | */
/* returns a bitmask for each option selected OR returns -1 if was closed */
typedef long		(*pfnEditor_Dialog_CheckOptions)		( const char *title, const char *text, const char *options );

/* Display a message box */
/* title: dialog title */
/* text: dialog text */
/* flags: type and behavior flags */
/* If the dialog was created without DIALOG_MB_OKCANCEL or DIALOG_MB_YESNO flags it will always return true upon closing */
typedef bool		(*pfnEditor_Dialog_MessageBox)			( const char *title, const char *text, int flags );

/* Allocates a CPluginDialog instance (NOTE: Only one plugin dialog can be active) */
/* title: dialog title */
typedef void		(*pfnEditor_Dialog_Begin)				( const char *title );

/* buttonText: text that will be displayed on the button */
/* command: command that will be executed after button is pressed */
typedef void		(*pfnEditor_Dialog_InitExternalCommand)	( const char *buttonText, const char *command );

/* buttonText: text that will be displayed on the button */
/* pfnCommand: function that will be executed after button is pressed */
typedef void		(*pfnEditor_Dialog_InitInternalCommand)	( const char *buttonText, void (*pfnCommand)() );

/* value: progress bar current value */
/* maxValue: progress bar max value */
typedef void		(*pfnEditor_Dialog_SetProgress)			( int value, int maxValue );

/* Add a textedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddTextEdit)			( const char *controlName, const char *title, const char *defaultValue, int flags );

/* Add a radiobox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial state (true - checked; false - unchecked) */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddRadioBox)			( const char *controlName, const char *title, bool defaultValue, int flags );

/* Add a checkbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial state (true - checked; false - unchecked) */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddCheckBox)			( const char *controlName, const char *title, bool defaultValue, int flags );

/* Add a spinbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial value */
/* minValue: min value */
/* maxValue: max value */
/* stepCount: number of elements one step goes thru */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddSpinBox)			( const char *controlName, const char *title, int defaultValue, int minValue, int maxValue, int stepCount, int flags );

/* Add a double spinbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial value */
/* minValue: min value */
/* maxValue: max value */
/* stepCount: number of elements one step goes thru */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddSpinBoxFloat)		( const char *controlName, const char *title, float defaultValue, float minValue, float maxValue, float stepCount, int flags );

/* Add a fileedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* extensionList: list of possible extensions split by "\n". Can be NULL */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddFileEdit)			( const char *controlName, const char *title, const char *defaultValue, const char *extensionList, int flags );

/* Add a filelist control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* fileList: list of strings split by "\n" */
/* extensionList: list of possible extensions split by "\n". Can be NULL */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddFileList)			( const char *controlName, const char *title, const char *fileList, const char *extensionList, int flags );

/* Add a combobox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* selectedIndex: will set the selected item to this index after parsing the list from optionsList */
/* optionsList: list of elements split by "\n" in format NAME\nVALUE (OPT1\n1\nOPT2\n2\nOPT3\n3) */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddComboBox)			( const char *controlName, const char *title, int selectedIndex, const char *optionsList, int flags );

/* Add a directoryedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddDirectoryEdit)	( const char *controlName, const char *title, const char *defaultValue, int flags );

/* Get control value (string) */
/* controlName: internal control name */
/* out: char buffer. This value is already cleared and null-terminated inside the function */
/* n: size of char buffer */
typedef void		(*pfnEditor_Dialog_QueryArgument)		( const char *controlName, char *out, size_t n );

/* Get control value (float) */
/* controlName: internal control name */
/* Returns value of controlName */
typedef float		(*pfnEditor_Dialog_QueryArgumentFloat)	( const char *controlName );

/* Get control value (int) */
/* controlName: internal control name */
/* Returns value of controlName */
typedef int			(*pfnEditor_Dialog_QueryArgumentInt)	( const char *controlName );

/* Finish and draw the dialog */
typedef void		(*pfnEditor_Dialog_End)					();

/* Appends text to the dialog */
/* String must not contain an ending newline */
typedef void		(*pfnEditor_Dialog_Printf)				( const char *format, ... );

/* Changes current cursor to Qt::WaitCursor and increments the wait counter by 1 */
typedef void		(*pfnEditor_Dialog_BeginWait)			();

/* Restores the cursor back to normal and decrements the wait counter by 1 */
typedef void		(*pfnEditor_Dialog_EndWait)				();

typedef struct
{
	pfnEditor_Dialog_CheckOptions pfnDialog_CheckOptions;
	pfnEditor_Dialog_MessageBox pfnDialog_MessageBox;
	pfnEditor_Dialog_Begin pfnDialog_Begin;
	pfnEditor_Dialog_InitExternalCommand pfnDialog_InitExternalCommand;
	pfnEditor_Dialog_InitInternalCommand pfnDialog_InitInternalCommand;
	pfnEditor_Dialog_SetProgress pfnDialog_SetProgress;
	pfnEditor_Dialog_AddTextEdit pfnDialog_AddTextEdit;
	pfnEditor_Dialog_AddRadioBox pfnDialog_AddRadioBox;
	pfnEditor_Dialog_AddCheckBox pfnDialog_AddCheckBox;
	pfnEditor_Dialog_AddSpinBox pfnDialog_AddSpinBox;
	pfnEditor_Dialog_AddSpinBoxFloat pfnDialog_AddSpinBoxFloat;
	pfnEditor_Dialog_AddFileEdit pfnDialog_AddFileEdit;
	pfnEditor_Dialog_AddFileList pfnDialog_AddFileList;
	pfnEditor_Dialog_AddComboBox pfnDialog_AddComboBox;
	pfnEditor_Dialog_AddDirectoryEdit pfnDialog_AddDirectoryEdit;
	pfnEditor_Dialog_QueryArgument pfnDialog_QueryArgument;
	pfnEditor_Dialog_QueryArgumentFloat pfnDialog_QueryArgumentFloat;
	pfnEditor_Dialog_QueryArgumentInt pfnDialog_QueryArgumentInt;
	pfnEditor_Dialog_End pfnDialog_End;
	pfnEditor_Dialog_Printf pfnDialog_Printf;
	pfnEditor_Dialog_BeginWait pfnDialog_BeginWait;
	pfnEditor_Dialog_EndWait pfnDialog_EndWait;
} dialog_apt_t;

// clang-format on

/* J.A.C.K. Plugin Interface */
/* m_intefaceVersion is a sizeof( plugin_funcs_t ) */
typedef struct plugin_funcs_s
{
	size_t m_intefaceVersion;

	/* Misc API */
	pfnEditor_Sys_Printf pfnSys_Printf;
	pfnEditor_Sys_DPrintf pfnSys_DPrintf;
	pfnEditor_Sys_Warning pfnSys_Warning;
	pfnEditor_Sys_Error pfnSys_Error;

	pfnEditor_Sys_Free pfnSys_Free;
	pfnEditor_Sys_Malloc pfnSys_Malloc;

	pfnEditor_TempBuffer_GetSpace pfnTempBuffer_GetSpace;
	pfnEditor_Sys_AllocString pfnSys_AllocString;

	pfnEditor_Sys_FloatTime pfnSys_FloatTime;

	pfnEditor_Sys_SetOption pfnSys_SetOption;
	pfnEditor_Sys_GetOption pfnSys_GetOption;

	pfnEditor_Steam_SetAchievemnt pfnSteam_SetAchievemnt;

	/* Parser API */
	parser_api_t parserfuncs;

	/* Rendering API */
	rendering_api_t renderingfuncs;

	/* FileSystem API */
	filesystem_api_t filesystemfuncs;

	/* Math API */
	math_api_t mathfuncs;

	pfnEditor_V_VersionString pfnV_VersionString;

	pfnEditor_Sys_GetTextureGamma pfnSys_GetTextureGamma;

	pfnEditor_Global_GetCurrentWorld pfnGlobal_GetCurrentWorld;

	pfnEditor_BuildPackageList pfnBuildPackageList;

	/* Entity API */
	entity_api_t entityfuncs;

	/* Brush API*/
	brush_api_t brushfuncs;

	/* Face API */
	face_api_t facefuncs;

	/* Patch API */
	patch_api_t patchfuncs;

	/* Overlay API */
	overlay_api_t overlayfuncs;

	/* Path API */
	path_api_t pathfuncs;

	/* Node API */
	node_api_t nodefuncs;

	/* Group API */
	group_api_t groupfuncs;

	/* Camera API */
	camera_api_t camerafuncs;

	/* Shader API */
	shader_api_t shaderfuncs;

	/* VisGroup API */
	visgroup_api_t visgroupfuncs;

	/* Undo API */
	undo_api_t undofuncs;

	/* Dialog API */
	dialog_apt_t dialogfuncs;
} plugin_funcs_t;
COMPILE_TIME_ASSERT( sizeof( plugin_funcs_t ) == SIZEOF_PLUGIN_FUNCS_T );

#define PLUGIN_VERSION 121

// clang-format off

typedef int (*vpMain_t)( plugin_funcs_t *editorFuncs, int editorPluginVersion );

// clang-format on

#if !defined( PLUGINEDITORFUNCTIONS_H )
#include "PluginEditorFunctions.h"
#endif // !PLUGINEDITORFUNCTIONS_H

#endif // !PLUGINAPI_H
