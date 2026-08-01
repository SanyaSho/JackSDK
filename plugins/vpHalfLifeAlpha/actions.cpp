//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#include "PluginMeta.h"

#include "engine/bspfile.h"
#include "engine/wad.h"

#include "vstdlib_static.h"

#define Align4( x ) ((x + 3) & ~3)

static int s_numbOfFiles = 0;
static int s_currentFile = 0;

static int s_numbOfTextures = 0;

struct textureList_t
{
	char name[16];
	int width;
	int height;

	int imgsize;
	short palsize;
	byte *imgdata;
	byte *paldata;

	textureList_t *next;
};

textureList_t *g_textureList = NULL;


/*
===============
LoadBSPTextures
===============
*/
static void LoadBSPTextures( FILE *f )
{
	dheader_t bspHeader;

	if ( fread( &bspHeader, 1, sizeof( dheader_t ), f ) != sizeof( dheader_t ) )
	{
		Dialog_Printf( "Error reading BSP file header" );
		return;
	}

	if ( bspHeader.version != BSPVERSION )
	{
		Dialog_Printf( "Bad BSP file version (%i should be %i)", bspHeader.version, BSPVERSION );
		return;
	}

	const lump_t &texturesLump = bspHeader.lumps[LUMP_TEXTURES];

	if ( texturesLump.filelen <= 0 )
	{
		Dialog_Printf( "BSP file contains no textures" );
		return;
	}

	// Why?
	fseek( f, texturesLump.fileofs - sizeof( dheader_t ), SEEK_CUR );

	int textureLumpStart = ftell( f );

	int numTextures = 0;
	if ( fread( &numTextures, 1, sizeof( int ), f ) != sizeof( int ) )
	{
		Dialog_Printf( "Error reading BSP file" );
		return;
	}

	if ( numTextures <= 0 )
	{
		Dialog_Printf( "BSP file contains no textures" );
		return;
	}

	int *offsets = (int *)Sys_Malloc( numTextures * sizeof( int ) );
	if ( fread( offsets, 1, numTextures * sizeof( int ), f ) != numTextures * sizeof( int ) )
	{
		Dialog_Printf( "Error reading BSP file textures" );
		// LEAK
		return;
	}

	for ( int i = 0; i < numTextures; i++ )
	{
		if ( offsets[i] < 0 )
			continue;

		fseek( f, textureLumpStart + offsets[i], SEEK_SET );

		miptex_t mipTex;

		if ( fread( &mipTex, 1, sizeof( miptex_t ), f ) != sizeof( miptex_t ) )
		{
			Dialog_Printf( "Error reading file" );
			continue;
		}

		if ( mipTex.offsets[0] == 0 )
			continue;

		if ( ( mipTex.width & 15 ) || ( mipTex.height & 15 ) )
		{
			Dialog_Printf( "Texture '%s' is not 16 aligned", mipTex.name );
			continue;
		}

		mipTex.name[sizeof( mipTex.name ) - 1] = '\0';

		bool duplicate = false;

		for ( textureList_t *tex = g_textureList; tex != NULL; tex = tex->next )
		{
			if ( !stricmp( mipTex.name, tex->name ) )
			{
				duplicate = true;
				break;
			}
		}

		if ( duplicate )
			continue;

		textureList_t *texList = (textureList_t *)Sys_Malloc( sizeof( textureList_t ) );
		if ( !texList )
		{
			Dialog_Printf( "Memory allocation failure" );
			continue;
		}

		memset( texList, 0, sizeof( textureList_t ) );

		strncpy( texList->name, mipTex.name, sizeof( texList->name ) );

		texList->width = mipTex.width;
		texList->height = mipTex.height;

		int pixels = texList->width * texList->height;
		texList->imgsize =
			pixels + pixels / 4 + pixels / 16 + pixels / 64;

		texList->imgdata = (byte *)Sys_Malloc( texList->imgsize );
		if ( !texList->imgdata )
		{
			Dialog_Printf( "Memory allocation failure" );
			continue;
		}

		// Normally image data starts right after miptex_t
		if ( mipTex.offsets[0] != sizeof( miptex_t ) )
		{
			fseek( f, textureLumpStart + offsets[i] + mipTex.offsets[0], SEEK_SET );
		}

		if ( fread( texList->imgdata, 1, texList->imgsize, f ) != texList->imgsize || fread( &texList->palsize, 1, sizeof( short ), f ) != sizeof( short ) )
		{
			Dialog_Printf( "Error reading file" );

			Sys_Free( texList->imgdata );
			Sys_Free( texList );
			continue;
		}

		if ( texList->palsize != 256 )
		{
			Dialog_Printf( "WARNING: strange palette size (%i should be 256)", texList->palsize );
		}

		texList->paldata = (byte *)Sys_Malloc( texList->palsize * 3 );
		if ( !texList->paldata )
		{
			Dialog_Printf( "Memory allocation failure" );

			Sys_Free( texList->imgdata );
			Sys_Free( texList );
			continue;
		}

		if ( fread( texList->paldata, 1, texList->palsize * 3, f ) != texList->palsize * 3 )
		{
			Dialog_Printf( "Error reading file" );

			Sys_Free( texList->paldata );
			Sys_Free( texList->imgdata );
			Sys_Free( texList );
			continue;
		}

		s_numbOfTextures++;

		texList->next = g_textureList;
		g_textureList = texList;
	}
}

