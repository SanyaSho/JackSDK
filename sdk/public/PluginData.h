//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// Versions: "J.A.C.K. 1.1.3773 Freeware"; "J.A.C.K. 1.1.3773"; "J.A.C.K. 1.2.4603"
//
// Copyright (C) 2026  SanyaSho
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <https://www.gnu.org/licenses/>.
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

 Quake II/Quake III uses custom fields for face's surface and content flags
 New flags can be registered using vpEnumSurfaceFlags and vpEnumContentFlags functions.
 Max count of surface flags: 20
 Max count of content flags: 32

 Example:
 DLL_EXPORT int vpEnumSurfaceFlags( pfnRegisterFlags registerFlags, void *libraryHandle )
 {
 	registerFlags( "vpQuake3", "Detail", 27, libraryHandle );
 	return 1;
 }

 Returned value is the count of successfully registered data types
 libraryHandle must be redirected into registerFlags. However, it's not used inside the editor so technically can be set to NULL.
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
 (Only in Steam Beta) vpEnumParticlesFormats requires vpSetPalette, vpUnloadParticles, vpLoadParticles and vpRenderParticles

 vpEnumArchiveFormats requires vpUnloadArchive, vpLoadArchive, vpFindArchiveFile, vpLoadArchiveFile, and vpListArchiveFiles
*/

// clang-format off

/* vpExport and vpImport are used in pair with vpEnumExportFormats and vpEnumImportFormats */
/* filePath - path to a file */
/* seekOffset/readLimit is always set to 0 in the editor */
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
typedef int (*vpExport_t)( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef );
typedef int (*vpImport_t)( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef );
#elif JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
typedef int (*vpExport_t)( int formatIndex, const char *filePath, qWorld_s *worldDef );
typedef int (*vpImport_t)( int formatIndex, const char *filePath, qWorld_s *worldDef );
#else
#error
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA


typedef const char *(*vpFilterTextureName_t)( int formatIndex, const char *textureName );
typedef bool (*vpLoadTexture_t)( int formatIndex, const char *textureName, byte *buf, int bufSize );


/* filePath - path to a file */
/* numMipTex - count of TYP_MIPTEX inside a WAD2/WAD3 */
typedef bool (*vpGetPackageInfo_t)( int formatIndex, const char *filePath, int *numMipTex );

/* filePath - path to a file */
/* Iterates all of the TYP_MIPTEX files inside a WAD2/WAD3 archive and registers them with Shader_Create/Shader_Finish */
typedef bool (*vpLoadPackage_t)( int formatIndex, const char *filePath );


/* Must maintain the m_skyTextureList[side] list inside the skyShader */
typedef bool (*vpLoadSky_t)( int formatIndex, byte *buf, unsigned int bufSize, qShader_s *skyShader, unsigned int side );


typedef struct qPalette_s
{
	byte data[768];
} qPalette_t;
//COMPILE_TIME_ASSERT( sizeof( qPalette_t ) == ??? );

/* Used by Quake 1. You might need this only if you have a profile with PROFILE_ALLOW_CUSTOM_PALETTE flag set */
typedef bool (*vpSetPalette_t)( qPalette_s *paletteData );

// clang-format on


/*
 Sprite data definition.
 Contains some basic info about a sprite image
*/

struct qSpriteData_s;

// clang-format off

/*
 Will be called only if qSpriteData_s has m_dataPtr
*/
typedef bool (*vpUnloadSprite_t)( int formatIndex, qSpriteData_s *spriteData );

typedef bool (*vpLoadSprite_t)( int formatIndex, const char *filePath, byte *buf, int bufSize, qSpriteData_s *spriteData );

// clang-format on

/* Sprite orientation (Quake/Half-Life) */
typedef enum
{
	ORIENTED = 0,
	VP_PARALLEL,
	VP_PARALLEL_UPRIGHT,
	FACING_UPRIGHT,
	VP_PARALLEL_ORIENTED
} spriteOrientation_e;

