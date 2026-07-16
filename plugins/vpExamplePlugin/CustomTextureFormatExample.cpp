//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>

// class ImageLoader
#include "ImageLoader.h"

// Plugin API
#include "PluginMeta.h"

// clang-format off
struct textureFormat_s
{
	int m_formatIndex;
	const char *m_formatName;
	const char *m_formatExtension;
} g_textureFormat[] =
{
	{ 0, "Portable Network Graphics", ".png" },
	{ 1, "Windows Bitmap", ".bmp" },
	{ 2, "Graphics Interchange Format", ".gif" },
	{ 3, "Adobe Photoshop Document", ".psd" },
	{ 4, "PICtor PIC image format", ".pic" },
	{ 5, "Joint Photographic Experts Group", ".jpg" },
	{ 6, "Portable pixmap", ".pnm" },
	{ 7, "Radiance RGBE", ".hdr" },
	{ 8, "Truevision TGA", ".tga" },
};
// clang-format on

/*
===============
vpEnumTextureFormats
Tells the editor which texture formats are implemented by this plugin.
Extensions of those formats must be set as supported in the game profile.
===============
*/
DLL_EXPORT int vpEnumTextureFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	int i = 0;
	for ( i = 0; i < ARRAYSIZE( g_textureFormat ); i++ )
	{
		i += registerIOFormat( g_textureFormat[i].m_formatIndex, g_textureFormat[i].m_formatName, g_textureFormat[i].m_formatExtension, libraryHandle ) ? 1 : 0;
	}

	return i;
}

/*
===============
vpFilterTextureName
Used to sanitize the texture name. (ex. removing drive letter; changing path separators)
===============
*/
DLL_EXPORT const char *vpFilterTextureName( int formatIndex, const char *filePath )
{
	return filePath;
}

/*
===============
vpLoadTexture
Loads texture from buf to the editor's memory
===============
*/
DLL_EXPORT bool vpLoadTexture( int formatIndex, const char *filePath, byte *buf, int bufSize )
{
	STBImageLoader stbImageLoader;
	return stbImageLoader.LoadImage( buf, bufSize, filePath );
}
