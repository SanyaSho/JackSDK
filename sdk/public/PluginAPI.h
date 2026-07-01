//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// 
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINAPI_H )
#define PLUGINAPI_H

#if defined( WIN32 )
#define DLL_EXPORT extern "C" __declspec( dllexport )
#define DLL_IMPORT extern "C" __declspec( dllimport )
#else
#define DLL_EXPORT extern "C" __attribute__ ((visibility("default")))
#define DLL_IMPORT extern "C" 
#endif // WIN32

// clang-format off

typedef void		(*pfnEditor_Sys_Printf)				( const char *format, ... );
typedef void		(*pfnEditor_Sys_DPrintf)			( const char *format, ... );
typedef void		(*pfnEditor_Sys_Warning)			( const char *format, ... );
typedef void		(*pfnEditor_Sys_Error)				( const char *format, ... );

typedef void		(*pfnEditor_Sys_Free)				( void *ptr );
typedef void *		(*pfnEditor_Sys_Malloc)				( size_t size );

typedef void *		(*pfnEditor_TempBuffer_GetSpace)	( int buffer, size_t size );
typedef char *		(*pfnEditor_Sys_AllocString)		( const char *src );

typedef float		(*pfnEditor_Sys_FloatTime)			();

#define SYS_OPTION_MAPSNAP		0
#define SYS_OPTION_MAPGRID		1
#define SYS_OPTION_MAPGRID3D	2
#define SYS_OPTION_GRIDSIZE		3
#define SYS_OPTION_TOOLSIG		4
#define SYS_OPTION_TOOLSCORDON	5

typedef void		(*pfnEditor_Sys_SetOption)			( int option, int value );
typedef long		(*pfnEditor_Sys_GetOption)			( int option );

// Steam_SetAchievemnt

typedef char *		(*pfnEditor_SC_Token)				();
typedef long		(*pfnEditor_SC_Line)				();
typedef bool		(*pfnEditor_SC_ParseFromFile)		( const char *file, int offset, int size, int );
typedef bool		(*pfnEditor_SC_ParseFromMemory)		( const char *file, int offset, int size );
typedef bool		(*pfnEditor_SC_CheckError)			();
typedef void		(*pfnEditor_SC_ParseError)			( const char *format, ... );
typedef void		(*pfnEditor_SC_ResetError)			();
// SC_SafeGetToken
// SC_GetToken
// SC_TokenAvailable
// SC_UnGetToken
// SC_MatchToken
// SC_SafeMatchToken
// SC_Parse3DMatrix
// SC_Parse2DMatrix
// SC_Parse1DMatrix

// PR[17]

// FileSystem API
typedef bool		(*pfnEditor_Sys_GetBaseDirectory)( char *dest, size_t n );
typedef bool		(*pfnEditor_Sys_GetModDirectory)( char *dest, size_t n );
typedef bool		(*pfnEditor_Sys_GetFallbackDirectory)( char *dest, size_t n );
// FS[2]
typedef bool		(*pfnEditor_Sys_FileExists)( const char *file );
typedef char *		(*pfnEditor_Sys_LoadFile)( const char *file, int *readBytes );
typedef bool		(*pfnEditor_Sys_CreatePath)( const char *path );

typedef char *		(*pfnEditor_Sys_PrintValue)( float value );
typedef char *		(*pfnEditor_Sys_PrintMapCoord)( float coord );
typedef char *		(*pfnEditor_Sys_PrintAxis)( float axis );
typedef void		(*pfnEditor_Sys_SnapVertex)( float *rgflVertex );
typedef void		(*pfnEditor_Sys_SnapAxis)( int num, float *rgflAxis );
typedef void		(*pfnEditor_Sys_SnapVertexToGrid)( float *rgflVertex );
typedef void		(*pfnEditor_Sys_SnapMapVertex)( float *rgflVertex );

typedef char *		(*pfnEditor_V_VersionString)();

typedef float		(*pfnEditor_Sys_GetTextureGamma)();

typedef void *		(*pfnEditor_Global_GetCurrentWorld)();

typedef bool		(*pfnEditor_BuildPackageList)( void *pWorld ); // TODO

// Entity API
typedef void *		(*pfnEditor_Entity_Create)( void *world, const char *classname, float *rgflOrigin, int editorFlags );
typedef void		(*pfnEditor_Entity_Build)( void *mapEntity, int );
typedef void		(*pfnEditor_Entity_GetColor)( void *mapEntity, char *cColorOut );
typedef void		(*pfnEditor_Entity_SetColor)( void *mapEntity, char *cColor );
typedef void		(*pfnEditor_Entity_AddToVisGroup)( void *world, void *mapEntity, int visGroupId );
typedef void		(*pfnEditor_Entity_RemoveFromVisGroup)( void *world, void *mapEntity, int visGroupId );
typedef long		(*pfnEditor_Entity_GetVisGroupIdent)( void *mapEntity, int visGroupId );
typedef long		(*pfnEditor_Entity_GetVisGroupCount)( void *mapEntity );
typedef void *		(*pfnEditor_Entity_FindByClassname)( void *world, const char *classname );
typedef void *		(*pfnEditor_Entity_FindByTargetname)( void *world, const char *classname );
typedef void *		(*pfnEditor_Entity_FindByKeyValue)( void *world, const char *key, const char *value );

