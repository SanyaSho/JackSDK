#include "stdio.h"

#if defined( WIN32 )
#include "Windows.h"

#define _DLL_EXT ".dll"
#define _PATHSEP "\\"
#else
#include <unistd.h>
#include <dlfcn.h>

typedef void *HMODULE;

static bool GetModuleFileName( void *, char *outBuf, size_t outBufSize )
{
	return readlink( "/proc/self/exe", outBuf, outBufSize ) > 0;
}

#define LOAD_WITH_ALTERED_SEARCH_PATH 0

static HMODULE LoadLibraryEx( const char *pszPath, void *, int )
{
	return dlopen( pszPath, RTLD_NOW );
}

static void *GetProcAddress( HMODULE hHandle, const char *pszName )
{
	return dlsym( hHandle, pszName );
}

#define _DLL_EXT ".so"
#define _PATHSEP "/"
#endif // WIN32

#include "GL/glew.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


static void Sys_Free( void *ptr );
static void *Sys_Malloc( size_t n );
static char *Sys_AllocString( const char *src );

#define PLUGINEDITORFUNCTIONS_H
#include "PluginMeta.h"


#if defined( JACK_64BIT )
#define PLUGIN_DLL "vpHalfLifex64" _DLL_EXT
//#define PLUGIN_DLL "vpQuakex64" _DLL_EXT
//#define PLUGIN_DLL "vpQuake3x64" _DLL_EXT
#else
#define PLUGIN_DLL "vpHalfLifex86" _DLL_EXT
//#define PLUGIN_DLL "vpQuakex86" _DLL_EXT
//#define PLUGIN_DLL "vpQuake3x86" _DLL_EXT
#endif


// clang-format off


/* Package IO */
//typedef bool (*vpGetPackageInfo)( int formatIndex, const char *filePath, int *numMipTex );
//typedef bool (*vpLoadPackage)( int formatIndex, const char *filePath );


/* Sky IO */
//typedef bool (*vpLoadSky)( int formatIndex, byte *buf, unsigned int bufSize, qShader_s *skyInfo, unsigned int side );


/* Sprite IO */
/*typedef struct qSpriteData_s
{
	char gap[12];
	int orientationMethod;
	qShader_s *shaderData;
} qSpriteData_t;*/

//typedef bool (*vpLoadSprite)( int formatIndex, const char *filePath, byte *buf, int bufSize, qSpriteData_s *outSpriteData );


/* Model IO */
/*typedef struct qStudioData_s
{
	char gap[16];
	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
	
	// Pointer to the internal plugin data (ex. allocated studiohdr struct)
	void *m_modelInfo;
} qStudioData_t;*/

//typedef bool (*vpGetModelFormatFlags)( int );
//typedef bool (*vpGetModelBounds)( int formatIndex, float *bboxMin, float *bboxMax, unsigned int flags, qStudioData_s *studioData, long a6 );
//typedef bool (*vpUnloadModel)( int formatIndex, qStudioData_s *studioData );
//typedef bool (*vpLoadModel)( int formatIndex, const char *filePath, byte *buf, int bufSize, qStudioData_s *outStudioData );
//typedef bool (*vpRenderModel)( int formatIndex, int a2, qStudioData_s *studioData, long a4 );


/* Archive IO */
/*typedef struct qArchiveData_s
{
	char gap[24];

	/* Pointer to the internal plugin data (ex. allocated PAK struct) */
	/*
		This pointer is allocated in the "vpLoadArchive" function and must be freed inside a "vpUnloadArchive"
	void *m_archiveInfo;
} qArchiveData_t;*/

//typedef void (*vpUnloadArchive)( int formatIndex, qArchiveData_s *archiveData );

//typedef bool (*vpLoadArchvie)( int formatIndex, const char *filePath, qArchiveData_s *outArchiveData );

// Returns true if file was found, false if not
//typedef bool (*vpFindArchiveFile)( int formatIndex, qArchiveData_s *archiveData, const char *filePath ); // Accepts full path to the file inside an archive

// outBuf must be freed
// Returns size of the file in bytes
/*
 Sys_Free( outBuf );
*/
//typedef long (*vpLoadArchiveFile)( int formatIndex, qArchiveData_s *archiveData, const char *filePath, char **outBuf ); // Accepts full path to the file inside an archive

// outFiles must be individually freed
/*
 for ( int i = 0; outFiles[i] != NULL; i++ )
 {
 	Sys_Free( outFiles[i] );
 }
 Sys_Free( outFiles );
*/
//typedef bool (*vpListArchiveFiles)( int formatIndex, qArchiveData_s *archiveData, const char *fileFilter, char ***outFiles );

