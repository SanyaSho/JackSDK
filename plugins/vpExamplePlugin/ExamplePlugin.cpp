#include <stdio.h>
#include <memory.h>
#include <locale.h>
#if !defined( WIN32 )
#include <signal.h>
#endif // !WIN32

// class ImageLoader
#include "ImageLoader.h"

#include "PluginAPI.h"
#include "PluginActions.h"
#include "PluginCamera.h"
#include "PluginProfile.h"
#include "PluginData.h"
#include "PluginNodes.h"
#include "PluginPaths.h"
#include "PluginEntity.h"
#include "PluginRender.h"
#include "PluginWorld.h"
#include "PluginGroups.h"
#include "PluginPrimitives.h"

//#define STBI_MALLOC( sz )		 Sys_Malloc( sz )
//#define STBI_REALLOC( p, newsz ) realloc( p, newsz )
//#define STBI_FREE( p )			 Sys_Free( p )

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "MapReader.h"

//#include "DoomWadLoader.h"

#include "engine/r_studio.h"

plugin_funcs_t gEditorfuncs;

/*
===============
vpMain
Application Entrypoint
===============
*/
DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
{
	if ( editorFuncs->m_intefaceVersion < sizeof( plugin_funcs_t ) )
		return -1;

	if ( editorPluginVersion != PLUGIN_VERSION )
		return PLUGIN_VERSION;

	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->m_intefaceVersion );
	setlocale( LC_ALL, "C" );

	srand( 69069 );

	return 0;
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

	Sys_SetOption( SYS_OPTION_SHOWGRID, 0 );

	for ( int i = 0; i < 6; i++ )
	{
		int64 bOK = Sys_GetOption( i );

		Sys_Printf( "PLUGIN: %llu\n", bOK );
	}

	char out[260] = { 0 };
	bool bOK = Sys_GetBaseDirectory( out, sizeof( out ) );
	Sys_Warning( "Sys_GetBaseDirectory (%d): %s\n", bOK, out );

	char out2[260] = { 0 };
	bOK = Sys_GetModDirectory( out2, sizeof( out2 ) );
	Sys_Warning( "Sys_GetModDirectory (%d): %s\n", bOK, out2 );

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	char out3[260] = { 0 };
	bOK = Sys_GetFallbackDirectory( out3, sizeof( out3 ) );
	Sys_Warning( "Sys_GetFallbackDirectory (%d): %s\n", bOK, out3 );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

	Sys_Warning( "V_VersionString: %s\n", V_VersionString() );

	Sys_Warning( "Sys_GetTextureGamma: %f\n", Sys_GetTextureGamma() );

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 3.7f ) );
	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 2.4f ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
}

pluginActionDesc_t runTests = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"Run tests",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Run tests",
	"Call RunTests()",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
	RunTests
};

void SpawnEntity()
{
	qWorld_s *world = Global_GetCurrentWorld();
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

pluginActionDesc_t spawnEntity = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"Spawn Entity",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Spawn Entity",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	SpawnEntity
};

void CreateCamera()
{
	qWorld_s *world = Global_GetCurrentWorld();
	if ( !world )
		return;

	float vec3_origin[3] = { 0, 0, 0 };
	float vec3_angles90[3] = { 0, 35, 90 };

	qCamera_t *pCamera = Camera_Create( world );

	Camera_Setup( pCamera, vec3_origin, vec3_angles90 );

	unsigned char cbColor[4] = { 0x00, 0xFF, 0x00, 0x00 };
	Camera_SetColor( pCamera, cbColor );
}

pluginActionDesc_t createCamera = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"CreateCamera",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Create Camera",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	CreateCamera
};

