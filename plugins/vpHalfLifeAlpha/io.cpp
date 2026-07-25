//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <algorithm>

// Plugin API
#include "PluginMeta.h"

#include "gamma.h"

#include "DataSerializer.h"
#include "serializer_map.h"
#include "serializer_rmf.h"

#include "engine/bspfile.h"
#include "engine/wad.h"

// Sprite
#include "engine/gl_model.h"

// StudioRender
#include "engine/r_studio.h"

#include "vstdlib_static.h"

//#define EXTRACT_TO_PNG_ON_LOAD

#if defined( EXTRACT_TO_PNG_ON_LOAD )
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif // EXTRACT_TO_PNG_ON_LOAD

// clang-format off
struct formatList_t
{
	int m_formatIndex;
	const char *m_formatDesctiption;
	const char *m_formatExtension;
	bool (*m_pfnFunction)( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
};
// clang-format on

// clang-format off
static formatList_t s_exportList[] =
{
	{ 0, "Half-Life Alpha 0.52 MAP", ".map", ExportMAP },
};
// clang-format on

/*
===============
vpEnumExportFormats
===============
*/
DLL_EXPORT int vpEnumExportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	int c = 0;

	for ( int i = 0; i < ARRAYSIZE( s_exportList ); i++ )
	{
		c += registerIOFormat( s_exportList[i].m_formatIndex, s_exportList[i].m_formatDesctiption, s_exportList[i].m_formatExtension, libraryHandle ) ? 1 : 0;
	}

	return c;
}

/*
===============
vpExport
===============
*/
DLL_EXPORT int vpExport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
{
	for ( int i = 0; i < ARRAYSIZE( s_exportList ); i++ )
	{
		if ( s_exportList[i].m_formatIndex == formatIndex )
		{
			return s_exportList[i].m_pfnFunction( filePath, seekOffset, readLimit, worldDef );
		}
	}

	return 0;
}

// clang-format off
static formatList_t s_importList[] =
{
	{ 0, "Half-Life Alpha 0.52 MAP", ".map", ImportMAP },
};
// clang-format on

/*
===============
vpEnumImportFormats
===============
*/
DLL_EXPORT int vpEnumImportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	int c = 0;

	for ( int i = 0; i < ARRAYSIZE( s_importList ); i++ )
	{
		c += registerIOFormat( s_importList[i].m_formatIndex, s_importList[i].m_formatDesctiption, s_importList[i].m_formatExtension, libraryHandle ) ? 1 : 0;
	}

	return c;
}

/*
===============
vpImport
===============
*/
DLL_EXPORT int vpImport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
{
	for ( int i = 0; i < ARRAYSIZE( s_importList ); i++ )
	{
		if ( s_importList[i].m_formatIndex == formatIndex )
		{
			return s_importList[i].m_pfnFunction( filePath, seekOffset, readLimit, worldDef );
		}
	}

	return 0;
}

/*
===============
vpGetPackageInfo
===============
*/
DLL_EXPORT bool vpGetPackageInfo( int formatIndex, const char *filePath, int *numMipTex )
{
	if ( formatIndex != 0 )
		return false;

	const char *localFilePath = Sys_MakeLocalFileName( filePath );

	FILE *f = fopen( localFilePath, "rb" );
	if ( !f )
		return false;

	wadinfo_t wadHeader;
	if ( fread( &wadHeader, 1, sizeof( wadHeader ), f ) != sizeof( wadHeader ) )
	{
		fclose( f );
		return false;
	}

	if ( wadHeader.identification != IDWADHEADER || wadHeader.numlumps <= 0 )
	{
		fclose( f );
		return false;
	}

	// Move to the infotable
	fseek( f, wadHeader.infotableofs, SEEK_SET );

	int mipTexCount = 0;

	for ( int i = 0; i < wadHeader.numlumps; i++ )
	{
		lumpinfo_t wadLump;
		if ( fread( &wadLump, 1, sizeof( lumpinfo_t ), f ) != sizeof( lumpinfo_t ) )
			break;

		if ( wadLump.type == TYP_MIPTEX )
			mipTexCount++;
	}

	*numMipTex = mipTexCount;
	fclose( f );

	return true;
}

