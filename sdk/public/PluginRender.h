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

#if !defined( PLUGINRENDER_H )
#define PLUGINRENDER_H

/*
 Rendering API
*/

/*
RENDERFLAGS:
1<<0 - Wireframe
1<<1 - No textures. Set if current viewmode is "3D Filled Polygons"; Set in pair with 1<<0 if current viewmode is "3D Wireframe"
1<<2 - Animate models
1<<3 - Don't scale the model (???)
1<<4 - Fullbright. Set if current viewmode is "3D Textured Polygons"
1<<5 - ?
1<<6 - Sequence-related
1<<7 - QSB (Invert studio model pitch)

!( 1<<1 and 1<<4 ) must be checked before calling PR_CalcLighing
*/

// clang-format off

#define RFL_WIREFRAME				( 1 << 0 ) /* Viewport wants a wireframe renderer */
#define RFL_NOTEXTURES				( 1 << 1 ) /* Viewport doesnt want to draw textures */
#define RFL_ANIMATEMODELS			( 1 << 2 ) /* "Animate models" is checked */
#define RFL_DISABLESCALING			( 1 << 3 )
#define RFL_FULLBRIGHT				( 1 << 4 ) /* Viewport doesnt want to draw lighting */
// 1 << 5
// 1 << 6
#define RFL_INVERTSTUDIOPITCH		( 1 << 7 ) /* Invert studio model pitch ("Stupid Quake Bug") */

// clang-format on

#include "BaseTypes.h"


#if 0
class CFrustum;
class CGLState;

typedef struct renditionInfo_s
{
	char gap[8];
	int m_renditionFlags;
	char gap2[12];
	int m_textureBindFlags;
	char gap3[80];
	vec3_t m_modelOrigin;
	vec3_t m_vec1;
	vec3_t m_right;
	vec3_t m_up;
	char gap4[28];
	CFrustum *m_frustum;
	CGLState *m_glState;
} renditionInfo_t;
//COMPILE_TIME_ASSERT( sizeof( renditionInfo_t ) == ??? );
#endif


typedef struct viewInfo_s
{
	vec3_t r_origin;
	vec3_t b;
	vec3_t vright;
	vec3_t d;
} viewInfo_t;
//COMPILE_TIME_ASSERT( sizeof( viewInfo_t ) == ??? );


/* Does not register/unregister this texture from internal hash map (CTextureManager::destroyTexture) */
/* Automatically set if Shader_UploadTexture was called without a shaderName set (CTextureManager::createTexture) */
#define TEXTURE_FLAG_UNREGISTERED			( 1 << 0 )

/* Disables any texture data cleanup (CTextureManager::destroyTexture) */
#define TEXTURE_FLAG_PERSISTENT				( 1 << 1 )

/* Disables MipMap generation for this texture (CTextureView::uploadTexture) */
#define TEXTURE_FLAG_NOMIPMAPS				( 1 << 2 )

/* Don't recursively destroy the linked texture chain / chain is externally owned or shared (CTextureManager::removeShaderStage) */
#define TEXTURE_FLAG_LINKED					( 1 << 3 )

class QIcon;

typedef struct qTexture_s
{
	/* Internal ref count */
	int m_refCount;

	int m_width;
	int m_height;

	short m_numChannels;

	/* Texture flags (see above) */
	short m_flags;

	char __pad[4];

	/* OpenGL data */
	unsigned int m_glPixelFormat;
	unsigned int m_glTextureFormat;
	unsigned int m_glTextureIndex;

	/* Icon used by the editor */
	QIcon *m_icon;

	/* Used to determine the next image frame in the list */
	struct qTexture_s *next;
} qTexture_t;
COMPILE_TIME_ASSERT( sizeof( qTexture_t ) == SIZEOF_QTEXTURE_S );


