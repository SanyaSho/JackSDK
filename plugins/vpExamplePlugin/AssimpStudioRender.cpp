//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#include "PluginMeta.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Assimp Library
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

#include "vstdlib_static.h"

#include "AssimpStudioRender.h"

static float s_rotationmatrix[3][4];

FORCEINLINE vec3_t TransformPoint( const float m[3][4], const aiVector3D &v )
{
	vec3_t p(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]
	);

	return p;
}

AssimpStudioRender::AssimpStudioRender()
{
	m_importer = new Assimp::Importer();
	m_scene = NULL;

	m_textureList = NULL;

	m_renderFlags = 0;
}

AssimpStudioRender::~AssimpStudioRender()
{
	if ( m_textureList )
	{
#if 0
		for ( int i = 0; i < m_scene->mNumMaterials; i++ )
		{
			if ( !m_textureList[i] )
				continue;

			Shader_Destroy( m_textureList[i] );
		}
#endif

		Sys_Free( m_textureList );
		m_textureList = NULL;
	}

	if ( m_importer )
	{
		delete m_importer;
		m_importer = NULL;
	}
}

bool AssimpStudioRender::LoadModel( const char *filePath, byte *buf, int bufSize, const char *hint, qStudioData_s *studioData )
{
	m_scene = m_importer->ReadFileFromMemory( buf, bufSize, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipUVs, hint );
	if ( !m_scene )
	{
		Sys_Warning( "Failed to load \"%s\" (%s)", filePath, m_importer->GetErrorString() );
		return false;
	}

	//
	// Load textures
	//
#if 0 // TODO: Need to have a proper VFS support in here
	if ( m_scene->mNumMaterials != 0 )
	{
		m_textureList = (qShader_t **)Sys_Malloc( m_scene->mNumMaterials * sizeof( qShader_t ) );
		memset( m_textureList, 0, m_scene->mNumMaterials * sizeof( qShader_t ) );

		for ( int i = 0; i < m_scene->mNumMaterials; i++ )
		{
			aiString aiTexName;
			if ( m_scene->mMaterials[i]->GetTexture( aiTextureType_DIFFUSE, 0, &aiTexName ) != AI_SUCCESS )
				continue;

			char basePath[MAX_PATH] = { 0 };
			V_ExtractFilePath( filePath, basePath, sizeof( basePath ) );

			char texPath[MAX_PATH] = { 0 };
			strncpy( texPath, basePath, sizeof( texPath ) );
			strncat( texPath, "/", sizeof( texPath ) );
			strncat( texPath, strlwr( (char *)aiTexName.C_Str() ), sizeof( texPath ) );

			if ( !Sys_FileExists( texPath ) )
			{
				V_StripExtension( texPath );
				strncat( texPath, ".jpg", sizeof( texPath ) );
			}

			int texSize = 0;
			byte *texData = Sys_LoadFile( texPath, &texSize );

			qShader_t *shaderDef = Shader_Create( NULL, NULL, 0 );
			if ( shaderDef )
			{
				SetBits( shaderDef->m_flags, SHADER_FLAG_BIT6 | SHADER_FLAG_BIT13 );

				int texWidth, texHeight, texNumChannels;
				byte *imgdata = stbi_load_from_memory( texData, texSize, &texWidth, &texHeight, &texNumChannels, 4 );

				unsigned int pixelFormat = 0;
				unsigned int textureFormat = 0;

				switch ( texNumChannels )
				{
				case 4:
					pixelFormat = GL_RGBA;
					textureFormat = GL_COMPRESSED_RGBA;
					break;
				default:
					pixelFormat = GL_RGB;
					textureFormat = GL_COMPRESSED_RGB;
				}

				qShaderStage_t shaderStage;
				memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

				qTexture_t *textureDef = Shader_UploadTexture( shaderDef, NULL, pixelFormat, textureFormat, texNumChannels, texWidth, texHeight, true, imgdata );

				shaderDef->m_texture = shaderStage.m_currentTexture = textureDef;
				shaderStage.m_currentTexture->m_refCount++;

				Shader_AddStage( shaderDef, &shaderStage );
				Shader_Finish( shaderDef );
			}

			m_textureList[i] = shaderDef;
		}
	}
#endif

	return true;
}