/*
===============
BuildAnimatedTextureChains
===============
*/
void BuildAnimatedTextureChains( qShader_t *shaderList, const char *filePath )
{
	//
	// Build an animation chain
	//
	for ( qShader_t *shaderDef = shaderList; shaderDef != NULL; shaderDef = shaderDef->next )
	{
		qShaderStage_t *shaderStage = shaderDef->m_stage;

		if ( !shaderStage || shaderDef->m_name[0] != '+' )
			continue;

		char texName[16] = { 0 };
		strncpy( texName, shaderDef->m_name, sizeof( texName ) );
		texName[sizeof( texName ) - 1] = '\0';

		// Increment the texture chain idx
		++texName[1];

		if ( !shaderStage->m_textureList )
		{
			shaderStage->m_textureList = shaderStage->m_currentTexture;
		}

		char shaderHashName[512] = { 0 };
		strncat( shaderHashName, filePath, sizeof( shaderHashName ) );
		strncat( shaderHashName, ":", sizeof( shaderHashName ) );
		strncat( shaderHashName, texName, sizeof( shaderHashName ) );
		shaderStage->m_textureList->next = Shader_LookupTexture( shaderHashName );

		if ( !shaderStage->m_textureList->next )
		{
			strncpy( texName, shaderDef->m_name, sizeof( texName ) );
			texName[sizeof( texName ) - 1] = '\0';

			char c = shaderDef->m_name[1];
			if ( c >= '0' && c <= '9' )
			{
				texName[1] = '0';
			}
			else if ( c >= 'A' && c <= 'Z' )
			{
				texName[1] = 'A';
			}

			memset( shaderHashName, 0, sizeof( shaderHashName ) );
			strncat( shaderHashName, filePath, sizeof( shaderHashName ) );
			strncat( shaderHashName, ":", sizeof( shaderHashName ) );
			strncat( shaderHashName, texName, sizeof( shaderHashName ) );
			shaderStage->m_textureList->next = Shader_LookupTexture( shaderHashName );
		}

		if ( shaderStage->m_textureList->next == shaderStage->m_textureList )
			shaderStage->m_textureList->next = NULL;
	}

	//
	// Check for broken animation chain
	//
	for ( qShader_t *shaderDef = shaderList; shaderDef != NULL; shaderDef = shaderDef->next )
	{
		qShaderStage_t *shaderStage = shaderDef->m_stage;
		if ( !shaderStage )
			continue;

		qTexture_t *textureList = shaderStage->m_textureList;
		if ( !textureList )
			continue;

		int texcnt = 100;

		qTexture_t *textureDef = textureList->next;
		while ( textureDef != NULL && textureDef != textureList )
		{
			textureDef = textureDef->next;

			if ( --texcnt == 0 )
			{
				SetBits( shaderDef->m_flags, SHADER_FLAG_BROKEN_ANIMATION );
				break;
			}
		}
	}

	//
	// Inform the user about an error
	//
	for ( qShader_t *shaderDef = shaderList; shaderDef != NULL; shaderDef = shaderDef->next )
	{
		if ( FBitSet( shaderDef->m_flags, SHADER_FLAG_BROKEN_ANIMATION ) )
		{
			Sys_Warning( "broken animation chain in \"%s\"\n", shaderDef->m_name );
			ClearBits( shaderDef->m_flags, SHADER_FLAG_BROKEN_ANIMATION );
			shaderDef->m_stage->m_textureList = NULL;
		}
	}
}

// clang-format off
struct textureTranslucencyList_t
{
	const char *name; /* Not NULL-safe */
	float translucency;
} g_textureTranslucencyList[] =
{
	{ "AAATRIGGER", 0.5 },
	{ "CLIP", 0.5 },
	{ "ORIGIN", 0.5 },
	{ "HINT", 0.5 },
	{ "SKIP", 0.5 },
	{ "BEVEL", 0.5 },
	{ "TRIGGER", 0.5 }
};
// clang-format on