#define DIALOG_NONE				( 1 << 0 )
//#define DIALOG_ ( 1 << 1 )
#define DIALOG_CRITICAL			( 1 << 2 )
#define DIALOG_WARNING			( 1 << 3 )
#define DIALOG_INFORMATION		( 1 << 4 )
#define DIALOG_QUESTION			( 1 << 5 )

#define DIALOG_FILE_OPEN		( 1 << 1 ) /* Will make a dialog open files instead of saving them */
#define DIALOG_FILE_CONNECT		( 1 << 2 ) /* Will connect internal Qt signals to the panel when used with DIALOG_FILE_OPEN */

// Dialog API

/* Check for custom options */
/* title: dialog title */
/* text: dialog text */
/* options: list of options split by | */
/* returns a bitmask for each option selected */
typedef long		(*pfnEditor_Dialog_CheckOptions)( const char *title, const char *text, const char *options );

/* Display a message box */
/* title: dialog title */
/* text: dialog text */
/* flags: type and behavior flags */
typedef long		(*pfnEditor_Dialog_MessageBox)( const char *title, const char *text, int flags );

/* Allocates a CPluginDialog instance (NOTE: Only one plugin dialog can be active) */
/* title: dialog title */
typedef void		(*pfnEditor_Dialog_Begin)( const char *title );

/* buttonText: text that will be displayed on the button */
/* command: command that will be executed after button is pressed */
typedef void		(*pfnEditor_Dialog_InitExternalCommand)( const char *buttonText, const char *command );

/* buttonText: text that will be displayed on the button */
/* pfnCommand: function that will be executed after button is pressed */
typedef void		(*pfnEditor_Dialog_InitInternalCommand)( const char *buttonText, void (*pfnCommand)() );

/* value: progress bar current value */
/* maxValue: progress bar max value */
typedef void		(*pfnEditor_Dialog_SetProgress)( int value, int maxValue );

/* Add a textedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddTextEdit)( const char *controlName, const char *title, const char *defaultValue, int flags );

/* Add a radiobox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial state (true - checked; false - unchecked) */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddRadioBox)( const char *controlName, const char *title, bool defaultValue, int flags );

/* Add a checkbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial state (true - checked; false - unchecked) */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddCheckBox)( const char *controlName, const char *title, bool defaultValue, int flags );

/* Add a spinbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial value */
/* minValue: min value */
/* maxValue: max value */
/* stepCount: number of elements one step goes thru */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddSpinBox)( const char *controlName, const char *title, int defaultValue, int minValue, int maxValue, int stepCount, int flags );

/* Add a double spinbox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial value */
/* minValue: min value */
/* maxValue: max value */
/* stepCount: number of elements one step goes thru */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddSpinBoxFloat)( const char *controlName, const char *title, float defaultValue, float minValue, float maxValue, float stepCount, int flags );

/* Add a fileedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* extensionList: list of possible extensions split by "\n". Can be NULL */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddFileEdit)( const char *controlName, const char *title, const char *defaultValue, const char *extensionList, int flags );

/* Add a filelist control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* fileList: list of strings split by "\n" */
/* extensionList: list of possible extensions split by "\n". Can be NULL */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddFileList)( const char *controlName, const char *title, const char *fileList, const char *extensionList, int flags );

/* Add a combobox control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* selectedIndex: will set the selected item to this index after parsing the list from optionsList */
/* optionsList: list of combobox elements split by "\n" */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddComboBox)( const char *controlName, const char *title, int selectedIndex, const char *optionsList, int flags );

/* Add a directoryedit control to the dialog */
/* controlName: internal control name */
/* title: textedit title */
/* defaultValue: initial text */
/* flags: type and behevaior flags */
typedef void		(*pfnEditor_Dialog_AddDirectoryEdit)( const char *controlName, const char *title, const char *defaultValue, int flags );

/* Get control value (string) */
/* controlName: internal control name */
/* out: char buffer */
/* n: size of char buffer */
typedef void		(*pfnEditor_Dialog_QueryArgument)( const char *controlName, char *out, size_t n );

