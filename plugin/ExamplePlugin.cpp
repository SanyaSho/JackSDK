#include <stdio.h>
#include <memory.h>
#include <locale.h>

#include "PluginAPI.h"
#include "PluginActions.h"
#include "PluginProfile.h"
#include "PluginData.h"
#include "PluginEntity.h"

plugin_funcs_t gEditorfuncs;

/*
===============
vpMain
Application Entrypoint
===============
*/
DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
{
	if ( editorFuncs->nIntefaceVersion < sizeof( plugin_funcs_t ) )
		return -1;

	if ( editorPluginVersion != PLUGIN_VERSION )
		return PLUGIN_VERSION;

	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->nIntefaceVersion );
	setlocale( LC_ALL, "C" );
	return 0;
}

void InternalCommand()
{
	char szText[256] = { 0 };
	Dialog_QueryArgument( "QueryArgumentTest", szText, sizeof( szText ) );

	float flValue = Dialog_QueryArgumentFloat( "QueryArgumentFloatTest" );
	Dialog_SetProgress( flValue, 1.f );

	int nValue = Dialog_QueryArgumentInt( "QueryArgumentIntTest" );

	Dialog_Printf( "QueryArgumentTest: %s\nQueryArgumentFloatTest: %f\nQueryArgumentIntTest: %d\n", szText, flValue, nValue );
}

void RunTests()
{
	/*float vec3[3] = { 0, 0, 0, };
	void *ent = Entity_Create( Global_GetCurrentWorld(), "info_player_start", vec3, 0 );
	Entity_Build( ent, 1<<0 );
	Sys_Printf( "ent: %p\n", ent );*/

	Sys_Printf( "Sys_Printf" );
	Sys_DPrintf( "Sys_DPrintf" );
	Sys_Warning( "Sys_Warning" );
	Sys_Error( "Sys_Error" );

	void *ptr = Sys_Malloc( 32 );
	Sys_Free( ptr );

	const char *str = Sys_AllocString( "mystring" );
	Sys_Warning( str );
	Sys_Free( (void *)str );

	Sys_SetOption( SYS_OPTION_MAPGRID, 0 );

	for ( int i = 0; i < 6; i++ )
	{
		__int64 bOK = Sys_GetOption( i );

		Sys_Printf( "PLUGIN: %llu\n", bOK );
	}

	char out[260] = { 0 };
	bool bOK = Sys_GetBaseDirectory( out, sizeof( out ) );
	Sys_Warning( "Sys_GetBaseDirectory (%d): %s\n", bOK, out );

	char out2[260] = { 0 };
	bOK = Sys_GetModDirectory( out2, sizeof( out2 ) );
	Sys_Warning( "Sys_GetModDirectory (%d): %s\n", bOK, out2 );

	char out3[260] = { 0 };
	bOK = Sys_GetFallbackDirectory( out3, sizeof( out3 ) );
	Sys_Warning( "Sys_GetFallbackDirectory (%d): %s\n", bOK, out3 );

	Sys_Warning( "V_VersionString: %s\n", V_VersionString() );

	Sys_Warning( "Sys_GetTextureGamma: %f\n", Sys_GetTextureGamma() );

	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 3.7f ) );
	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 2.4f ) );

	long bitmask = Dialog_CheckOptions( NULL, NULL, "MYOPTION1|TestOption2|TestOption3|CustomOption4" );
	Sys_Printf( "CheckOptions: %d\n", bitmask );

	//Dialog_MessageBox( "test1testtesttesttesttesttest", "test2testtesttesttesttesttesttesttest", DIALOG_TYPE_QUESTION );

	Dialog_Begin( "MyDialog" );
	{
		Dialog_AddTextEdit( "QueryArgumentTest", "Text", "TestText", 0 );
		Dialog_AddTextEdit( "QueryArgumentFloatTest", "Progress", "1.0", 0 );

		Dialog_AddRadioBox( "QueryArgumentIntTest", "RadioBox", false, 0 );

		Dialog_AddCheckBox( "CheckBoxTest", "CheckBox", true, 0 );

		Dialog_AddFileEdit( "FileEdit", "Select Map", "mapsrc", "*.jmf\n*.rmf\n*.*", DIALOG_FILE_OPEN );

		Dialog_AddFileList( "FileList", "test", "file1\nfile2\nfile3\nfile4", NULL, DIALOG_FILE_OPEN );

		Dialog_AddDirectoryEdit( "DirectoryEdit", "test", "C:\\JACK_10155", DIALOG_FILE_OPEN );

		Dialog_AddComboBox( "ComboBox", "My Combo Box", 0, "opt1\nopt2\nopt3\nopt4\nopt5", 0 );

		// SpinBox with values [0-15], step count 1 and initial value 1
		Dialog_AddSpinBox( "SpinBox", "My SpinBox", 1, 0, 15, 1, 0 );

		// SpinBox with values [0-15], step count 1 and initial value 1
		Dialog_AddSpinBoxFloat( "SpinBoxFloat", "My SpinBox", 1.f, 0.f, 100.f, 2.5f, 0 );

		Dialog_InitExternalCommand( "buttonTextExternal", "test" );
		Dialog_InitInternalCommand( "buttonTextInternal", InternalCommand );

		Dialog_Printf( "Dialog_Printf test" );
	}
	Dialog_End();
}