/*
===============
PrecacheWADTextures
===============
*/
static void PrecacheWADTextures( FILE *f, wadinfo_t &wadHeader, const char *filePath )
{
	BuildGammaTable();

	qShader_t *shaderList = NULL;
	qShader_t *lastShader = NULL;

	char wadBaseName[MAX_PATH] = { 0 };
	V_ExtractFileBase( filePath, wadBaseName, sizeof( filePath ) );

	bool hasAnimatedTextures = false;
	bool isDecalsWad = ( stricmp( wadBaseName, "decals" ) == 0 );

	qShaderStage_t shaderStage;
	memset( &shaderStage, 0, sizeof( qShaderStage_t ) );
	memset( &shaderStage.m_data, 0, sizeof( qShaderStageData_t ) );

	shaderStage.m_framerate = 10.f;

	int textureLumpOffset = wadHeader.infotableofs;

	for ( int i = 0; i < wadHeader.numlumps; i++ )
	{
		fseek( f, textureLumpOffset, SEEK_SET );

		lumpinfo_t textureLump;
		memset( &textureLump, 0, sizeof( lumpinfo_t ) );

		if ( fread( &textureLump, 1, sizeof( lumpinfo_t ), f ) != sizeof( lumpinfo_t ) )
			break;

		// Move to the next lump immediately
		textureLumpOffset += sizeof( lumpinfo_t );

		if ( textureLump.type != TYP_MIPTEX )
			continue;

		// Uppercase the texture name
		V_Strupr( textureLump.name );

		//
		// Read texture lump
		//
		miptex_t *texlump = (miptex_t *)TempBuffer_GetSpace( 1, textureLump.disksize );

		fseek( f, textureLump.filepos, SEEK_SET );

		if ( fread( texlump, 1, textureLump.disksize, f ) != textureLump.disksize )
			break;

		int pixels = texlump->width * texlump->height;
		byte *imgdata = (byte *)TempBuffer_GetSpace( 0, pixels * 4 ); // RGBA

		//
		// Get the image data
		//
		byte *texdata = (byte *)texlump + texlump->offsets[0];

		//byte *paldata = (byte *)texlump + texlump->offsets[0] + (((texlump->width * texlump->height) * 85)>>6);
		//short palsize = *(short *)paldata;
		//paldata += sizeof( short );

		int paloffset = ( ( texlump->width / 8 ) * ( texlump->height / 8 ) ) + texlump->offsets[3];
		short palsize = *(short *)((byte *)texlump + paloffset);
		byte *paldata = (byte *)texlump + paloffset + 2;

		//
		// Gamma correct palette
		//
		for ( int j = 0; j < palsize; j++ )
		{
			paldata[j*3+0] = g_gammatable[paldata[j*3+0]];
			paldata[j*3+1] = g_gammatable[paldata[j*3+1]];
			paldata[j*3+2] = g_gammatable[paldata[j*3+2]];
		}

		//
		// Generate image preview
		//

		/*
		 0 - not a decal
		 1 - decal from an external storage
		 2 - decal from decals wad
		*/
		int decalType = 0;
		int numChannels = 0;
		unsigned int textureFormat = 0;
		unsigned int pixelFormat = 0;

		// Decals have special parameters
		if ( textureLump.name[0] == '{' )
		{
			if ( isDecalsWad )
			{
				byte lastR = paldata[(palsize - 1)*3+0];
				byte lastG = paldata[(palsize - 1)*3+1];
				byte lastB = paldata[(palsize - 1)*3+2];

				for ( int j = 0; j < pixels; j++ )
				{
					int p = texdata[j];

					if ( p == 0 )
					{
						imgdata[j*3+0] = paldata[0];
						imgdata[j*3+1] = paldata[1];
						imgdata[j*3+2] = paldata[2];
					}
					else
					{
						float t = p / 255.f;

						imgdata[j*3+0] = paldata[p*3+0] + (lastR - paldata[p*3+0]) * t;
						imgdata[j*3+1] = paldata[p*3+1] + (lastG - paldata[p*3+1]) * t;
						imgdata[j*3+2] = paldata[p*3+2] + (lastB - paldata[p*3+2]) * t;
					}
				}

				decalType = 2;

				numChannels = 3;
				textureFormat = GL_COMPRESSED_RGB;
				pixelFormat = GL_RGB;
			}
			else
			{
				for ( int j = 0; j < pixels; j++ )
				{
					int p = texdata[j];

					imgdata[j*4+0] = paldata[p*3+0];
					imgdata[j*4+1] = paldata[p*3+1];
					imgdata[j*4+2] = paldata[p*3+2];
					imgdata[j*4+3] = ( p == ( palsize - 1 ) ) ? 0 : 255;
				}

				decalType = 1;

				numChannels = 4;
				textureFormat = GL_COMPRESSED_RGBA;
				pixelFormat = GL_RGBA;
			}
		}
		else
		{
			for ( int j = 0; j < pixels; j++ )
			{
				int p = texdata[j];

				imgdata[j*3+0] = paldata[p*3+0];
				imgdata[j*3+1] = paldata[p*3+1];
				imgdata[j*3+2] = paldata[p*3+2];
			}

			decalType = 0;

			numChannels = 3;
			textureFormat = GL_COMPRESSED_RGB;
			pixelFormat = GL_RGB;
		}

		//
		// Prepare the shader
		//
		char shaderHashName[512] = { 0 };
		strncat( shaderHashName, filePath, sizeof( shaderHashName ) );
		strncat( shaderHashName, ":", sizeof( shaderHashName ) );
		strncat( shaderHashName, textureLump.name, sizeof( shaderHashName ) );

		qShader_t *shaderDef = Shader_Create( shaderHashName, textureLump.name, 0 );
		if ( shaderDef && !shaderDef->m_stage )
		{
			if ( textureLump.name[0] == '+' )
			{
				hasAnimatedTextures = true;
			}

			shaderDef->unknownInt1 = 16;

			shaderDef->m_value = 0;

			if ( textureLump.name[0] == '!' )
			{
				SetBits( shaderDef->m_materialType, MAT_LIQUID );
			}
			else if ( !strcmp( textureLump.name, "NULL" ) || !strcmp( textureLump.name, "NOCLIP" ) || !strcmp( textureLump.name, "CONTENTEMPTY" ) || !strcmp( textureLump.name, "CONTENTWATER" ) )
			{
				SetBits( shaderDef->m_materialType, MAT_UTILITY | MAT_NULL );
			}
			else if ( !strcmp( textureLump.name, "SKIP" ) || !strcmp( textureLump.name, "HINT" ) || !strcmp( textureLump.name, "SOLIDHINT" ) )
			{
				SetBits( shaderDef->m_materialType, MAT_UTILITY | MAT_HINT );
			}
			else if ( !strcmp( textureLump.name, "CLIP" ) || !strcmp( textureLump.name, "BEVEL" ) || !strcmp( textureLump.name, "CLIPHULL1" ) || !strcmp( textureLump.name, "CLIPHULL2" ) || !strcmp( textureLump.name, "CLIPHULL3" ) )
			{
				SetBits( shaderDef->m_materialType, MAT_UTILITY | MAT_CLIP );
			}
			else if ( !strcmp( textureLump.name, "AAATRIGGER" ) || !strcmp( textureLump.name, "TRIGGER" ) )
			{
				SetBits( shaderDef->m_materialType, MAT_UTILITY | MAT_TRIGGER );
			}
			else if ( !strcmp( textureLump.name, "SKY" ) )
			{
				SetBits( shaderDef->m_materialType, MAT_UTILITY | MAT_SKY );
			}

#if defined( EXTRACT_TO_PNG_ON_LOAD )
			char name[64] = { 0 };
			strncat( name, "wad/", 4 );
			strncat( name, texlump->name, 16 );
			strncat( name, ".png", 4 );
			stbi_write_png( name, texlump->width, texlump->height, numChannels, imgdata, 0 );
#endif // !EXTRACT_TO_PNG_ON_LOAD

			qTexture_t *textureDef = Shader_UploadTexture( shaderDef, shaderHashName, pixelFormat, textureFormat, numChannels, texlump->width, texlump->height, true, imgdata );
			shaderDef->m_texture = textureDef;

			memset( &shaderStage.m_data.m_subData, 0, sizeof( qShaderStageSubData_t ) );

			//
			// Generate custom decal inworld image
			//
			if ( decalType != 0 )
			{
				byte *decaldata = (byte *)TempBuffer_GetSpace( 0, pixels * 4 );

				switch ( decalType )
				{
				case 1:
				{
					for ( int j = 0; j < pixels; j++ )
					{
						int p = texdata[j];

						if ( p == ( palsize - 1 ) )
						{
							decaldata[j*4+0] = 0;
							decaldata[j*4+1] = 0;
							decaldata[j*4+2] = 0;
							decaldata[j*4+3] = 0;
						}
						else
						{
							decaldata[j*4+0] = paldata[p*3+0];
							decaldata[j*4+1] = paldata[p*3+1];
							decaldata[j*4+2] = paldata[p*3+2];
							decaldata[j*4+3] = 255;
						}
					}
					break;
				}
				case 2:
				{
					byte lastR = paldata[(palsize - 1)*3+0];
					byte lastG = paldata[(palsize - 1)*3+1];
					byte lastB = paldata[(palsize - 1)*3+2];

					for ( int j = 0; j < pixels; j++ )
					{
						decaldata[j*4+0] = lastR;
						decaldata[j*4+1] = lastG;
						decaldata[j*4+2] = lastB;
						decaldata[j*4+3] = texdata[j];
					}
					break;
				}
				default:
				{
					for ( int j = 0; j < pixels; j++ )
					{
						int p = texdata[j];

						decaldata[j*4+0] = paldata[p*3+0];
						decaldata[j*4+1] = paldata[p*3+1];
						decaldata[j*4+2] = paldata[p*3+2];
						decaldata[j*4+3] = 255;
					}
					break;
				}
				};

#if defined( EXTRACT_TO_PNG_ON_LOAD )
				char name[64] = { 0 };
				strncat( name, "wad/decal/", 10 );
				strncat( name, texlump->name, 16 );
				strncat( name, ".png", 4 );
				stbi_write_png( name, texlump->width, texlump->height, 4, decaldata, 0 );
#endif // EXTRACT_TO_PNG_ON_LOAD

				shaderStage.m_currentTexture = Shader_UploadTexture( shaderDef, NULL, GL_RGBA, GL_COMPRESSED_RGBA, 4, texlump->width, texlump->height, true, decaldata );

				if ( decalType == 2 )
				{
					shaderStage.unknownInt1 = 101;
					SetBits( shaderDef->m_flags, SHADER_FLAG_TRANSLUCENT );
				}

				SetBits( shaderDef->m_flags, SHADER_FLAG_BIT12 );
			}
			else
			{
				shaderStage.m_currentTexture = shaderDef->m_texture;
				shaderStage.m_currentTexture->m_refCount++;
			}

			//
			// Initialize custom translucency for special textures
			//
			shaderDef->m_translucency = 1.f;

			for ( int j = 0; j < ARRAYSIZE( g_textureTranslucencyList ); j++ )
			{
				if ( !strcmp( g_textureTranslucencyList[j].name, textureLump.name ) )
				{
					shaderDef->m_translucency = g_textureTranslucencyList[j].translucency;
					break;
				}
			}

			if ( shaderDef->m_translucency < 1.f )
			{
				shaderDef->unknownInt2 = 101;

				SetBits( shaderDef->m_materialType, MAT_UTILITY );

				SetBits( shaderDef->m_flags, SHADER_FLAG_TRANSLUCENT );
			}

			// Texture with SCROLL in it's name becomes a scrolltexture
			if ( !strncmp( textureLump.name, "SCROLL", 6 ) )
			{
				shaderStage.m_data.m_subData.m_unknownInt3 = 3;
				shaderStage.m_data.m_subData.m_unknownFloat1 = -1.f;
			}

			Shader_AddStage( shaderDef, &shaderStage );
			Shader_Finish( shaderDef );

			lastShader = shaderDef;

			shaderDef->next = shaderList;
			shaderList = shaderDef;
		}
	}

	if ( hasAnimatedTextures )
	{
		BuildAnimatedTextureChains( lastShader, filePath );
	}
}