void CreatePath()
{
	qWorld_s *worldDef = Global_GetCurrentWorld();
	if ( !worldDef )
		return;

	qPath_t *pathDef = Path_Create( worldDef );

	pathDef->m_pathClassname = Sys_AllocString( "path_corner" );
	pathDef->m_pathName = Sys_AllocString( "apiPath" );
	pathDef->m_pathDirection = 1; // One-way path

	for ( int i = 0; i < 5; i++ )
	{
		int dir = ( rand() & 2 );

		qNode_s *nodeDef = Node_Append( worldDef, pathDef );

		nodeDef->m_vecOrigin = vec3_t( dir == 0 ? i * 64.f : 0.f, dir == 1 ? i * 32.f : 0.f, dir == 2 ? i * 48.f : 0.f );
		nodeDef->m_vecAngles = vec3_t( 0.f, 0.f, 0.f );

		nodeDef->m_speed = 200.f;
		nodeDef->m_yaw_speed = 0.f;
		nodeDef->m_wait = 0.1f;
		nodeDef->m_fov = 90.f;
	}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	Path_Build( pathDef, 0 );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
}

pluginActionDesc_t createPath = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"CreatePath",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Create a Path",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	CreatePath
};

void PrintSysFloatTime()
{
	Sys_Printf( "Sys_FloatTime: %f\n", Sys_FloatTime() );
}

pluginActionDesc_t printSysFloatTime = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"PrintSysFloatTime",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Sys_FloatTime",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
	PrintSysFloatTime
};

void RunBuildPackageList()
{
	qWorld_s *world = Global_GetCurrentWorld();
	if ( !world )
		return;

	char *buf = NULL;

	// 0: packageList: C:/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/halflife.wad;C:/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/liquids.wad;C:/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/xeno.wad;C:/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/decals.wad;C:/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/zhlt.wad
	// 1: packageList: /Program Files (x86)/Steam/steamapps/common/Half-Life/valve/halflife.wad;/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/liquids.wad;/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/xeno.wad;/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/decals.wad;/Program Files (x86)/Steam/steamapps/common/Half-Life/valve/zhlt.wad
	bool bOK = BuildPackageList( world, &buf, ';', 0 );
	if ( bOK )
	{
		Sys_Printf( "packageList: %s", buf );
	}

	if ( buf )
	{
		Sys_Free( buf );
		buf = NULL;
	}
}

pluginActionDesc_t runBuildPackageList = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"RunBuildPackageList",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&BuildPackageList",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	RunBuildPackageList
};

extern void MessageBoxTest();
pluginActionDesc_t mbTest = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"MessageBoxTest",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&MessageBoxTest",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
	MessageBoxTest
};

void DebuggerBreakWithWorld()
{
	qWorld_t *worldDef = Global_GetCurrentWorld();
	if ( !worldDef )
		return;

	Sys_Printf( "worldDef: 0x%p", worldDef );
#if defined( WIN32 )
	__debugbreak();
#else
	raise( SIGTRAP );
#endif
}

pluginActionDesc_t debuggerbreakwithworld = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"DebuggerBreakWithWorld",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&DebuggerBreak with World",
	"Perform a __debugbreak with access to the world pointer",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	DebuggerBreakWithWorld
};

void FindPlayerSpawn()
{
	qWorld_t *worldDef = Global_GetCurrentWorld();
	if ( !worldDef )
		return;

	qEntity_t *entityDef = NULL;

	entityDef = Entity_FindByClassname( worldDef->m_entityList, "info_player_start" );
	if ( !entityDef )
		entityDef = Entity_FindByClassname( worldDef->m_entityList, "info_player_deathmatch" );

	if ( !entityDef )
		return;

	Sys_Printf( "Found %s on (%f %f %f)", entityDef->m_className, entityDef->m_vecOrigin.x, entityDef->m_vecOrigin.y, entityDef->m_vecOrigin.z );
}

pluginActionDesc_t findPlayerSpawn = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"FindPlayerSpawn",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&FindPlayerSpawn",
	"",
	"ExamplePlugin",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	ACTION_FLAG_INLEVEL,
	FindPlayerSpawn
};



#if 0
#define BSPVERSION	29

typedef struct
{
	int		fileofs, filelen;
} lump_t;

