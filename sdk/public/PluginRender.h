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


typedef struct viewInfo_s
{
	vec3_t a;
	vec3_t b;
	vec3_t c;
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


typedef struct qShaderStageData_s
{
	char gap4[92];

	int m_unknownInt1; // Set to 3 for SCROLL texture (vpHalfLife)

	char gap5[20];

	float m_unknownFloat1; // Set to 8.f and 16.f by vpQuake // Set to -1.f for SCROLL texture (vpHalfLife)

	char gap6[164];
} qShaderStageData_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStageData_t ) == SIZEOF_QSHADERSTAGEDATA_S );

/* Wont free any of textures from the textureList in CTextureManager::removeShaderStage */
#define SHDAERSTAGE_FLAG_NOFREELIST			( 1 << 1 )

typedef struct qShaderStage_s
{
	struct qShaderStage_s *next;

	/* Currently used texture */
	struct qTexture_s *m_currentTexture;

#if defined( JACK_64BIT )
	char gap2[56];
#else
	char gap2[28];
#endif // JACK_64BIT

	/* Texture list (used if texture has multiple frames) */
	struct qTexture_s *m_textureList;

	int m_numColors; // TODO: this is not a number of colors

	/* Stage flags (see above) */
	int m_shaderStageFlags;

	char gap3[8];

	float m_framerate;

	/* Not sure about this one, vpQuake and vpHalfLife does memset with sizeof( 284 ) on this address (only on Windows) */
	struct qShaderStageData_s m_data;
} qShaderStage_t;
COMPILE_TIME_ASSERT( sizeof( qShaderStage_t ) == SIZEOF_QSHADERSTAGE_S );

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


#define SHADER_FLAG_BROKEN_ANIMATION ( 1 << 24 ) // Used to setup textures with animation

typedef struct qShader_s
{
	char m_name[64];

	int m_refCount;

	char gap[4]; // m_refCount2

	int m_textureWidth;
	int m_textureHeight;

	int m_flags;

#if defined( JACK_64BIT )
	char gap2[12];
#else
	char gap2[8];
#endif // JACK_64BIT

	int m_surfaceFlags;
	int m_contentFlags;
	int m_materialType;
	int m_value;

	size_t unknownInt1; // TODO: is this size_t?

	struct qTexture_s *m_texture;

	float m_framerate; // TODO: Is this actually a framerate?

	int unknownInt2; // Set to 101 in vpHalfLife if "framerate" is < 1

#if defined( JACK_64BIT )
	char gap5[8];
#endif // JACK_64BIT

	struct qTexture_s *m_skyTextureList[6];

#if defined( JACK_64BIT )
	char gap7[8];
#else
	char gap7[4];
#endif // JACK_64BIT

	struct qShaderStage_s *m_stage;

	struct qShader_s *next;

#if defined( WIN32 )
	char gap8[424];
#else
	char gap8[1192];
#endif // WIN32
} qShader_t;
#if defined( WIN32 )
COMPILE_TIME_ASSERT( sizeof( qShader_t ) == SIZEOF_QSHADER_S_WINDOWS );
#else
COMPILE_TIME_ASSERT( sizeof( qShader_t ) == SIZEOF_QSHADER_S_LINUX );
#endif // WIN32

// clang-format off

// clang-format on

#endif // !PLUGINRENDER_H