// clang-format on

//-----------------------------------------------------------------------------
// Purpose: Return the directory where this .exe is running from
// Output : char
//-----------------------------------------------------------------------------
static char *GetBaseDir( const char *pszBuffer )
{
	static char basedir[MAX_PATH];
	char szBuffer[MAX_PATH];
	size_t j;
	char *pBuffer = NULL;

	strcpy( szBuffer, pszBuffer );

#ifdef _WIN32
	pBuffer = strrchr( szBuffer, '\\' );
#else
	pBuffer = strrchr( szBuffer, '/' );
#endif
	if ( pBuffer )
	{
		*( pBuffer + 1 ) = '\0';
	}

	strcpy( basedir, szBuffer );

	j = strlen( basedir );
	if ( j > 0 )
	{
		if ( ( basedir[j - 1] == '\\' ) || ( basedir[j - 1] == '/' ) )
		{
			basedir[j - 1] = 0;
		}
	}

	return basedir;
}

static void Sys_Printf( const char *format, ... )
{
	static char szMessage[8192] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( szMessage, sizeof( szMessage ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	OutputDebugString( szMessage );
	OutputDebugString( "\n" );
#else
	fprintf( stdout, "%s\n", szMessage );
#endif
}

static void Sys_Error( const char *format, ... )
{
	static char szError[8192] = { 0 };

	va_list argptr;
	va_start( argptr, format );
	vsnprintf( szError, sizeof( szError ), format, argptr );
	va_end( argptr );

#if defined( WIN32 )
	char szBuf[1024];
	_snprintf( szBuf, sizeof( szBuf ), "%s", szError );
	szBuf[sizeof( szBuf ) - 1] = '\0';
	MessageBox( 0, szBuf, "Error", MB_OK );
#else
	fprintf( stderr, "%s\n", szError );
#endif

	exit( 1 );
}

static void Sys_Free( void *ptr )
{
	//Sys_Printf( "%s( 0x%p )", __FUNCTION__, ptr );
	free( ptr );
}

static void *Sys_Malloc( size_t n )
{
	//Sys_Printf( "%s( %d )", __FUNCTION__, n );

	void *ptr = calloc( 1, n );
	if ( !ptr )
	{
		Sys_Error( "Epic Fail!" );
		return calloc( 1, n );
	}

	return ptr;
}

static void *TempBuffer_GetSpace( int buffer, size_t size )
{
	void *ptr = Sys_Malloc( size );
	Sys_Printf( "%s( %d, %d ) -> 0x%p", __FUNCTION__, buffer, size, ptr );
	return ptr;
}

static char *Sys_AllocString( const char *src )
{
	//Sys_Printf( "%s( %s )", __FUNCTION__, src );
	if ( !src )
		return NULL;

	int len = strlen( src );
	char *str = (char *)Sys_Malloc( len + 1 );
	strcat( str, src );
	return str;
}

static float Sys_FloatTime()
{
	Sys_Printf( "%s()", __FUNCTION__ );
	return 1.f;
}

static float Sys_GetTextureGamma()
{
	return 1.f;
}

static char *Sys_MakeLocalFileName( const char *file )
{
	Sys_Printf( "%s( %s )", __FUNCTION__, file );
	return (char *)file;
}

static byte *Sys_LoadFile( const char *filePath, int *numBytesRead )
{
	Sys_Printf( "%s( %s, 0x%p )", __FUNCTION__, filePath, numBytesRead );
	FILE *f = fopen( Sys_MakeLocalFileName( filePath ), "rb" );
	if ( !f )
	{
		Sys_Error( "can't open \"%s\"", filePath );
		return NULL;
	}

	fseek( f, 0, SEEK_END );
	int fileSize = ftell( f );
	rewind( f );

	void *buf = Sys_Malloc( fileSize + 1 );

	int readSize = fread( buf, 1, fileSize, f );
	if ( readSize == fileSize )
	{
		if ( numBytesRead )
			*numBytesRead = readSize;

		fclose( f );
		return (byte *)buf;
	}

	return NULL;
}

/*struct qShader_s
{
	char gap[1408];
};

struct qTexture_s
{
	char gap[48];
};*/

static qShader_s *Shader_Create( const char *shaderName, const char *textureName, int a )
{
	void *ptr = Sys_Malloc( sizeof( qShader_s ) );
	memset( ptr, 0, sizeof( qShader_s ) );
	Sys_Printf( "%s( %s, %s, %d ) -> 0x%p", __FUNCTION__, shaderName, textureName, a, ptr );
	return (qShader_s *)ptr;
}

static qShader_s *Shader_Lookup( const char *shaderName )
{
	void *ptr = Sys_Malloc( sizeof( qShader_s ) );
	memset( ptr, 0, sizeof( qShader_s ) );
	Sys_Printf( "%s( %s ) -> 0x%p", __FUNCTION__, shaderName, ptr );
	return (qShader_s *)ptr;
}

static void Shader_Destroy( qShader_s *shaderHandle )
{
	Sys_Printf( "%s( 0x%p (%s) )", __FUNCTION__, shaderHandle, shaderHandle->m_name );
	Sys_Free( shaderHandle );
}

static void Shader_AddStage( qShader_s *shaderHandle, qShaderStage_s *shaderStage )
{
	Sys_Printf( "%s( 0x%p, 0x%p )", __FUNCTION__, shaderHandle, shaderStage );
}

static void Shader_RemoveStage( qShaderStage_s *shaderStage )
{
	Sys_Printf( "%s( 0x%p )", __FUNCTION__, shaderStage );
}

static void Shader_Finish( qShader_s *shaderHandle )
{
	Sys_Printf( "%s( 0x%p )", __FUNCTION__, shaderHandle );
}

static qTexture_s *Shader_GetWhiteTexture()
{
	Sys_Printf( "%s()", __FUNCTION__ );
	return NULL;
}

static qTexture_s *Shader_GetBlackTexture()
{
	Sys_Printf( "%s()", __FUNCTION__ );
	return NULL;
}

static qTexture_s *Shader_LookupTexture( const char *textureName )
{
	Sys_Printf( "%s( %s )", __FUNCTION__, textureName );
	return NULL;
}

static int s_texNum = 0;

static qTexture_s *Shader_UploadTexture( qShader_s *shaderHandle, const char *shaderName, unsigned int pixelFormat, unsigned int textureFormat, int textureNumChannels, int textureWidth, int textureHeight, bool b, unsigned char *textureData )
{
	void *ptr = Sys_Malloc( sizeof( qTexture_s ) );
	Sys_Printf( "%s( 0x%p, %d, %d, %d, %d, %d, %d, 0x%p ) -> 0x%p", __FUNCTION__, shaderHandle, pixelFormat, textureFormat, textureNumChannels, textureWidth, textureHeight, b, ptr );

	{
		char s[16];
		snprintf( s, sizeof( s ), "%d.bmp", s_texNum++ );
		s[sizeof( s ) - 1] = 0;
		stbi_write_bmp( s, textureWidth, textureHeight, textureNumChannels, textureData );
		Sys_Printf( "Writting %s", s );
	}

	return (qTexture_s *)ptr;
}

static void Shader_DestroyTexture( qTexture_s *textureHandle )
{
	Sys_Printf( "%s( 0x%p )", __FUNCTION__, textureHandle );
	Sys_Free( textureHandle );
}

plugin_funcs_t gEditorfuncs;
void InitializeEditorFuncs()
{
	memset( &gEditorfuncs, 0, sizeof( plugin_funcs_t ) );

	gEditorfuncs.m_intefaceVersion = sizeof( plugin_funcs_t );

	gEditorfuncs.pfnSys_Printf = Sys_Printf;
	gEditorfuncs.pfnSys_DPrintf = Sys_Printf;
	gEditorfuncs.pfnSys_Warning = Sys_Printf;
	gEditorfuncs.pfnSys_Error = Sys_Error;

	gEditorfuncs.pfnSys_Free = Sys_Free;
	gEditorfuncs.pfnSys_Malloc = Sys_Malloc;

	gEditorfuncs.pfnTempBuffer_GetSpace = TempBuffer_GetSpace;
	gEditorfuncs.pfnSys_AllocString = Sys_AllocString;

	gEditorfuncs.pfnSys_FloatTime = Sys_FloatTime;

	gEditorfuncs.pfnSys_SetOption = NULL;
	gEditorfuncs.pfnSys_GetOption = NULL;

	gEditorfuncs.pfnSteam_SetAchievemnt = NULL;

	gEditorfuncs.parserfuncs.pfnSC_Token = NULL;
	gEditorfuncs.parserfuncs.pfnSC_Line = NULL;
	gEditorfuncs.parserfuncs.pfnSC_ParseFromFile = NULL;
	gEditorfuncs.parserfuncs.pfnSC_ParseFromMemory = NULL;
	gEditorfuncs.parserfuncs.pfnSC_CheckError = NULL;
	gEditorfuncs.parserfuncs.pfnSC_ParseError = NULL;
	gEditorfuncs.parserfuncs.pfnSC_ResetError = NULL;
	gEditorfuncs.parserfuncs.pfnSC_SafeGetToken = NULL;
	gEditorfuncs.parserfuncs.pfnSC_GetToken = NULL;
	gEditorfuncs.parserfuncs.pfnSC_TokenAvailable = NULL;
	gEditorfuncs.parserfuncs.pfnSC_UnGetToken = NULL;
	gEditorfuncs.parserfuncs.pfnSC_MatchToken = NULL;
	gEditorfuncs.parserfuncs.pfnSC_SafeMatchToken = NULL;
	// SC_Matrix
	gEditorfuncs.parserfuncs.pfnSC_SkipRestOfLine = NULL;
	gEditorfuncs.parserfuncs.pfnSC_EndOfParsing = NULL;
	gEditorfuncs.parserfuncs.pfnSC_GetParseFlags = NULL;
	gEditorfuncs.parserfuncs.pfnSC_SetParseFlags = NULL;
	gEditorfuncs.parserfuncs.pfnSC_ShouldQuote = NULL;
	gEditorfuncs.parserfuncs.pfnSC_CopyBlock = NULL;
	gEditorfuncs.parserfuncs.pfnSC_SkipBlock = NULL;
	gEditorfuncs.parserfuncs.pfnSC_SkipLineOrBlock = NULL;
	gEditorfuncs.parserfuncs.pfnSC_GetBlockSize = NULL;

	gEditorfuncs.pfnSys_GetTextureGamma = Sys_GetTextureGamma;

	// PR[17]

	gEditorfuncs.filesystemfuncs.pfnSys_GetBaseDirectory = NULL;
	gEditorfuncs.filesystemfuncs.pfnSys_GetModDirectory = NULL;
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	gEditorfuncs.filesystemfuncs.pfnSys_GetFallbackDirectory = NULL;
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	gEditorfuncs.filesystemfuncs.pfnSys_ExpandFileName = NULL;
	gEditorfuncs.filesystemfuncs.pfnSys_MakeLocalFileName = Sys_MakeLocalFileName;
	gEditorfuncs.filesystemfuncs.pfnSys_FileExists = NULL;
	gEditorfuncs.filesystemfuncs.pfnSys_LoadFile = Sys_LoadFile;
	gEditorfuncs.filesystemfuncs.pfnSys_CreatePath = NULL;

	gEditorfuncs.shaderfuncs.pfnShader_Create = Shader_Create;
	gEditorfuncs.shaderfuncs.pfnShader_Lookup = Shader_Lookup;
	gEditorfuncs.shaderfuncs.pfnShader_Destroy = Shader_Destroy;
	gEditorfuncs.shaderfuncs.pfnShader_AddStage = Shader_AddStage;
	gEditorfuncs.shaderfuncs.pfnShader_RemoveStage = Shader_RemoveStage;
	gEditorfuncs.shaderfuncs.pfnShader_Finish = Shader_Finish;
	gEditorfuncs.shaderfuncs.pfnShader_GetWhiteTexture = Shader_GetWhiteTexture;
	gEditorfuncs.shaderfuncs.pfnShader_GetBlackTexture = Shader_GetBlackTexture;
	gEditorfuncs.shaderfuncs.pfnShader_LookupTexture = Shader_LookupTexture;
	gEditorfuncs.shaderfuncs.pfnShader_UploadTexture = Shader_UploadTexture;
	gEditorfuncs.shaderfuncs.pfnShader_DestroyTexture = Shader_DestroyTexture;
}

static void Editor_RegisterProfile( mapProfile_t *profileInfo, void *libraryHandle )
{
}

static bool Editor_RegisterIOFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );
	return true;
}