#define	LUMP_ENTITIES	0
#define	LUMP_PLANES		1
#define	LUMP_TEXTURES	2
#define	LUMP_VERTEXES	3
#define	LUMP_VISIBILITY	4
#define	LUMP_NODES		5
#define	LUMP_TEXINFO	6
#define	LUMP_FACES		7
#define	LUMP_LIGHTING	8
#define	LUMP_CLIPNODES	9
#define	LUMP_LEAFS		10
#define	LUMP_MARKSURFACES 11
#define	LUMP_EDGES		12
#define	LUMP_SURFEDGES	13
#define	LUMP_MODELS		14

#define	HEADER_LUMPS	15

typedef struct
{
	int			version;	
	lump_t		lumps[HEADER_LUMPS];
} dheader_t;

// LUMP_TEXTURES
typedef struct
{
	int			nummiptex;
	int			dataofs[4];		// [nummiptex]
} dmiptexlump_t;

#define	MIPLEVELS	4
typedef struct miptex_s
{
	char		name[16];
	unsigned	width, height;
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
} miptex_t;

void QuakeExtractTexturesToPNGs_LoadBSP( const char *filePath, const char *outDir )
{
	const char *file = Sys_MakeLocalFileName( SC_Token() );
	FILE *f = fopen( file, "rb" );
	if ( !f )
	{
		Dialog_Printf( "Failed to open \"%s\"", file );
		return;
	}

	dheader_t header;

	if ( fread( &header, 1, sizeof( dheader_t ), f ) != sizeof( dheader_t ) )
	{
		Dialog_Printf( "Error reading BSP file header" );
		fclose( f );
		return;
	}

	if ( header.version != BSPVERSION )
	{
		Dialog_Printf( "Bad BSP file version (%i should be %i)", header.version, BSPVERSION );
		fclose( f );
		return;
	}

	const lump_t &lumpTextures = header.lumps[LUMP_TEXTURES];

	if ( !lumpTextures.filelen )
	{
		Dialog_Printf( "BSP file contains no textures" );
		fclose( f );
		return;
	}

	fseek( f, lumpTextures.fileofs - 124, SEEK_CUR );

	int v3 = ftell( f );

	int nummiptex = 0;
	if ( fread( &nummiptex, 1, 4, f ) != 4 )
	{
		Dialog_Printf( "Error reading BSP file" );
		fclose( f );
		return;
	}

	if ( !nummiptex )
	{
		Dialog_Printf( "BSP file contains no textures" );
		fclose( f );
		return;
	}

	int imgBufSize = nummiptex * 4;
	int *imgBuf = (int *)Sys_Malloc( imgBufSize );

	if ( fread( imgBuf, 1, imgBufSize, f ) != imgBufSize )
	{
		Dialog_Printf( "Error reading BSP file textures" );
		Sys_Free( imgBuf );
		fclose( f );
		return;
	}

	for ( int i = 0; i < nummiptex && imgBuf != NULL; i++, imgBuf++ )
	{
		fseek( f, *imgBuf + v3, SEEK_SET );

		miptex_t mipTex;
		if ( fread( &mipTex, 1, sizeof( miptex_t ), f ) != sizeof( miptex_t ) )
		{
			Dialog_Printf( "Error reading file" );
			continue;
		}

		if ( mipTex.offsets[0] == 0 )
		{
			continue;
		}

		/*if ( ( mipTex.width & 0xF ) == 0 || ( mipTex.height & 0xF ) == 0 )
		{
			Dialog_Printf( "Texture '%s' is not 16 aligned", mipTex.name );
			continue;
		}*/

		int bufSize = ( 85 * mipTex.width * mipTex.height ) >> 6;
		byte *buf = (byte *)Sys_Malloc( bufSize );
		if ( !buf )
		{
			Dialog_Printf( "Memory allocation failure" );
			continue;
		}

		if ( mipTex.offsets[0] != 40 )
		{
			fseek( f, v3 + mipTex.offsets[0] + *imgBuf, SEEK_SET );
		}

		if ( fread( buf, 1, bufSize, f ) != bufSize )
		{
			Dialog_Printf( "Error reading file" );
			Sys_Free( buf );
			continue;
		}

		char fileName[MAX_PATH] = { 0 };
		snprintf( fileName, sizeof( fileName ), "%s/%s.bmp", outDir, mipTex.name );
		fileName[sizeof( fileName ) - 1] = '\0';
		stbi_write_bmp( fileName, mipTex.width, mipTex.height, 1, buf );
		Dialog_Printf( "Writting %s", fileName );
	}

	Sys_Free( imgBuf );
	fclose( f );
}