pluginActionInfo_t runTests = { "Run tests", "&Run tests", "Call RunTests()", "ExamplePlugin", 0, RunTests };

void SpawnEntity()
{
	void *world = Global_GetCurrentWorld();
	if ( !world )
		return;

	float vec3_origin[3] = { 0, 0, 0, };

	qEntity_t *pEntity = Entity_Create( world, "info_player_start", vec3_origin, 0 );
	//Sys_Printf( "pEntity: %p\n", pEntity );

	Entity_AddToVisGroup( world, pEntity, 1 );

	Entity_Build( pEntity, 1 << 0 );

	FILE *f = fopen( "info_player_start.bin", "wb" );
	fwrite( pEntity, sizeof( *pEntity ), 1, f );
	fclose( f );
}

pluginActionInfo_t spawnEntity = { "Spawn Entity", "&Spawn Entity", "", "ExamplePlugin", 0, SpawnEntity };

/*
===============
vpEnumActions
Menu Actions
===============
*/
DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
{
	registerAction( &runTests, pluginManager );
	registerAction( &spawnEntity, pluginManager );
	return 2;
}

// clang-format off
mapProfile_t profile =
{
	NULL,

	0,

	0,

	/*
	hl1:
	0b100101100010100100000
	[5, 8, 10, 14, 15, 17, 20]

	q1:
	0b101010011000000010
	[1, 9, 10, 13, 15, 17]

	q2:
	0b11000010000011110
	[1, 2, 3, 4, 10, 15, 16]

	q3:
	0b10000000000001001001
	[0, 3, 6, 19]
	*/
	PROFILE_DECALTOOL_USE_HL1_DECALS | PROFILE_ENTITY_HL1_RENDERPROPS | ( 1 << 10 ) | ( 1 << 14 ) | PROFILE_SKY_SHADER | PROFILE_STUDIO_INVERT_PITCH_STATE | PROFILE_ENABLE_FALLBACK_DIR,

	"vpHalfLife",
	"Half-Life / TFC (Custom)",

	".bsp",
	"",
	".wad",
	".pak",
	".spr",
	".mdl",
	".wav",
	".aur",
	"",
	"",

	".tga",
	"ft;bk;rt;lf;up;dn",

	"sprites",
	"models",
	"sound",
	"aurora",
	"",
	"",
	"textures",
	"",
	"gfx/env",

	"-onlyents",
	"",
	"-fast",
	"-extra",
	"+map",

	"info_player_start",
	"",
	"light",
	"",
	"",
	"",
	"infodecal",
	"",

	"desert",
	"tools/trigger",
	"tools/nodraw",

	"tools/nodraw",
	"tools/nodraw",
	"tools/nodraw",

	"+-",

	0,
	0,
	0
};
// clang-format on

/*
===============
vpEnumProfiles
Game Profiles
===============
*/
DLL_EXPORT int vpEnumProfiles( pfnRegisterProfile registerProfile, void *libraryHandle )
{
	registerProfile( &profile, libraryHandle );
	return 1;
}

DLL_EXPORT int vpEnumSkyFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "TGA", ".tga", libraryHandle ) != false;
}

DLL_EXPORT int vpLoadSky( int formatIndex, const char *a, int b, const char *textureBaseName, int side )
{
	Sys_Printf( "vpLoadSky: %d %s %d %s %d\n", formatIndex, a, b, textureBaseName, side );
	return 0;
}

/*
===============
vpEnumExportFormats
IO Formats
===============
*/
DLL_EXPORT int vpEnumExportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "Plain Text", ".txt", libraryHandle ) != false;
}

DLL_EXPORT int vpExport( int formatIndex, const char *filePath, long seekOffset, long readLimit, void *pWorld )
{
	Sys_Printf( "vpImport (idx: %d / pth: %s / %d %d %p)\n", formatIndex, filePath, seekOffset, readLimit, pWorld );
	return 0;
}

/*
===============
vpEnumExportFormats
IO Formats
===============
*/
DLL_EXPORT int vpEnumImportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "Plain Text", ".txt", libraryHandle ) != false;
}