typedef struct qSpriteData_s
{
	/*
	 Indicates that the sprite is loaded. Set by the editor after successful vpLoadSprite call
	*/
	int m_loaded;

	/*
	 The sprite wont be free'd after opening a new project if m_refCount is >0
	*/
	int m_refCount;

	/*
	 Internal format declared by vpEnumSpriteFormats
	*/
	int m_formatIndex;

	/*
	 Sprite orientation. See SPR_ defines above
	*/
	int m_spriteOrientation;

	struct qShader_s *m_spriteShader;

	/*
	 Pointer to an allocated memory (ex. SpriteMgr class instance)

	 Created by: vpLoadSprite
	 Destroyed by: vpUnloadSprite
	*/
	void *m_dataPtr;

	/*
	 vpUnloadSprite
	*/
	vpUnloadSprite_t m_pfnUnloadSprite;
} qSpriteData_t;
COMPILE_TIME_ASSERT( sizeof( qSpriteData_t ) == SIZEOF_QSPRITEDATA_S );


/*
 StudioMDL data definition.
 Contains some basic info about a studio model
*/

struct qStudioData_s;

// clang-format off

/*
 Returns true if the model is a studiomodel (adds a 0x8000 flag to the entity)
*/
typedef bool (*vpGetModelFormatFlags_t)( int formatIndex );

/*
 flags:
 0x20 - !Sys_VectorEmpty( m_bobParms1 )
 0x40 - has 0x8000 in GDClass
 0x80 - StudioInvertPitchScale
*/
typedef bool (*vpGetModelBounds_t)( int formatIndex, vec3_t *bboxMin, vec3_t *bboxMax, unsigned int flags, qStudioData_s *studioData, qEntity_s *entityInfo );

/*
 Will be called only if qStudioData_s has m_dataPtr
*/
typedef void (*vpUnloadModel_t)( int formatIndex, qStudioData_s *studioData );

typedef bool (*vpLoadModel_t)( int formatIndex, const char *filePath, byte *buf, int bufSize, qStudioData_s *studioData );

typedef void (*vpRenderModel_t)( int formatIndex, int renderFlags, qStudioData_s *studioData, qEntity_s *entityInfo );

// clang-format on

typedef struct qStudioData_s
{
	/*
	 Indicates that the model is loaded. Set by the editor after successful vpLoadModel call
	*/
	int m_loaded;

	/*
	 The model wont be free'd after opening a new project if m_refCount is >0
	*/
	int m_refCount;

	/*
	 Internal format declared by vpEnumModelFormats
	*/
	int m_formatIndex;

	/*
	 Does this model has transparent textures?
	 Set this to 1 when loading model textures
	*/
	int m_hasTranslucency;

	/*
	 Model bounding box
	*/
	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	/*
	 Pointer to an allocated memory (ex. StudioRender class instance)

	 Created by: vpLoadModel
	 Destroyed by: vpUnloadModel
	*/
	void *m_dataPtr;

	/*
	 vpRenderModel, vpGetModelBounds, vpUnloadModel
	*/
	vpRenderModel_t m_pfnRenderModel;
	vpGetModelBounds_t m_pfnGetModelRenderBounds;
	vpUnloadModel_t m_pfnUnloadModel;
} qStudioData_t;
COMPILE_TIME_ASSERT( sizeof( qStudioData_t ) == SIZEOF_QSTUDIODATA_S );


#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
 Particles data definition.
 Contains some basic info about a particle
*/

struct qParticlesData_s;

// clang-format off

/*
 Will be called only if qParticlesData_s has m_dataPtr
*/
typedef bool (*vpUnloadParticles_t)( int formatIndex, qParticlesData_s *particlesData );

typedef bool (*vpLoadParticles_t)( int formatIndex, const char *filePath, byte *buf, int bufSize, qParticlesData_s *particlesData );

typedef bool (*vpRenderParticles_t)( int formatIndex, int editorFlags, qParticlesData_s *particlesData, qEntity_s *entityInfo );

// clang-format on