void QuakeExtractTexturesToPNGs_Command()
{
	Dialog_Printf( "Extracting Quake textures..." );

	static float startTime = Sys_FloatTime();

	{
		char fileList[8192] = { 0 };
		Dialog_QueryArgument( "src", fileList, sizeof( fileList ) );

		char outDir[MAX_PATH] = { 0 };
		Dialog_QueryArgument( "dir", outDir, sizeof( outDir ) );

		SC_ParseFromMemory( fileList, strlen( fileList ), 0 );

		while ( SC_GetToken( true ) )
		{
			char fileExtension[32] = { 0 };
			V_ExtractFileExtension( SC_Token(), fileExtension, sizeof( fileExtension ) );

			if ( !strcmp( fileExtension, ".bsp" ) )
			{
				Dialog_Printf( "Loading: \"%s\"", SC_Token() );

				QuakeExtractTexturesToPNGs_LoadBSP( SC_Token(), outDir );
			}
		}
	}

	static float endTime = Sys_FloatTime();
	Dialog_Printf( "%.2f seconds elapsed", endTime - startTime );
}

void QuakeExtractTexturesToPNGs()
{
	Dialog_Begin( "Extract Quake Textures" );
	Dialog_InitInternalCommand( "E&xtract", QuakeExtractTexturesToPNGs_Command );
	Dialog_AddFileList( "src", "Source Files", NULL, "BSP and PAK Files (*.bsp *.pak)", 3 );
	Dialog_AddDirectoryEdit( "dir", "Output Directory", "quake/", 1 );
	Dialog_End();
}

pluginActionDesc_t quakeExtractTexturesToPNGs = { "QuakeExtractTexturesToPNGs", "&Extract textures...", "Extract Quake textures from BSP/PAK files", "ExamplePlugin", 0, QuakeExtractTexturesToPNGs };
#endif

#if 0
void DoomWadTest()
{
	Dialog_Begin( "Extract DOOM Textures" );
	Dialog_InitInternalCommand( "E&xtract",
		[]()
		{
			char fileList[8192] = { 0 };
			Dialog_QueryArgument( "src", fileList, sizeof( fileList ) );

			SC_ParseFromMemory( fileList, strlen( fileList ), 0 );
			
			while ( SC_GetToken( true ) )
			{
				char fileExtension[32] = { 0 };
				V_ExtractFileExtension( SC_Token(), fileExtension, sizeof( fileExtension ) );

				if ( strcmp( fileExtension, ".wad" ) )
				{
					Dialog_Printf( "Loading: \"%s\"", SC_Token() );

					DoomWadReader wadReader;
					if ( wadReader.LoadFile( SC_Token() ) )
					{
						wadReader.Read();
					}
				}
			}
		}
	);
	Dialog_AddFileList( "src", "Source Files", NULL, "DOOM IWAD Files (*.wad)", 3 );
	Dialog_End();
}

pluginActionDesc_t doomwadtest = { "DoomWadTest", "&Extract textures...", "Extract DOOM textures from IWAD files", "ExamplePlugin", 0, DoomWadTest };
#endif

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
	registerAction( &createCamera, pluginManager );
	registerAction( &createPath, pluginManager );
	registerAction( &printSysFloatTime, pluginManager );
	registerAction( &runBuildPackageList, pluginManager );
	registerAction( &mbTest, pluginManager );
	registerAction( &debuggerbreakwithworld, pluginManager );
	registerAction( &findPlayerSpawn, pluginManager );