static vpSetPalette_t s_vpSetPalette;


/* Texture IO */

static bool Editor_RegisterTextureFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpSetPalette = (vpSetPalette_t)GetProcAddress( (HMODULE)libraryHandle, "vpSetPalette" );

	return true;
}


/* Package IO */
static vpGetPackageInfo_t s_vpGetPackageInfo;
static vpLoadPackage_t s_vpLoadPackage;

static bool Editor_RegisterPackageFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpSetPalette = (vpSetPalette_t)GetProcAddress( (HMODULE)libraryHandle, "vpSetPalette" );

	s_vpGetPackageInfo = (vpGetPackageInfo_t)GetProcAddress( (HMODULE)libraryHandle, "vpGetPackageInfo" );
	if ( !s_vpGetPackageInfo )
	{
		Sys_Error( "Plugin \"%s\" defines package format \"%s\" (%s), but doesn't export \"vpGetPackageInfo\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpLoadPackage = (vpLoadPackage_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadPackage" );
	if ( !s_vpLoadPackage )
	{
		Sys_Error( "Plugin \"%s\" defines package format \"%s\" (%s), but doesn't export \"vpLoadPackage\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


/* Sky IO */
static vpLoadSky_t s_vpLoadSky;

static bool Editor_RegisterSkyFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpLoadSky = (vpLoadSky_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadSky" );
	if ( !s_vpLoadSky )
	{
		Sys_Error( "Plugin \"%s\" defines sky format \"%s\" (%s), but doesn't export \"vpLoadSky\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


/* Sprite IO */
static vpLoadSprite_t s_vpLoadSprite;

static bool Editor_RegisterSpriteFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpSetPalette = (vpSetPalette_t)GetProcAddress( (HMODULE)libraryHandle, "vpSetPalette" );

	// vpUnloadSprite

	s_vpLoadSprite = (vpLoadSprite_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadSprite" );
	if ( !s_vpLoadSprite )
	{
		Sys_Error( "Plugin \"%s\" defines sprite format \"%s\" (%s), but doesn't export \"vpLoadSprite\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


/* Model IO */
static vpGetModelFormatFlags_t s_vpGetModelFormatFlags;
static vpGetModelBounds_t s_vpGetModelBounds;
static vpUnloadModel_t s_vpUnloadModel;
static vpLoadModel_t s_vpLoadModel;
static vpRenderModel_t s_vpRenderModel;

static bool Editor_RegisterModelFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpSetPalette = (vpSetPalette_t)GetProcAddress( (HMODULE)libraryHandle, "vpSetPalette" );
	/* Not checked */

	s_vpGetModelFormatFlags = (vpGetModelFormatFlags_t)GetProcAddress( (HMODULE)libraryHandle, "vpGetModelFormatFlags" );
	/* Not checked */

	s_vpGetModelBounds = (vpGetModelBounds_t)GetProcAddress( (HMODULE)libraryHandle, "vpGetModelBounds" );
	/* Not checked */

	s_vpUnloadModel = (vpUnloadModel_t)GetProcAddress( (HMODULE)libraryHandle, "vpUnloadModel" );
	/* Not checked */

	s_vpLoadModel = (vpLoadModel_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadModel" );
	if ( !s_vpLoadModel )
	{
		Sys_Error( "Plugin \"%s\" defines model format \"%s\" (%s), but doesn't export \"vpLoadModel\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpRenderModel = (vpRenderModel_t)GetProcAddress( (HMODULE)libraryHandle, "vpRenderModel" );
	if ( !s_vpRenderModel )
	{
		Sys_Error( "Plugin \"%s\" defines model format \"%s\" (%s), but doesn't export \"vpRenderModel\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


/* Particles IO */
static vpUnloadParticles_t s_vpUnloadParticles;
static vpLoadParticles_t s_vpLoadParticles;
static vpRenderParticles_t s_vpRenderParticles;

static bool Editor_RegisterParticleFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpSetPalette = (vpSetPalette_t)GetProcAddress( (HMODULE)libraryHandle, "vpSetPalette" );
	/* Not checked */

	s_vpUnloadParticles = (vpUnloadParticles_t)GetProcAddress( (HMODULE)libraryHandle, "vpUnloadParticles" );
	/* Not checked */

	s_vpLoadParticles = (vpLoadParticles_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadParticles" );
	if ( !s_vpLoadParticles )
	{
		Sys_Error( "Plugin \"%s\" defines particles format \"%s\" (%s), but doesn't export \"vpLoadParticles\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpRenderParticles = (vpRenderParticles_t)GetProcAddress( (HMODULE)libraryHandle, "vpRenderParticles" );
	if ( !s_vpRenderParticles )
	{
		Sys_Error( "Plugin \"%s\" defines particles format \"%s\" (%s), but doesn't export \"vpRenderParticles\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


/* Archive IO */
static vpUnloadArchive_t s_vpUnloadArchive;
static vpLoadArchive_t s_vpLoadArchvie;
static vpFindArchiveFile_t s_vpFindArchiveFile;
static vpLoadArchiveFile_t s_vpLoadArchiveFile;
static vpListArchiveFiles_t s_vpListArchiveFiles;

static bool Editor_RegisterArchiveFormat( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle )
{
	Sys_Printf( "  %d / %s / %s", formatIndex, formatName, formatExtension );

	s_vpUnloadArchive = (vpUnloadArchive_t)GetProcAddress( (HMODULE)libraryHandle, "vpUnloadArchive" );
	/* Not checked */

	s_vpLoadArchvie = (vpLoadArchive_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadArchive" );
	if ( !s_vpLoadArchvie )
	{
		Sys_Error( "Plugin \"%s\" defines archive format \"%s\" (%s), but doesn't export \"vpLoadArchive\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpFindArchiveFile = (vpFindArchiveFile_t)GetProcAddress( (HMODULE)libraryHandle, "vpFindArchiveFile" );
	if ( !s_vpFindArchiveFile )
	{
		Sys_Error( "Plugin \"%s\" defines archive format \"%s\" (%s), but doesn't export \"vpFindArchiveFile\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpLoadArchiveFile = (vpLoadArchiveFile_t)GetProcAddress( (HMODULE)libraryHandle, "vpLoadArchiveFile" );
	if ( !s_vpLoadArchiveFile )
	{
		Sys_Error( "Plugin \"%s\" defines archive format \"%s\" (%s), but doesn't export \"vpLoadArchiveFile\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	s_vpListArchiveFiles = (vpListArchiveFiles_t)GetProcAddress( (HMODULE)libraryHandle, "vpListArchiveFiles" );
	if ( !s_vpListArchiveFiles )
	{
		Sys_Error( "Plugin \"%s\" defines archive format \"%s\" (%s), but doesn't export \"vpListFiles\" function!", PLUGIN_DLL, formatName, formatExtension );
		return false;
	}

	return true;
}


static void Editor_RegisterAction( pluginActionDesc_t *actionInfo, void *pluginManager )
{
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	Sys_Printf( "  %s / %s / %s / %s / %x / 0x%p", actionInfo->m_actionName, actionInfo->m_actionTitle, actionInfo->m_actionDescription, actionInfo->m_actionCategory, actionInfo->m_actionFlags, actionInfo->m_dispatchFunc );
#elif JACK_API_VERSION <= 100
	Sys_Printf( "  %s / %s / %s / %d / %x / 0x%p", actionInfo->m_actionTitle, actionInfo->m_actionDescription, actionInfo->m_actionCategory, actionInfo->m_unknown, actionInfo->m_actionFlags, actionInfo->m_dispatchFunc );
#else
#error
#endif
}


static void Editor_RegisterPrimitive( const primitiveDesc_s *primDesc, void *libraryHandle )
{
	Sys_Printf( "  %s / %s / %d %d %d / %d / 0x%p", primDesc->m_primitiveName, primDesc->m_primitiveCategory, primDesc->m_numFaces, primDesc->m_minFaces, primDesc->m_maxFaces, primDesc->m_flags, primDesc->pfnDispatchFunc );
}


static void Editor_RegisterFlags( const char *pluginName, const char *surfaceFlagName, unsigned int bitIndex, void *libraryHandle )
{
	Sys_Printf( "  %s %s %d", pluginName, surfaceFlagName, bitIndex );
}


static qTexture_t *s_whiteTexture;
static void CreateWhiteTexture()
{
	if ( !s_whiteTexture )
	{
		//char buf[256];
		//memset( buf, 0xFF, sizeof( buf ) );

		s_whiteTexture = (qTexture_t *)Sys_Malloc( sizeof( qTexture_t ) );
		s_whiteTexture->m_flags = 2;
		s_whiteTexture->m_width = 8;
		s_whiteTexture->m_height = 8;
		s_whiteTexture->m_numChannels = 4;
		s_whiteTexture->m_glPixelFormat = GL_RGBA;
		s_whiteTexture->m_glTextureFormat = GL_RGBA;
	}
}

static void DestroyWhiteTexture()
{
	if ( s_whiteTexture )
	{
		Sys_Free( s_whiteTexture );
	}
}

static void RunPluginTests()
{
	CreateWhiteTexture();

	/* Sky IO */
	if ( 1 && s_vpLoadSky )
	{
		int numBytes = 0;
		byte *buf = Sys_LoadFile( "valve/gfx/env/cityft.tga", &numBytes );

		qShader_t *skyShader = (qShader_t *)Sys_Malloc( sizeof( qShader_t ) );
		memset( skyShader, 0, sizeof( qShader_t ) );

		skyShader->m_refCount = 1;
		skyShader->m_framerate = 1.f;
		skyShader->m_materialType = 512; // TODO: bitmask

		strncpy( skyShader->m_name, "city", sizeof( skyShader->m_name ) );
		skyShader->m_name[sizeof( skyShader->m_name ) - 1] = '\0';

		bool bOK = s_vpLoadSky( 0, buf, numBytes, skyShader, 0 );
		if ( bOK )
		{
			Sys_Printf( "vpLoadSky: %d", bOK );
		}

		Sys_Free( skyShader );
	}

	/* Package IO */
	if ( 0 && s_vpGetPackageInfo )
	{
		int numMipTex;
		bool bOK = s_vpGetPackageInfo( 0, "quake106/ID1/quake.wad", &numMipTex );
		if ( bOK )
		{
			Sys_Printf( "  quake.wad: %d", numMipTex );
		}
	}

	/* Sprite IO */
	if ( 1 && s_vpLoadSprite )
	{
		byte *buf = Sys_LoadFile( "valve/sprites/explode1.spr", NULL );

		qSpriteData_t spriteData;
		memset( &spriteData, 0, sizeof( spriteData ) );

		bool bOK = s_vpLoadSprite( 0, "myshader", buf, 0, &spriteData );
		if ( bOK )
		{
			Sys_Printf( "vpLoadSprite: %d", bOK );
		}

		Sys_Free( buf );
	}

	/* Model IO */
	if ( 1 && s_vpLoadModel && s_vpUnloadModel )
	{
		byte *buf = Sys_LoadFile( "valve/player.mdl", NULL );

		qStudioData_t modelData;
		memset( &modelData, 0, sizeof( modelData ) );

		bool bOK = s_vpLoadModel( 0, "valve/player.mdl", buf, 0, &modelData );
		if ( bOK )
		{
			Sys_Printf( "vpLoadModel: %d", bOK );

			s_vpUnloadModel( 0, &modelData );
		}

		Sys_Free( buf );
	}

	/* Archive IO */
	if ( 1 && s_vpLoadArchvie && s_vpFindArchiveFile && s_vpLoadArchiveFile && s_vpListArchiveFiles && s_vpUnloadArchive )
	{
		qArchiveData_t archiveData;
		memset( &archiveData, 0, sizeof( archiveData ) );

		bool bOK = s_vpLoadArchvie( 0, "quake106/ID1/PAK0.PAK", &archiveData );
		if ( bOK )
		{
			bOK = s_vpFindArchiveFile( 0, &archiveData, "default.cfg" );
			if ( bOK )
			{
				char *buf = NULL;
				int size = s_vpLoadArchiveFile( 0, &archiveData, "default.cfg", &buf );

				Sys_Printf( "vpLoadArchiveFile: %d %s", size, buf );

				Sys_Free( buf );
			}

			{
				char **fileList = NULL;
				bOK = s_vpListArchiveFiles( 0, &archiveData, "*.bsp", &fileList );
				if ( bOK )
				{
					for ( int i = 0; fileList[i] != NULL; i++ )
					{
						Sys_Printf( "  %s", fileList[i] );
					}

					for ( int i = 0; fileList[i] != NULL; i++ )
					{
						Sys_Free( fileList[i] );
					}
					Sys_Free( fileList );
				}
			}

			s_vpUnloadArchive( 0, &archiveData );
		}
	}

	DestroyWhiteTexture();
}

#if defined( WIN32 )
int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
#else
int main( int argc, char **argv )
#endif // WIN32
{
	InitializeEditorFuncs();

#if !defined( WIN32 )
	void *hInstance = NULL;
#endif // !WIN32

	char moduleName[MAX_PATH];
	if ( !GetModuleFileName( hInstance, moduleName, MAX_PATH ) )
	{
		return 0;
	}

	char *pRootDir = GetBaseDir( moduleName );

	char szPluginPath[MAX_PATH];
	snprintf( szPluginPath, sizeof( szPluginPath ), "%s" _PATHSEP PLUGIN_DLL, pRootDir );
	szPluginPath[sizeof( szPluginPath ) - 1] = '\0';

	HMODULE hPluginModule = LoadLibraryEx( szPluginPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
	if ( !hPluginModule )
	{
		char *pszError;
#if defined( WIN32 )
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPTSTR)&pszError, 0, NULL );
#else
		pszError = (char *)dlerror();
#endif

		Sys_Error( "%s", pszError );

#if defined( WIN32 )
		LocalFree( pszError );
#endif // WIN32
		return 0;
	}

	vpMain_t pluginMain = (vpMain_t)GetProcAddress( hPluginModule, "vpMain" );
	if ( !pluginMain )
	{
		Sys_Error( "Couldn't find \"vpMain\" export in \"%s\"", szPluginPath );
		return 0;
	}

	int ret = pluginMain( &gEditorfuncs, PLUGIN_VERSION );
	if ( ret == -1 )
	{
		Sys_Error( "Plugin \"%s\" has wrong interface size", szPluginPath );
		return 0;
	}
	if ( ret > 0 )
	{
		Sys_Error( "Plugin \"%s\" has wrong version number (%i should be %i)", szPluginPath, ret, PLUGIN_VERSION );
		return 0;
	}

	vpEnumProfiles_t pluginEnumProfiles = (vpEnumProfiles_t)GetProcAddress( hPluginModule, "vpEnumProfiles" );
	if ( pluginEnumProfiles )
	{
		pluginEnumProfiles( Editor_RegisterProfile, hPluginModule );
	}

	vpEnumGenericFunction_t pluginEnumExportFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumExportFormats" );
	if ( pluginEnumExportFormats )
	{
		int ret = pluginEnumExportFormats( Editor_RegisterIOFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i map export format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumImportFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumImportFormats" );
	if ( pluginEnumImportFormats )
	{
		int ret = pluginEnumImportFormats( Editor_RegisterIOFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i map import format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumTextureFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumTextureFormats" );
	if ( pluginEnumTextureFormats )
	{
		int ret = pluginEnumTextureFormats( Editor_RegisterTextureFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i texture format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumPackageFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumPackageFormats" );
	if ( pluginEnumPackageFormats )
	{
		int ret = pluginEnumPackageFormats( Editor_RegisterPackageFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i package format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumSkyFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumSkyFormats" );
	if ( pluginEnumSkyFormats )
	{
		int ret = pluginEnumSkyFormats( Editor_RegisterSkyFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i sky format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumSpriteFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumSpriteFormats" );
	if ( pluginEnumSpriteFormats )
	{
		int ret = pluginEnumSpriteFormats( Editor_RegisterSpriteFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i sprite format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumModelFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumModelFormats" );
	if ( pluginEnumModelFormats )
	{
		int ret = pluginEnumModelFormats( Editor_RegisterModelFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i model format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumParticlesFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumParticlesFormats" );
	if ( pluginEnumParticlesFormats )
	{
		int ret = pluginEnumParticlesFormats( Editor_RegisterParticleFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i sprite format(s) registered", ret );
	}

	vpEnumGenericFunction_t pluginEnumArchiveFormats = (vpEnumGenericFunction_t)GetProcAddress( hPluginModule, "vpEnumArchiveFormats" );
	if ( pluginEnumArchiveFormats )
	{
		int ret = pluginEnumArchiveFormats( Editor_RegisterArchiveFormat, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i archive format(s) registered", ret );
	}

	vpEnumActions_t pluginEnumActions = (vpEnumActions_t)GetProcAddress( hPluginModule, "vpEnumActions" );
	if ( pluginEnumActions )
	{
		int ret = pluginEnumActions( Editor_RegisterAction, hPluginModule ); /* Editor does not check the return value */
		if ( ret != 0 )
			Sys_Printf( "%i plugin action(s) registered", ret );
	}

	vpEnumPrimitives_t pluginEnumPrimitives = (vpEnumPrimitives_t)GetProcAddress( hPluginModule, "vpEnumPrimitives" );
	if ( pluginEnumPrimitives )
	{
		int ret = pluginEnumPrimitives( Editor_RegisterPrimitive, hPluginModule );
		if ( ret != 0 )
			Sys_Printf( "%i plugin primitive(s) registered", ret );
	}

	vpEnumGenericFlags_t pluginEnumSurfaceFlags = (vpEnumGenericFlags_t)GetProcAddress( hPluginModule, "vpEnumSurfaceFlags" );
	if ( pluginEnumSurfaceFlags )
	{
		int ret = pluginEnumSurfaceFlags( Editor_RegisterFlags, hPluginModule ); /* Editor does not check the return value */
		if ( ret != 0 )
			Sys_Printf( "%i plugin surface flag(s) registered", ret );
	}

	vpEnumGenericFlags_t pluginEnumContentFlags = (vpEnumGenericFlags_t)GetProcAddress( hPluginModule, "vpEnumContentFlags" );
	if ( pluginEnumContentFlags )
	{
		int ret = pluginEnumContentFlags( Editor_RegisterFlags, hPluginModule ); /* Editor does not check the return value */
		if ( ret != 0 )
			Sys_Printf( "%i plugin content flag(s) registered", ret );
	}

	RunPluginTests();

	return 0;
}