DLL_EXPORT int vpImport( int formatIndex, const char *filePath, long seekOffset, long readLimit, void *pWorld )
{
	Sys_Printf( "vpImport (idx: %d / pth: %s / %d %d %p)\n", formatIndex, filePath, seekOffset, readLimit, pWorld );

	/*struct CMapWorld
	{
		char gap[136];
		int flags; // 0x800000 - headless mode
		char gap2[188];
	};
	CMapWorld *data = (CMapWorld *)pWorld;

	FILE *f = fopen( "data.bin", "wb" );
	fwrite( data, sizeof( CMapWorld ), 1, f );
	fclose( f );

	Sys_Printf( "WorldData: %x %x %x\n", data->gap, data->flags, data->gap2 );*/

	/*if ( !SC_ParseFromFile( filePath, seekOffset, readLimit, 0 ) )
	{
		Sys_Error( "Can't load %s %d %d\n", filePath, seekOffset, readLimit );
		return 0;
	}

	const char *token = NULL;
	while ( SC_GetToken( true ) )
	{
		token = SC_Token();

		Sys_Printf( "token: %s\n", token );
	}*/

	return 0;
}

#if 0

typedef bool ( *pfnIOFormatFunc )( const char *filePath, void *b, void *c, void *d );

typedef struct pluginIOFormatDesc_s
{
	int formatIndex;
	const char *formatName;
	const char *formatExtension;
	pfnIOFormatFunc func;
} pluginIOFormatDesc_t;

// clang-format off
pluginIOFormatDesc_t exportFormats[] =
{
	{
		0,
		"Text1",
		".txt",
		NULL
	},
	
	{
		1,
		NULL, // "Log"
		".log",
		NULL
	},
	
	{
		2,
		"Text3",
		".txt3",
		NULL
	}
};
// clang-format on

/*
===============
vpEnumExportFormats
IO Formats
===============
*/
DLL_EXPORT int vpEnumExportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	int reg = 0;

	for ( int i = 0; i < sizeof( exportFormats ) / sizeof( exportFormats[0] ); i++ )
	{
		if ( registerIOFormat( exportFormats[i].formatIndex, exportFormats[i].formatName, exportFormats[i].formatExtension, libraryHandle ) )
		{
			reg++;
		}
	}

	return reg;
}

DLL_EXPORT int vpExport( int formatIndex, const char *filePath, void *b, void *c, void *d )
{
	Sys_Printf( "vpExport (idx: %d / pth: %s / %p %p %p)\n", formatIndex, filePath, b, c, d );

	for ( int i = 0; i < sizeof( exportFormats ) / sizeof( exportFormats[0] ); i++ )
	{
		if ( exportFormats[0].formatIndex == formatIndex && exportFormats[0].func )
		{
			return exportFormats[0].func( filePath, b, c, d );
		}
	}

	return 0;
}

// clang-format off
pluginIOFormatDesc_t importFormats[] =
{
	{
		0,
		"Text1",
		".txt",
		NULL
	},
	
	{
		1,
		NULL, // "Log"
		".log",
		NULL
	},
	
	{
		2,
		"Text3",
		".txt3",
		NULL
	}
};
// clang-format on

/*
===============
vpEnumExportFormats
IO Formats
===============
*/
DLL_EXPORT int vpEnumImportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	int reg = 0;

	for ( int i = 0; i < sizeof( importFormats ) / sizeof( importFormats[0] ); i++ )
	{
		if ( registerIOFormat( importFormats[i].formatIndex, importFormats[i].formatName, importFormats[i].formatExtension, libraryHandle ) )
		{
			reg++;
		}
	}

	return reg;
}

DLL_EXPORT int vpImport( int formatIndex, const char *filePath, void *b, void *c, void *d )
{
	Sys_Printf( "vpImport (idx: %d / pth: %s / %p %p %p)\n", formatIndex, filePath, b, c, d );

	for ( int i = 0; i < sizeof( importFormats ) / sizeof( importFormats[0] ); i++ )
	{
		if ( importFormats[0].formatIndex == formatIndex && importFormats[0].func )
		{
			return importFormats[0].func( filePath, b, c, d );
		}
	}

	return 0;
}

DLL_EXPORT int vpEnumPackageFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "VPK", ".vpk", libraryHandle ) != false;
}

DLL_EXPORT bool vpGetPackageInfo( int formatIndex, const char *filePath, int *a )
{
	*a = false;

	Sys_Printf( "vpGetPackageInfo (%d %s %d)\n", formatIndex, filePath, a );

	return false;
}

DLL_EXPORT bool vpLoadPackage( int formatIndex, const char *filePath )
{
	Sys_Printf( "vpLoadPackage (%d %s)\n", formatIndex, filePath );
	return false;
}

DLL_EXPORT int vpEnumSkyFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "VTF", ".vtf", libraryHandle ) != false;
}

/*DLL_EXPORT int vpLoadSky( int formatIndex )
{
	return 0;
}*/

DLL_EXPORT int vpEnumSpriteFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "VTF", ".vtf", libraryHandle ) != false;
}

/*DLL_EXPORT int vpSetPalette()
{
}*/

DLL_EXPORT bool vpLoadSprite( int formatIndex, const char *filePath, const char *a, const char *b, const char *c )
{
	Sys_Printf( "vpLoadSprite (%d %s %p %p %p)\n", formatIndex, filePath, a, b, c );
	return false;
}

DLL_EXPORT int vpEnumModelFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "MDL", ".mdl", libraryHandle ) != false;
}

#endif