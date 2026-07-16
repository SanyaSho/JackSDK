//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>

#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "PluginMeta.h"

/*
===============
vpEnumSpriteFormats
Tells the editor which formats are implemented by this plugin.
Those formats must be set as a supported sprite types in the game profile.
===============
*/
DLL_EXPORT int vpEnumSpriteFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	registerIOFormat( 0, "Portable Network Graphics", ".png", libraryHandle );
	registerIOFormat( 1, "Graphics Interchange Format", ".gif", libraryHandle );
	return 2;
}

/*
===============
vpUnloadSprite
===============
*/
DLL_EXPORT bool vpUnloadSprite( int formatIndex, qSpriteData_s *spriteData )
{
	return true;
}

/*
===============
LoadSprite_PNG
PNG Loader (single-frame)
===============
*/
static bool LoadSprite_PNG( const char *filePath, byte *buf, int bufSize, qSpriteData_s *outSpriteData )
{
	static const unsigned char s_pngSignature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	if ( memcmp( buf, s_pngSignature, sizeof( s_pngSignature ) ) != 0 )
	{
		Sys_Printf( "Failed to load \"%s\". (Not a valid PNG)", filePath );
		return false;
	}

	outSpriteData->m_spriteOrientation = SPR_ORIENTED;

	/* Create the shader */
	qShader_s *spriteShader = Shader_Create( filePath, NULL, 0 );

	spriteShader->m_flags |= 0x2040;

	int width, height, numchannels;
	unsigned char *pixels = stbi_load_from_memory( buf, bufSize, &width, &height, &numchannels, 4 );

	spriteShader->m_textureWidth = width;
	spriteShader->m_textureHeight = height;

	qShaderStage_t shaderStage;
	memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

	shaderStage.m_framerate = 1.f;

	qTexture_t *textureDef = Shader_UploadTexture( spriteShader, NULL, GL_RGBA, GL_COMPRESSED_RGBA, numchannels, width, height, true, pixels );
	AddTextureToList( shaderStage.m_textureList, textureDef );

	spriteShader->m_texture = shaderStage.m_currentTexture = shaderStage.m_textureList;

	++shaderStage.m_textureList->m_refCount;

	Shader_AddStage( spriteShader, &shaderStage );
	Shader_Finish( spriteShader );

	outSpriteData->m_spriteShader = spriteShader;

	return true;
}

/*
===============
LoadSprite_GIF
GIF Loader (mutli-frame)
===============
*/
static bool LoadSprite_GIF( const char *filePath, byte *buf, int bufSize, qSpriteData_s *outSpriteData )
{
	static const unsigned char s_gifSignature[6] = { 'G', 'I', 'F', '8', '9', 'a' };
	if ( memcmp( buf, s_gifSignature, sizeof( s_gifSignature ) ) != 0 )
	{
		Sys_Printf( "Failed to load \"%s\". (Not a valid GIF)", filePath );
		return false;
	}

	outSpriteData->m_spriteOrientation = SPR_ORIENTED;

	/* Create the shader */
	qShader_s *spriteShader = Shader_Create( filePath, NULL, 0 );

	spriteShader->m_flags |= 0x2040;

	int *numdelays = NULL;
	int width, height, numframes, numchannels;
	unsigned char *pixels = stbi_load_gif_from_memory( buf, bufSize, &numdelays, &width, &height, &numframes, &numchannels, 4 );

	spriteShader->m_textureWidth = width;
	spriteShader->m_textureHeight = height;

	qShaderStage_t shaderStage;
	memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

	shaderStage.m_framerate = 1.f;

	for ( int i = 0; i < numframes; i++ )
	{
		unsigned char *framePixels = pixels + ( width * height * numchannels * i );

		qTexture_t *textureDef = Shader_UploadTexture( spriteShader, NULL, GL_RGBA, GL_COMPRESSED_RGBA, numchannels, width, height, true, framePixels );
		AddTextureToList( shaderStage.m_textureList, textureDef );
	}

	spriteShader->m_texture = shaderStage.m_currentTexture = shaderStage.m_textureList;

	++shaderStage.m_textureList->m_refCount;

	Shader_AddStage( spriteShader, &shaderStage );
	Shader_Finish( spriteShader );

	outSpriteData->m_spriteShader = spriteShader;

	if ( numdelays )
	{
		free( numdelays );
		numdelays = NULL;
	}

	return true;
}

/*
===============
vpLoadSprite
Tells the plugin to load sprite with a specific format
===============
*/
DLL_EXPORT bool vpLoadSprite( int formatIndex, const char *filePath, byte *buf, int bufSize, qSpriteData_s *outSpriteData )
{
	switch ( formatIndex )
	{
	case 0:
		return LoadSprite_PNG( filePath, buf, bufSize, outSpriteData );
	case 1:
		return LoadSprite_GIF( filePath, buf, bufSize, outSpriteData );
	};

	return false;
}
