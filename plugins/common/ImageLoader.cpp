//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

// V_Stristr, V_StripExtension
#include "vstdlib_static.h"

// GL_LUMINANCE, GL_RGB, ...
#include "GL/glew.h"

// class ImageLoader
#include "ImageLoader.h"

// Plugin API
#include "PluginMeta.h"

#if defined( ENABLE_STB_IMAGELOADER )
// stbi_load_from_memory
#define STB_IMAGE_IMPLEMENTATION
#include "../vpExamplePlugin/stb_image.h"
#endif // ENABLE_STB_IMAGELOADER

/*
===============
ImageLoader::LoadImage
Creates a shader and uploads the texture
===============
*/
bool ImageLoader::LoadImage( const byte *buf, unsigned int bufSize, const char *filePath )
{
#if defined( QUAKE3 )
	if ( V_Stristr( filePath, "\\textures\\" ) || V_Stristr( filePath, "/textures/" ) )
		filePath += 10;
#endif

	char hashName[MAX_PATH] = { 0 };
	strncpy( hashName, filePath, sizeof( hashName ) );
	hashName[sizeof( hashName ) - 1] = '\0';

	V_StripExtension( hashName );

	char shaderName[64] = { 0 };
	strncat( shaderName, hashName, sizeof( shaderName ) );
	shaderName[sizeof( shaderName ) - 1] = '\0';

	V_FixSlashes( shaderName, '\\', '/' );

	qShader_t *shaderDef = Shader_Create( hashName, shaderName, 0 );
	if ( !shaderDef )
		return false;

	if ( shaderDef->m_stage )
		return false;

	qShaderStage_t shaderStage;
	memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

	qTexture_t *textureDef = Shader_LookupTexture( shaderName );
	if ( textureDef )
	{
		textureDef->m_refCount++;
	}
	else
	{
#if defined( USE_GPL_CODE )
		BuildGammaTable();
#endif // USE_GPL_CODE

		int width, height, numchannels;
		byte *imageData = LoadImageData( buf, bufSize, &width, &height, &numchannels, NULL );
		if ( imageData )
		{
			int pixelCount = width * height;

#if defined( USE_GPL_CODE )
			int rgbChannels = std::min( numchannels, 3 );

			for ( int i = 0; i < pixelCount; i++ )
			{
				for ( int c = 0; c < rgbChannels; c++ )
				{
					int b = ( i * numchannels + c );
					imageData[b] = g_gammatable[imageData[b]];
				}
			}
#endif // USE_GPL_CODE

			unsigned int pixelFormat = GL_LUMINANCE;
			unsigned int textureFormat = GL_COMPRESSED_LUMINANCE;

			switch ( numchannels )
			{
			case 3:
			{
				pixelFormat = GL_RGB;
				textureFormat = GL_COMPRESSED_RGB;
				break;
			}
			case 4:
			{
				pixelFormat = GL_RGBA;
				textureFormat = GL_COMPRESSED_RGBA;

				bool hasUsefulAlpha = false;

				for ( int i = 0; i < pixelCount; i++ )
				{
					if ( imageData[i * 4 + 3] >= 8 )
					{
						hasUsefulAlpha = true;
						break;
					}
				}

				if ( !hasUsefulAlpha )
				{
					for ( int i = 0; i < pixelCount; i++ )
					{
						imageData[i * 4 + 3] = 255;
					}
				}

				break;
			}
			};

			textureDef = Shader_UploadTexture( shaderDef, shaderName, pixelFormat, textureFormat, numchannels, width, height, true, imageData );
		}
	}

	shaderStage.m_numColors = 0;

	if ( textureDef )
	{
		shaderDef->m_texture = shaderStage.m_currentTexture = textureDef;
		textureDef->m_refCount++;
	}

	Shader_AddStage( shaderDef, &shaderStage );
	Shader_Finish( shaderDef );

	return true;
}

#if defined( ENABLE_STB_IMAGELOADER )
/*
===============
STBImageLoader::LoadImage
Loads any supported image type
===============
*/
byte *STBImageLoader::LoadImageData( const byte *buf, unsigned int bufSize, int *widthOut, int *heightOut, int *numChannelsOut, int *b )
{
	int width, height, numchannels;
	byte *pixels = stbi_load_from_memory( buf, bufSize, &width, &height, &numchannels, 0 );

	if ( widthOut )
		*widthOut = width;
	if ( heightOut )
		*heightOut = height;
	if ( numChannelsOut )
		*numChannelsOut = numchannels;
	if ( b )
		*b = 1;

	return pixels;
}
#endif // ENABLE_STB_IMAGELOADER
