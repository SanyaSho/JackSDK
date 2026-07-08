//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINRENDER_H )
#define PLUGINRENDER_H

/*
 Rendering API
*/

#include "BaseTypes.h"


class CFrustum;
class CGLState;

typedef struct renditionInfo_s
{
	char gap[8];
	int m_renditionFlags;
	char gap2[96];
	vec3_t m_unknownVector1;
	vec3_t m_unknownVector2;
	vec3_t m_unknownVector3;
	vec3_t m_unknownVector4;
	char gap3[28];
	CFrustum *m_frustum;
	CGLState *m_glState;
} renditionInfo_t;
//COMPILE_TIME_ASSERT( sizeof( renditionInfo_t ) == ??? );


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
COMPILE_TIME_ASSERT( sizeof( qTexture_t ) == 48 );


/* Wont free any of textures from the textureList in CTextureManager::removeShaderStage */
#define SHDAERSTAGE_FLAG_NOFREELIST			( 1 << 1 )

typedef struct qShaderStage_s
{
	struct qShaderStage_s *next;

	/* Initial texture used by this stage */
	struct qTexture_s *m_currentTexture;

	char gap2[56];

	struct qTexture_s *m_textureList;

	int m_numColors;

	/* Stage flags (see above) */
	int m_shaderStageFlags;

	char gap3[8];

	float m_framerate;

	char gap4[284];
} qShaderStage_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStage_t ) == 384 );

inline qTexture_s *AddTextureToList( qTexture_s *&head, qTexture_s *textureHandle )
{
	qTexture_s **it = &head;

	while ( *it )
	{
		it = &( *it )->next;
	}

	*it = textureHandle;
	return textureHandle;
}


typedef struct qShader_s
{
	char m_name[64];

	int m_refCount;

	char gap[4];

	int m_textureWidth;
	int m_textureHeight;

	int m_flags;

	char gap2[28];

	int unknownInt1;

	char gap3[4];

	struct qTexture_s *m_texture;

	float unknownFloat1;

	char gap4[68];

	struct qShaderStage_s *m_stage;

	char gap5[432];
} qShader_t;
#if defined( WIN32 )
COMPILE_TIME_ASSERT( sizeof( qShader_t ) == 640 );
#else
COMPILE_TIME_ASSERT( sizeof( qShader_t ) == 1408 );
#endif // WIN32

// clang-format off

// clang-format on

#endif // !PLUGINRENDER_H
