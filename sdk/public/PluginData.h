//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINEXPORT_H )
#define PLUGINEXPORT_H

/*
 Data handling

 All of the data enumeration functions share the same function prototype:

 DLL_EXPORT int vpEnumXXX( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
 {
 	return 0;
 }

 Returned value is the count of successfully registered data types
 pluginManager must be redirected into registerIOFormat.
*/

/*
 Supported data types:
 vpEnumExportFormats requires vpExport
 vpEnumImportFormats requires vpImport

 vpEnumTextureFormats requires vpSetPalette, vpFilterTextureName and vpLoadTexture
 vpEnumPackageFormats requires vpSetPalette, vpGetPackageInfo and vpLoadPackage
 vpEnumSkyFormats requires vpLoadSky

 vpEnumSpriteFormats requires vpSetPalette, vpUnloadSprite, vpLoadSprite
 vpEnumModelFormats requires vpSetPalette, vpGetModelFormatFlags, vpGetModelBounds, vpUnloadModel, vpLoadModel and vpRenderModel
 vpEnumParticlesFormats requires vpSetPalette, vpUnloadParticles, vpLoadParticles and vpRenderParticles

 vpEnumArchiveFormats requires vpUnloadArchive, vpLoadArchive, vpFindArchiveFile, vpLoadArchiveFile, and vpListArchiveFiles
*/

// clang-format off

// Prototype for the function that is used to register IO formats internally
typedef bool (*pfnRegisterIOFormat)( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle );

// clang-format on

#endif // !PLUGINEXPORT_H