#if 0
	registerAction( &doomwadtest, pluginManager );
	return 11;
#endif
#if 0
	registerAction( &quakeExtractTexturesToPNGs, pluginManager );
	return 11;
#endif
	return 10;
}
/*
typedef struct mapCallbacks_s
{
	size_t m_interfaceVersion;

	void ( *fn0 )( const char *, void * );
	void ( *fn1 )( char *, const char * );
	void ( *fn2 )( char *, char *, bool );
	void ( *fn3 )( char *, char *, void *, void *, void * );
	void ( *fn4 )( char *, char *, char *, char *, size_t );
	void ( *fn5 )( char *, int, char *, size_t );
} mapCallbacks_t;*/

bool ShaderEditor_LoadImage( const char *filePath, qShader_s *shaderDef )
{
	return false;
}

bool ShaderEditor_LoadScriptFile( const char *filePath, qShader_s *shaderDef )
{
	return false;
}

bool ShaderEditor_SaveScriptFile( const char *filePath, qShader_s *shaderDef, bool recursive )
{
	return false;
}

bool ShaderEditor_BuildSourceCode( const char *filePath, qShader_s *shaderDef, pfnShaderEditor_Message pfnPushWarning, pfnShaderEditor_Message pfnPushError, void *shaderEditorDialog )
{
	pfnPushWarning( 1, "BuildSourceCode pfnPushWarning", shaderEditorDialog );
	pfnPushError( 2, "BuildSourceCode pfnPushError", shaderEditorDialog );
	return false;
}

bool ShaderEditor_ReformatText( const char *str1, const char *str2, int, char *outBuf, size_t outBufSize )
{
	return false;
}

bool ShaderEditor_FillCompleteSuggestions( const char *parmName, int a, char *outBuf, size_t outBufSize )
{
	if ( outBuf )
	{
		memset( outBuf, 0, outBufSize );
	}

	if ( parmName && !strcmp( parmName, "test" ) )
	{
		strncat( outBuf, "autocomp1", outBufSize );
		strncat( outBuf, " ", outBufSize );

		strncat( outBuf, "autocomp2", outBufSize );
		strncat( outBuf, " ", outBufSize );
		return true;
	}

	return false;
}

// clang-format off
shaderEditorCallbacks_t g_callbacks =
{
	sizeof( shaderEditorCallbacks_t ),
	&ShaderEditor_LoadImage,
	&ShaderEditor_LoadScriptFile,
	&ShaderEditor_SaveScriptFile,
	&ShaderEditor_BuildSourceCode,
	&ShaderEditor_ReformatText,
	&ShaderEditor_FillCompleteSuggestions
};

mapProfile_t profile =
{
	NULL,

	&g_callbacks,

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
	PROFILE_ENABLE_SHADER_EDITOR | ( 1 << 6  ) | PROFILE_FACE_HAS_SURFACEFLAGS | PROFILE_FACE_HAS_CONTENTFLAGS |PROFILE_DECALTOOL_USE_Q3_OVERLAYS | PROFILE_ENTITY_HL1_RENDERPROPS | ( 1 << 10 ) | ( 1 << 14 ) | PROFILE_SKY_SHADER | PROFILE_STUDIO_INVERT_PITCH_STATE | PROFILE_ENABLE_FALLBACK_DIR,
	//1<<0|1<<3|1<<6|1<<19,

	"vpHalfLife",
	"Half-Life / TFC (Custom)",

	".bsp",
	".png;.bmp;.gif;.psd;.pic;.jpg;.pnm;.hdr;.tga",/* vpEnumTextureFormats */ //"",
	".wad",
	".pak",
	".spr;.png;.gif", /* vpEnumSpriteFormats */ //".spr",
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
	"scripts",
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

	1<<2,
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
#if 1
DLL_EXPORT int vpEnumProfiles( pfnRegisterProfile registerProfile, void *libraryHandle )
{
	registerProfile( &profile, libraryHandle );
	return 1;
}

#if 0
DLL_EXPORT int vpEnumSkyFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "TGA", ".tga", libraryHandle ) != false;
}