void AssimpStudioRender::GetModelBounds( int flags, vec3_t *bboxMin, vec3_t *bboxMax )
{
	vec3_t mins( FLT_MAX, FLT_MAX, FLT_MAX );
	vec3_t maxs( -FLT_MAX, -FLT_MAX, -FLT_MAX );

	for ( int i = 0; i < m_scene->mNumMeshes; i++ )
	{
		const aiMesh *mesh = m_scene->mMeshes[i];

		for ( int j = 0; j < mesh->mNumVertices; j++ )
		{
			const aiVector3D &p = mesh->mVertices[j];

			mins.x = std::min( mins.x, p.x );
			mins.y = std::min( mins.y, p.y );
			mins.z = std::min( mins.z, p.z );

			maxs.x = std::max( maxs.x, p.x );
			maxs.y = std::max( maxs.y, p.y );
			maxs.z = std::max( maxs.z, p.z );
		}
	}

	if ( bboxMin )
		*bboxMin = mins;
	if ( bboxMax )
		*bboxMax = maxs;
}

void AssimpStudioRender::RenderModel( qEntity_s *ent, qStudioData_s *studioData, int renderFlags )
{
	m_renderFlags = renderFlags;

	R_RotateForEntity( ent );

	for ( int i = 0; i < m_scene->mNumMeshes; i++ )
	{
		const aiMesh *mesh = m_scene->mMeshes[i];
		DrawMesh( ent, mesh );
	}
}

void AssimpStudioRender::DrawMesh( qEntity_t *ent, const aiMesh *mesh )
{
	byte alpha = 255;
	int renderFlags = 0;

	if ( !FBitSet( m_renderFlags, RFL_WIREFRAME | RFL_NOTEXTURES ) )
	{
		if ( FBitSet( ent->m_editorFlags, EFL_TRANSPARENT ) )
		{
			byte minAlpha = (byte)( PR_GetMinAlpha() * 255.f );
			alpha = std::min( minAlpha, ent->m_renderMode.m_renderColor.a );

			renderFlags = ent->m_renderMode.m_renderFlags;
		}

		if ( ( m_renderFlags & RFL_NOTEXTURES | RFL_FULLBRIGHT ) == RFL_FULLBRIGHT && alpha != 255 )
		{
			PR_Color4ub( 255, 255, 255, alpha );
		}

		if ( m_textureList && ( mesh->mMaterialIndex < m_scene->mNumMaterials ) )
		{
			PR_SetState( renderFlags | GLS_DEPTHWRITE );
			PR_BindShader( m_textureList[mesh->mMaterialIndex] );
		}
	}

	PR_Begin( PRIMTYPE_TRIANGLES );

	for ( int i = 0; i < mesh->mNumFaces; i++ )
	{
		const aiFace &face = mesh->mFaces[i];

		for ( int j = 0; j < face.mNumIndices; j++ )
		{
			if ( !FBitSet( m_renderFlags, RFL_WIREFRAME | RFL_FULLBRIGHT ) )
			{
				// TODO: Lighting
				PR_Color4ub( 255, 255, 255, alpha );
			}

			int idx = face.mIndices[j];

			if ( mesh->HasNormals() )
			{
				PR_Normal3fv( (float *)&mesh->mNormals[idx] );
			}

			if ( mesh->HasTextureCoords( 0 ) )
			{
				const aiVector3D &texCoord = mesh->mTextureCoords[0][idx];
				PR_TexCoord2f( texCoord.x, texCoord.y );
			}

			vec3_t vert = TransformPoint( s_rotationmatrix, mesh->mVertices[idx] );
			PR_Vertex3fv( vert.Base() );
		}
	}

	PR_End();
}

void AssimpStudioRender::R_RotateForEntity( qEntity_t *ent )
{
	float scale = 1.f;
	if ( !FBitSet( m_renderFlags, RFL_DISABLESCALING ) )
	{
		scale = ent->m_entityState.m_scale;
	}

	vec3_t angles = ent->m_vecAngles;

	if ( FBitSet( m_renderFlags, RFL_INVERTSTUDIOPITCH ) )
	{
		angles[PITCH] *= -1.f;
	}

	V_BuildTransformStudioMatrix( angles[YAW], angles[PITCH], angles[ROLL], ent->m_vecOrigin.Base(), scale, s_rotationmatrix );
}