/*
===============
vpLoadPackage
===============
*/
DLL_EXPORT bool vpLoadPackage( int formatIndex, const char *filePath )
{
	if ( formatIndex != 0 )
		return false;

	const char *localFilePath = Sys_MakeLocalFileName( filePath );

	FILE *f = fopen( localFilePath, "rb" );
	if ( !f )
		return false;

	wadinfo_t wadHeader;
	if ( fread( &wadHeader, 1, sizeof( wadHeader ), f ) != sizeof( wadHeader ) )
	{
		fclose( f );
		return false;
	}

	if ( wadHeader.identification != IDWADHEADER || wadHeader.numlumps <= 0 )
	{
		fclose( f );
		return false;
	}

	PrecacheWADTextures( f, wadHeader, filePath );
	fclose( f );

	return true;
}

/*
===============
vpEnumSpriteFormats
Tells the editor which sprite formats are implemented by this plugin.
Extensions of those formats must be set as supported in the game profile.
===============
*/
DLL_EXPORT int vpEnumSpriteFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "Half-Life Alpha 0.52 SPR", ".spr", libraryHandle ) != false;
}

/*
===============
vpUnloadSprite
===============
*/
DLL_EXPORT void vpUnloadSprite( int formatIndex, qSpriteData_s *spriteData )
{
	if ( formatIndex != 0 )
		return;

	if ( spriteData->m_spritePtr )
	{
		Sys_Free( spriteData->m_spritePtr );
		spriteData->m_spritePtr = NULL;
	}
}