/*
===============
WriteWAD
===============
*/
static void WriteWAD( const char *filePath )
{
	Dialog_Printf( "Writing: \"%s\"", filePath );

	const char *localFilePath = Sys_MakeLocalFileName( filePath );

	FILE *f = fopen( localFilePath, "wb" );
	if ( !f )
		return;

	//
	// Write WAD3 header
	//

	wadinfo_t wadHeader;
	wadHeader.identification = IDWADHEADER;
	wadHeader.numlumps = s_numbOfTextures;
	wadHeader.infotableofs = sizeof( wadinfo_t );

	fwrite( &wadHeader, sizeof( wadinfo_t ), 1, f );

	//
	// Write lump directory
	//

	int filePos = wadHeader.infotableofs + wadHeader.numlumps * sizeof( lumpinfo_t );

	for ( textureList_t *tex = g_textureList; tex != NULL; tex = tex->next )
	{
		lumpinfo_t textureLump;
		memset( &textureLump, 0, sizeof( lumpinfo_t ) );

		textureLump.filepos = filePos;
		textureLump.disksize = textureLump.size = sizeof( miptex_t ) + Align4( tex->imgsize + ( 2 + ( tex->palsize * 3 ) ) );
		textureLump.type = TYP_MIPTEX;

		strncpy( textureLump.name, tex->name, 16 );

		fwrite( &textureLump, sizeof( lumpinfo_t ), 1, f );

		filePos += textureLump.disksize;
	}

	//
	// Write textures
	//

	for ( textureList_t *tex = g_textureList; tex != NULL; tex = tex->next )
	{
		miptex_t mipTex;
		memset( &mipTex, 0, sizeof( miptex_t ) );

		strncpy( mipTex.name, tex->name, 16 );

		mipTex.width = tex->width;
		mipTex.height = tex->height;

		int pixels = tex->width * tex->height;
		mipTex.offsets[0] = sizeof( miptex_t );
		mipTex.offsets[1] = mipTex.offsets[0] + pixels;
		mipTex.offsets[2] = mipTex.offsets[1] + pixels / 4;
		mipTex.offsets[3] = mipTex.offsets[2] + pixels / 16;

		fwrite( &mipTex, sizeof( miptex_t ), 1, f );

		fwrite( tex->imgdata, tex->imgsize, 1, f );

		fwrite( &tex->palsize, sizeof( short ), 1, f );
		fwrite( tex->paldata, tex->palsize * 3, 1, f );

		int written = tex->imgsize + 2 + tex->palsize * 3;
		int aligned = Align4( written );

		static const byte zeros[4] = {};
		fwrite( zeros, aligned - written, 1, f );
	}

	fflush( f );

	//
	// Cleanup
	//

	fclose( f );

	Dialog_Printf( "%i textures extracted", s_numbOfTextures );

	// "Unpacker" achievement
	Steam_SetAchievemnt( 30 );
}