typedef struct qShaderStageSubData_s
{
	int m_unknownInt3; // Set to 3 for SCROLL texture (vpHalfLife)

	char gap7[20];

	float m_unknownFloat1; // Set to 8.f and 16.f by vpQuake // Set to -1.f for SCROLL texture (vpHalfLife)

	char gap8[20];
} qShaderStageSubData_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStageSubData_t ) == SIZEOF_QSHADERSTAGESUBDATA_S );

typedef struct qShaderStageData_s
{
	char gap4[28];

	int m_unknownInt1; // Set to 2 for shadereditor material (vpQuake3)

	char gap5[20];

	int m_unknownInt2; // Set to 1 for shadereditor material (vpQuake3)

	char gap6[36];

	/* Not sure about this one, vpHalfLife does memset with sizeof( 48 ) on this address */
	struct qShaderStageSubData_s m_subData;

	char gap8[144];
} qShaderStageData_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStageData_t ) == SIZEOF_QSHADERSTAGEDATA_S );

/* Wont free any of textures from the textureList in CTextureManager::removeShaderStage */
#define SHDAERSTAGE_FLAG_NOFREELIST			( 1 << 1 )

typedef struct qShaderStage_s
{
	struct qShaderStage_s *next;

	/* Currently used texture */
	struct qTexture_s *m_currentTexture;

	/* 7 pointers? */
#if defined( JACK_64BIT )
	char gap2[56];
#else
	char gap2[28];
#endif // JACK_64BIT

	/* Texture list (used if texture has multiple frames) */
	struct qTexture_s *m_textureList;

	int unknownInt1;// Set to 101 in vpHalfLife if decaltype is 2

	/* Stage flags (see above) */
	int m_flags;

	int m_textureCount;

	char gap3[4];

	float m_framerate;

	/* Not sure about this one, vpQuake and vpHalfLife does memset with sizeof( 284 ) on this address (only on Windows) */
	struct qShaderStageData_s m_data;
} qShaderStage_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStage_t ) == SIZEOF_QSHADERSTAGE_S );

FORCEINLINE qTexture_s *AddTextureToList( qTexture_s *&head, qTexture_s *textureHandle )
{
	qTexture_s **it = &head;

	while ( *it )
	{
		it = &( *it )->next;
	}

	*it = textureHandle;
	return textureHandle;
}


// clang-format off

#define SHADER_FLAG_TRANSLUCENT			( 1 << 1  ) /* Enable translucency support */
#define SHADER_FLAG_BIT2				( 1 << 2  )
#define SHADER_FLAG_BIT4				( 1 << 4  )
#define SHADER_FLAG_BIT6				( 1 << 6  )
#define SHADER_FLAG_BIT12				( 1 << 12 )
#define SHADER_FLAG_BIT13				( 1 << 13 )
#define SHADER_FLAG_BROKEN_ANIMATION	( 1 << 24 ) // Used to setup textures with animation

#define MAT_NULL						( 1 << 0  ) /* NULL texture */
#define MAT_CLIP						( 1 << 1  ) /* CLIP texture */
#define MAT_HINT						( 1 << 2  ) /* HINT texture */
#define MAT_TRIGGER						( 1 << 3  ) /* TRIGGER texture */
// 1 << 4
#define MAT_UTILITY						( 1 << 5  ) /* A utility texture recognized by the map compilers */
// 1 << 6
#define MAT_LIQUID						( 1 << 7  ) /* Any liquid texture */
// 1 << 8
#define MAT_SKY							( 1 << 9  ) /* SKY texture */

// clang-format on

class CTextureParcel;

