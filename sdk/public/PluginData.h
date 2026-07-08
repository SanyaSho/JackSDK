//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINDATA_H )
#define PLUGINDATA_H

/*
 Data handling

 All of the IO enumeration functions share the same function prototype:

 DLL_EXPORT int vpEnumXXX( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
 {
 	return 0;
 }

 Returned value is the count of successfully registered data types
 pluginManager must be redirected into registerIOFormat.
*/

#include "BaseTypes.h"

struct qEntity_s;
struct qShader_s;

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

/* filePath - path to a file */
/* numMipTex - count of TYP_MIPTEX inside a WAD2/WAD3 */
typedef bool (*vpGetPackageInfo_t)( int formatIndex, const char *filePath, int *numMipTex );

/* filePath - path to a file */
/* Iterates all of the TYP_MIPTEX files inside a WAD2/WAD3 archive and registers them with Shader_Create/Shader_Finish */
typedef bool (*vpLoadPackage_t)( int formatIndex, const char *filePath );


/* Used by Quake 1. You might need this only if you have a profile with PROFILE_ALLOW_CUSTOM_PALETTE flag set */
typedef bool (*vpSetPalette_t)( struct qPalette_s *paletteData );

// clang-format on

/*
 Sprite data definition.
 Contains some basic info about a sprite image
*/
typedef struct qSpriteData_s
{
	int unkint1;
	int unkint2;

	/*
	 Internal format declared by vpEnumSpriteFormats
	*/
	int m_formatIndex;

	int m_spriteOrientation;

	qShader_s *m_spriteShader;

	char gap2[8];

	/*
	 vpUnloadSprite
	*/
	void *pfn_UnloadModel;
} qSpriteData_t;
COMPILE_TIME_ASSERT( sizeof( qSpriteData_t ) == 40 );


/*
 StudioMDL data definition.
 Contains some basic info about a studio model
*/

struct qStudioData_s;

// clang-format off

typedef bool (*vpGetModelFormatFlags_t)( int );

typedef bool (*vpGetModelBounds_t)( int formatIndex, float *bboxMin, float *bboxMax, unsigned int flags, qStudioData_s *studioData, qEntity_s *entityInfo );

typedef bool (*vpUnloadModel_t)( int formatIndex, qStudioData_s *studioData );

typedef bool (*vpLoadModel_t)( int formatIndex, const char *filePath, byte *buf, int bufSize, qStudioData_s *outStudioData );

typedef bool (*vpRenderModel_t)( int formatIndex, int editorFlags, qStudioData_s *studioData, qEntity_s *entityInfo );

// clang-format on

typedef struct qStudioData_s
{
	int unkint1;
	int unkint2;

	/*
	 Internal format declared by vpEnumModelFormats
	*/
	int m_formatIndex;

	char gap_[4];

	/*
	 Model bounding box
	*/
	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	/*
	 Pointer to an allocated memory (ex. studiohdr_t)

	 Created by: vpLoadModel
	 Destroyed by: vpUnloadModel
	*/
	void *m_studioInfo;

	/*
	 vpRenderModel, vpGetModelBounds, vpUnloadModel
	*/
	vpRenderModel_t pfnRenderModel;
	vpGetModelBounds_t pfnGetModelRenderBounds;
	vpUnloadModel_t pfnUnloadModel;
} qStudioData_t;
COMPILE_TIME_ASSERT( sizeof( qStudioData_t ) == 72 );


/*
 Particles data definition.
 Contains some basic info about a particle
*/

struct qParticlesData_s;

// clang-format off

typedef bool (*vpUnloadParticles_t)( int formatIndex, qParticlesData_s *particlesData );

typedef bool (*vpLoadParticles_t)( int formatIndex, const char *filePath, byte *buf, int bufSize, qParticlesData_s *outParticlesData );

typedef bool (*vpRenderParticles_t)( int formatIndex, int editorFlags, qParticlesData_s *particlesData, qEntity_s *entityInfo );

// clang-format on

typedef struct qParticlesData_s
{
	int unkint1;
	int unkint2;

	/*
	 Internal format declared by vpEnumModelFormats
	*/
	int m_formatIndex;

	char gap2[36];

	vpRenderParticles_t pfnRenderParticle;
	vpUnloadParticles_t pfnUnloadParticle;
} qParticlesData_t;
COMPILE_TIME_ASSERT( sizeof( qParticlesData_t ) == 64 );


/*
 Archive data definition.
 Contains some basic info about an archive
*/

struct qArchiveData_s;

// clang-format off

typedef void (*vpUnloadArchive_t)( int formatIndex, qArchiveData_s *archiveData );

/* filePath must be a full path to an archive */
typedef bool (*vpLoadArchvie_t)( int formatIndex, const char *filePath, qArchiveData_s *outArchiveData );

/* filePath accepts a full path to the file inside an archive */
typedef bool (*vpFindArchiveFile_t)( int formatIndex, qArchiveData_s *archiveData, const char *filePath );

/* filePath accepts a full path to the file inside an archive */
/* Value of **outBuf must be allocated using Sys_Malloc and freed using Sys_Free */
typedef long (*vpLoadArchiveFile_t)( int formatIndex, qArchiveData_s *archiveData, const char *filePath, char **outBuf );

/* Each value (including **outFiles) in ***outFiles must be allocated using Sys_Malloc and freed using Sys_Free when not needed */
typedef bool (*vpListArchiveFiles_t)( int formatIndex, qArchiveData_s *archiveData, const char *fileFilter, char ***outFiles );

// clang-format on

struct unknownArchvieStruct_t
{
	char gap[16];
};

typedef struct qArchiveData_s
{
	int unkint1;
	int unkint2;

	/*
	 Internal format declared by vpEnumModelFormats
	*/
	int m_formatIndex;

	char gap_[4];

	unknownArchvieStruct_t *m_unknownArchiveStruct;

	/*
	 Pointer to an allocated memory (ex. pak_t)

	 Created by: pfnLoadArchive
	 Destroyed by: pfnUnloadArchive
	*/
	void *m_archiveData;

	/*
	 vpFindArchiveFile, vpLoadArchvie, vpListArchiveFiles, vpUnloadArchive
	*/
	vpFindArchiveFile_t pfnFindArchiveFile;
	vpLoadArchvie_t pfnLoadArchive;
	vpListArchiveFiles_t pfnListArchvieFiles;
	vpUnloadArchive_t pfnUnloadArchive;
} qArchiveData_t;
COMPILE_TIME_ASSERT( sizeof( qArchiveData_t ) == 64 );

// clang-format off

// Prototype for the function that is used to register IO formats internally
typedef bool (*pfnRegisterIOFormat)( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle );

typedef int (*vpEnumGenericFunction_t)( pfnRegisterIOFormat registerIOFormat, void *libraryHandle );

// clang-format on

#endif // !PLUGINDATA_H