DLL_EXPORT int vpLoadSky( int formatIndex, byte *buf, int bufSize, qShader_s *skyShader, int side )
{
	Sys_Printf( "vpLoadSky: %d 0x%p %d %s %d\n", formatIndex, buf, bufSize, skyShader->m_name, side );
	return 0;
}
#endif
#endif

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

DLL_EXPORT int vpExport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, void *pWorld )
{
	Sys_Printf( "vpExport (idx: %d / pth: %s / %d %d %p)\n", formatIndex, filePath, seekOffset, readLimit, pWorld );
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

DLL_EXPORT int vpImport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
{
	Sys_Printf( "vpImport (idx: %d / pth: %s / seekoff: %d / readlim: %d / world: %p)\n", formatIndex, filePath, seekOffset, readLimit, worldDef );

	MapReader mapReader( filePath, seekOffset, readLimit, worldDef );

	return mapReader.LoadMap();

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

	//return 0;
}


#if 0
DLL_EXPORT int vpEnumModelFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "Half-Life Alpha 0.52 MDL", ".mdl", libraryHandle ) != false;
	return 1;
}
#endif // 0

DLL_EXPORT bool vpGetModelBounds( int formatIndex, float *bboxMin, float *bboxMax, unsigned int flags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	if ( formatIndex != 0 )
		return false;

	StudioRender *studioRender = (StudioRender *)studioData->m_studioPtr;
	if ( studioRender )
	{
		studioRender->GetModelBounds( flags, (vec3_t *)&bboxMin, (vec3_t *)&bboxMax );
		return true;
	}

	return false;
}

DLL_EXPORT void vpUnloadModel( int formatIndex, qStudioData_s *studioData )
{
	if ( formatIndex != 0 )
		return;

	StudioRender *studioRender = (StudioRender *)studioData->m_studioPtr;
	if ( studioRender )
	{
		delete studioRender;
		studioData->m_studioPtr = NULL;
	}
}

DLL_EXPORT bool vpLoadModel( int formatIndex, const char *filePath, byte *buf, int bufSize, qStudioData_s *studioData )
{
	if ( formatIndex != 0 )
		return false;

	StudioRender *studioRender = new StudioRender();
	if ( !studioRender->Mod_LoadStudioModel( filePath, buf, bufSize, studioData ) )
	{
		delete studioRender;
		return false;
	}

	studioRender->GetModelBounds( 0, &studioData->m_bboxMin, &studioData->m_bboxMax );

	studioData->m_studioPtr = studioRender;
	return true;
}

DLL_EXPORT void vpRenderModel( int formatIndex, int renderFlags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	if ( formatIndex != 0 )
		return;

	StudioRender *studioRender = (StudioRender *)studioData->m_studioPtr;
	if ( studioRender )
	{
		studioRender->R_StudioRenderFinal( entityInfo, studioData, renderFlags );
	}
}

/*struct qShader_s
{
	char gap[72];
	int maxWidth;
	int maxHeight;
	int flags;
	char gap2[1320];
};*/

DLL_EXPORT int vpEnumSurfaceFlags( pfnRegisterFlags reg, void *libraryHandle )
{
	reg( "vpHalfLife", "MySurfaceFlag", 1, libraryHandle );
	reg( "vpHalfLife", "MySurfaceFlag 2", 2, libraryHandle );
	return 1;
}

DLL_EXPORT int vpEnumContentFlags( pfnRegisterFlags reg, void *libraryHandle )
{
	reg( "vpHalfLife", "MyContentFlag", 1, libraryHandle );
	reg( "vpHalfLife", "MyContentFlag 2", 2, libraryHandle );
	return 1;
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