/*
===============
vpLoadSprite
===============
*/
DLL_EXPORT bool vpLoadSprite( int formatIndex, const char *filePath, byte *buf, int bufSize, qSpriteData_s *spriteData )
{
	if ( formatIndex != 0 )
		return false;

	return Mod_LoadSpriteModel( filePath, buf, spriteData );
}

/*
===============
vpEnumModelFormats
Tells the editor which model formats are implemented by this plugin.
Extensions of those formats must be set as supported in the game profile.
===============
*/
DLL_EXPORT int vpEnumModelFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "Half-Life Alpha 0.52 MDL", ".mdl", libraryHandle ) != false;
}

/*
===============
vpGetModelFormatFlags
===============
*/
DLL_EXPORT bool vpGetModelFormatFlags( int formatIndex )
{
	return formatIndex == 0;
}

/*
===============
vpGetModelBounds
===============
*/
DLL_EXPORT bool vpGetModelBounds( int formatIndex, float *bboxMin, float *bboxMax, unsigned int flags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	if ( formatIndex != 0 )
		return false;

	StudioRender *studioRender = reinterpret_cast<StudioRender *>( studioData->m_studioPtr );
	if ( studioRender )
	{
		studioRender->GetModelBounds( flags, (vec3_t *)&bboxMin, (vec3_t *)&bboxMax );
		return true;
	}

	return false;
}

/*
===============
vpUnloadModel
===============
*/
DLL_EXPORT void vpUnloadModel( int formatIndex, qStudioData_s *studioData )
{
	if ( formatIndex != 0 )
		return;

	StudioRender *studioRender = reinterpret_cast<StudioRender *>( studioData->m_studioPtr );
	if ( studioRender )
	{
		delete studioRender;
		studioData->m_studioPtr = NULL;
	}
}

/*
===============
vpLoadModel
===============
*/
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

/*
===============
vpRenderModel
===============
*/
DLL_EXPORT void vpRenderModel( int formatIndex, int renderFlags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	if ( formatIndex != 0 )
		return;

	StudioRender *studioRender = reinterpret_cast<StudioRender *>( studioData->m_studioPtr );
	if ( studioRender )
	{
		studioRender->R_StudioRenderFinal( entityInfo, studioData, renderFlags );
	}
}