typedef struct qShader_s
{
	char m_name[64];

	int m_refCount;

	char gap[4]; // m_refCount2

	int m_textureWidth;
	int m_textureHeight;

	int m_flags;

	char gap1[4];

	/* Internal */
	CTextureParcel *m_texParcel;

	int m_surfaceFlags;
	int m_contentFlags;

	/*
	 Material type. See MAT_ defines above
	*/
	int m_materialType;

	/*
	 Quake II value
	*/
	int m_value;

	int unknownInt1; // Texture bitness? Set to 16 by vpHalfLife

	struct qTexture_s *m_texture;

	float m_translucency;

	int unknownInt2; // Set to 101 in vpHalfLife if shader translucency is < 1

	/* A pointer? */
#if defined( JACK_64BIT )
	char gap5[8];
#else
	char gap5[4];
#endif // JACK_64BIT

	struct qTexture_s *m_skyTextureList[6];

	/* A pointer? */
#if defined( JACK_64BIT )
	char gap7[8];
#else
	char gap7[4];
#endif // JACK_64BIT

	struct qShaderStage_s *m_stage;

	/* Next shader in the list */
	struct qShader_s *next;

	/* Currently used shader code (must be allocated and freed if used) */
	char *m_shaderCode;

	/* This gap is always 0 and always 156 bytes (no pointers) */
	char gap8[156];

	/* Path to the shader file */
	char m_shaderFilePath[MAX_PATH];
} qShader_t;
COMPILE_TIME_ASSERT( sizeof( qShader_t ) == SIZEOF_QSHADER_S );


/*
 qEntity_s::m_drawData for mod_sprite
*/
typedef struct qSpriteDrawData_s
{
	struct qSpriteData_s *m_spriteData;
	char gap1[2];
	char gap2[2];
	char gap3[4];
	char gap4[4];
	int m_unknownInt1;
} qSpriteDrawData_t;
COMPILE_TIME_ASSERT( sizeof( qSpriteDrawData_t ) == SIZEOF_QSPRITEDRAWDATA_S );


/*
 qEntity_s::m_drawData for mod_decal
*/
typedef struct qDecal_s
{
	struct qDecalFragment_s *m_decalData;

	/* Shader used by this decal */
	struct qShader_s *m_decalShader;

	/* Selection box size */
	vec3_t m_sizeMin;
	vec3_t m_sizeMax;
} qDecal_t;
COMPILE_TIME_ASSERT( sizeof( qDecal_t ) == SIZEOF_QDECAL_S );


/*
 qEntity_s::m_drawData for mod_studio
*/
typedef struct qStudioDrawData_s
{
	struct qStudioData_s *m_studioData1; // /shrug
	struct qStudioData_s *m_studioData2; // /shrug

	/* Selection box size */
	vec3_t m_sizeMin;
	vec3_t m_sizeMax;

	int m_unknownInt1;

	/* Sequence frametime (host_frametime) */
	float m_frametime;

	/* Quake III bobparms */
	/*
	if ( v71 != nullptr && (v71->m_flags & 0x4000) != 0 )
	{
		this->m_entityData->m_drawData->m_bobParms1 = v71->m_bobParms;
		this->m_entityData->m_drawData->m_bobParms2 = this->m_currentGDClass->m_bobParms2;
	}
	*/
	vec3_t m_bobParmsMin;
	vec3_t m_bobParmsMax;
} qStudioDrawData_t;
COMPILE_TIME_ASSERT( sizeof( qStudioDrawData_t ) == SIZEOF_QSTUDIODRAWDATA_S );


#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
 qEntity_s::m_drawData for mod_particles
*/
typedef struct qParticlesDrawData_s
{
	struct qParticlesData_s *m_particlesData;

	/* A pointer? */
#if defined( JACK_64BIT )
	char gap[8];
#else
	char gap[4];
#endif // JACK_64BIT

	/* Selection box size */
	vec3_t m_sizeMin;
	vec3_t m_sizeMax;

	char gap2[8];
} qParticlesDrawData_t;
COMPILE_TIME_ASSERT( sizeof( qParticlesDrawData_t ) == SIZEOF_QPARTICLESDRAWDATA_S );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

// clang-format off

// clang-format on

#endif // !PLUGINRENDER_H