typedef struct qParticlesData_s
{
	/*
	 Indicates that the particle system is loaded. Set by the editor after successful vpLoadParticles call
	*/
	int m_loaded;

	/*
	 The particle system wont be free'd after opening a new project if m_refCount is >0
	*/
	int m_refCount;

	/*
	 Internal format declared by vpEnumParticlesFormats
	*/
	int m_formatIndex;

	char gap2[4];

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	/*
	 Pointer to an allocated memory (ex. ParticleMgr class instance)

	 Created by: vpLoadParticles
	 Destroyed by: vpUnloadParticles
	*/
	void *m_dataPtr;

	vpRenderParticles_t m_pfnRenderParticle;
	vpUnloadParticles_t m_pfnUnloadParticle;
} qParticlesData_t;
COMPILE_TIME_ASSERT( sizeof( qParticlesData_t ) == SIZEOF_QPARTICLESDATA_S );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA


/*
 Archive data definition.
 Contains some basic info about an archive
*/

struct qArchiveData_s;

// clang-format off

/*
 Will be called only if qArchiveData_s has m_dataPtr
*/
typedef void (*vpUnloadArchive_t)( int formatIndex, qArchiveData_s *archiveData );

/* filePath must be a full path to the archive */
typedef bool (*vpLoadArchive_t)( int formatIndex, const char *filePath, qArchiveData_s *archiveData );

/* filePath accepts a full path to the file inside an archive */
typedef bool (*vpFindArchiveFile_t)( int formatIndex, qArchiveData_s *archiveData, const char *filePath );

/* filePath accepts a full path to the file inside an archive */
/* Value of **outBuf must be allocated using Sys_Malloc and freed using Sys_Free */
/* Returns size of file in bytes */
typedef size_t (*vpLoadArchiveFile_t)( int formatIndex, qArchiveData_s *archiveData, const char *filePath, char **outBuf );

/* Each value (including **outFiles) in ***outFiles must be allocated using Sys_Malloc and freed using Sys_Free when not needed */
typedef bool (*vpListArchiveFiles_t)( int formatIndex, qArchiveData_s *archiveData, const char *fileFilter, char ***outFiles );

// clang-format on

struct GCList_t
{
	GCList_t *next;
	struct CGameConfig *m_gameConfig;
};
COMPILE_TIME_ASSERT( sizeof( GCList_t ) == SIZEOF_GCLIST_T );

typedef struct qArchiveData_s
{
	/*
	 Indicates that the archive is loaded. Set by the editor after successful vpLoadArchive call
	*/
	int m_loaded;

	/*
	 Number of CGameConfig owners of this archive
	*/
	int m_refCount;

	/*
	 Internal format declared by vpEnumModelFormats
	*/
	int m_formatIndex;

	/*
	 List of CGameConfig instances with this archive loaded
	*/
	GCList_t *m_ownerList;

	/*
	 Pointer to an allocated memory (ex. pak_t struct )

	 Created by: pfnLoadArchive
	 Destroyed by: pfnUnloadArchive
	*/
	void *m_dataPtr;

	/*
	 vpFindArchiveFile, vpLoadArchvie, vpListArchiveFiles, vpUnloadArchive
	*/
	vpFindArchiveFile_t m_pfnFindArchiveFile;
	vpLoadArchive_t m_pfnLoadArchive;
	vpListArchiveFiles_t m_pfnListArchvieFiles;
	vpUnloadArchive_t m_pfnUnloadArchive;
} qArchiveData_t;
COMPILE_TIME_ASSERT( sizeof( qArchiveData_t ) == SIZEOF_QARCHIVEDATA_S );

// clang-format off

// Prototype for the function that is used to register IO formats internally
typedef bool (*pfnRegisterIOFormat)( int formatIndex, const char *formatName, const char *formatExtension, void *libraryHandle );

typedef int (*vpEnumGenericFunction_t)( pfnRegisterIOFormat registerIOFormat, void *libraryHandle );


// Prototype for the function that is used to register surface/content flags internally
typedef void (*pfnRegisterFlags)( const char *pluginName, const char *flagName, unsigned int bitIndex, void *libraryHandle );

typedef int (*vpEnumGenericFlags_t)( pfnRegisterFlags registerFlags, void *libraryHandle );

// clang-format on

#endif // !PLUGINDATA_H