/*
===============
PluginAction_ExtractTextures_Command
===============
*/
static void PluginAction_ExtractTextures_Command()
{
	Dialog_Printf( "Extracting Half-Life textures..." );

	float startTime = Sys_FloatTime();

	char fileList[8192] = { 0 };
	Dialog_QueryArgument( "src", fileList, sizeof( fileList ) );

	char wadOut[MAX_PATH] = { 0 };
	Dialog_QueryArgument( "wad", wadOut, sizeof( wadOut ) );

	s_numbOfFiles = 0;

	//
	// Count the files
	//
	{
		SC_ParseFromMemory( fileList, strlen( fileList ), 0 );

		while ( SC_GetToken( true ) )
		{
			char fileExt[32] = { 0 };

			const char *token = SC_Token();
			V_ExtractFileExtension( token, fileExt, sizeof( fileExt ) );

			if ( !stricmp( fileExt, ".bsp" ) )
			{
				s_numbOfFiles++;
			}
		}

		if ( s_numbOfFiles == 0 )
		{
			Dialog_Printf( "Noting to extract!" );
			return;
		}
	}

	//
	// Extract textures
	//
	{
		SC_ParseFromMemory( fileList, strlen( fileList ), 0 );

		while ( SC_GetToken( true ) )
		{
			char fileExt[32] = { 0 };

			const char *token = SC_Token();
			V_ExtractFileExtension( token, fileExt, sizeof( fileExt ) );

			if ( !stricmp( fileExt, ".bsp" ) )
			{
				Dialog_Printf( "Loading: \"%s\"", token );

				const char *localFilePath = Sys_MakeLocalFileName( token );

				FILE *f = fopen( localFilePath, "rb" );
				if ( f )
				{
					LoadBSPTextures( f );
					fclose( f );

					Dialog_SetProgress( ++s_currentFile, s_numbOfFiles );
				}
			}
		}

		if ( s_numbOfTextures == 0 )
		{
			Dialog_Printf( "Noting to extract!" );
			return;
		}

		WriteWAD( wadOut );
	}

	//
	// Cleanup
	//

	if ( g_textureList != NULL )
	{
		for ( textureList_t *tex = g_textureList; tex != NULL; )
		{
			textureList_t *next = tex->next;

			Sys_Free( tex->paldata );
			Sys_Free( tex->imgdata );
			Sys_Free( tex );

			tex = next;
		}
	}
	g_textureList = NULL;

	s_numbOfTextures = 0;

	float endTime = Sys_FloatTime();
	Dialog_Printf( "%.2f seconds elapsed", (endTime - startTime) );
}

/*
===============
PluginAction_ExtractTextures
===============
*/
static void PluginAction_ExtractTextures( int state )
{
	Dialog_Begin( "Extract Half-Life Textures" );
	Dialog_InitInternalCommand( "E&xtract", PluginAction_ExtractTextures_Command );
	Dialog_AddFileList( "src", "Source Files", NULL, "BSP Files (*.bsp)", DIALOG_FILE_OPEN | DIALOG_FILE_CONNECT );
	Dialog_AddFileEdit( "wad", "Output Package", "extracted.wad", "WAD3 Texture Package (*.wad)", DIALOG_FILE_OPEN );
	Dialog_End();
}

pluginActionDesc_t g_actionExtractTextures = {
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"ExtractTextures",
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	"&Extract textures...",
	"Extract Half-Life textures from BSP...",
	"Half-Life Alpha",
#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	0,
	0,
	PluginAction_ExtractTextures
};

/*
===============
vpEnumActions
Interactive menu actions
===============
*/
DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
{
	registerAction( &g_actionExtractTextures, pluginManager );
	return 1;
}