/* Get control value (float) */
/* controlName: internal control name */
/* Returns value of controlName */
typedef float		(*pfnEditor_Dialog_QueryArgumentFloat)( const char *controlName );

/* Get control value (int) */
/* controlName: internal control name */
/* Returns value of controlName */
typedef int			(*pfnEditor_Dialog_QueryArgumentInt)( const char *controlName );

/* Finish and draw the dialog */
typedef void		(*pfnEditor_Dialog_End)();

/* Appends text to the dialog */
typedef void		(*pfnEditor_Dialog_Printf)( const char *format, ... );

/* Changes current cursor to Qt::WaitCursor and increments a wait counter by 1 */
typedef void		(*pfnEditor_Dialog_BeginWait)();

/* Restores the cursor back to normal and decrements the wait counter by 1 */
typedef void		(*pfnEditor_Dialog_EndWait)();

// clang-format on

/* J.A.C.K. Plugin Interface */
/* nInterfaceVersion is a sizeof( plugin_funcs_t ) */
typedef struct plugin_funcs_s
{
	int nIntefaceVersion;

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

	void *Steam_SetAchievemnt;

	/* Parser API */
	pfnEditor_SC_Token pfnSC_Token;
	pfnEditor_SC_Line pfnSC_Line;
	pfnEditor_SC_ParseFromFile pfnSC_ParseFromFile;
	pfnEditor_SC_ParseFromMemory pfnSC_ParseFromMemory;
	pfnEditor_SC_CheckError pfnSC_CheckError;
	pfnEditor_SC_ParseError pfnSC_ParseError;
	pfnEditor_SC_ResetError pfnSC_ResetError;
	void *SC[18];

	/* Rendering API */
	void *PR[17];

	/* FileSystem API */
	pfnEditor_Sys_GetBaseDirectory pfnSys_GetBaseDirectory;
	pfnEditor_Sys_GetModDirectory pfnSys_GetModDirectory;
	pfnEditor_Sys_GetFallbackDirectory pfnSys_GetFallbackDirectory;
	void *FS[2];
	pfnEditor_Sys_FileExists pfnSys_FileExists;
	pfnEditor_Sys_LoadFile pfnSys_LoadFile;
	pfnEditor_Sys_CreatePath pfnSys_CreatePath;

	/* Math API */
	pfnEditor_Sys_PrintValue pfnSys_PrintValue;
	pfnEditor_Sys_PrintMapCoord pfnSys_PrintMapCoord;
	pfnEditor_Sys_PrintAxis pfnSys_PrintAxis;
	pfnEditor_Sys_SnapVertex pfnSys_SnapVertex;
	pfnEditor_Sys_SnapAxis pfnSys_SnapAxis;
	pfnEditor_Sys_SnapVertexToGrid pfnSys_SnapVertexToGrid;
	pfnEditor_Sys_SnapMapVertex pfnSys_SnapMapVertex;

	pfnEditor_V_VersionString pfnV_VersionString;

	pfnEditor_Sys_GetTextureGamma pfnSys_GetTextureGamma;

	pfnEditor_Global_GetCurrentWorld pfnGlobal_GetCurrentWorld;

	void *BuildPackageList;

	/* Entity API */
	pfnEditor_Entity_Create pfnEntity_Create;
	pfnEditor_Entity_Build pfnEntity_Build;
	pfnEditor_Entity_GetColor pfnEntity_GetColor;
	pfnEditor_Entity_SetColor pfnEntity_SetColor;
	pfnEditor_Entity_AddToVisGroup pfnEntity_AddToVisGroup;
	pfnEditor_Entity_RemoveFromVisGroup pfnEntity_RemoveFromVisGroup;
	pfnEditor_Entity_GetVisGroupIdent pfnEntity_GetVisGroupIdent;
	pfnEditor_Entity_GetVisGroupCount pfnEntity_GetVisGroupCount;
	pfnEditor_Entity_FindByClassname pfnEntity_FindByClassname;
	pfnEditor_Entity_FindByTargetname pfnEntity_FindByTargetname;
	pfnEditor_Entity_FindByKeyValue pfnEntity_FindByKeyValue;

	/* Brush API*/
	void *brush[8];

	/* Face API */
	void *face[2];

	/* Patch API */
	void *patch[7];

	/* Overlay API */
	void *overlay[2];

	/* Path API */
	void *path[3];

	/* Node API */
	void *node[3];

	/* Group API */
	void *group[7];

	/* Camera API */
	void *camera[5];

	/* Shader API */
	void *shader[11];

	/* VisGroup API */
	void *visgroup[7];

	/* Undo API */
	void *undo[27];

	/* Dialog API */
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
} plugin_funcs_t;

#define PLUGIN_VERSION 121

#include "PluginFace.h"

#endif // !PLUGINAPI_H
