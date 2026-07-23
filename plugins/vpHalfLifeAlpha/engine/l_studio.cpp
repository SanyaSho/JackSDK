#include "quakedef.h"

/*
==================
Mod_LoadStudioModel
==================
*/
bool StudioRender::Mod_LoadStudioModel( const char *filePath, byte *buf, int bufSize, qStudioData_s *studioData )
{
	studiohdr_t *pinmodel;
	int id, version, i, j, p;
	mstudiotexture_t *ptexture;
	byte *data, *pPal, *pb, *imgdata;
	char identifier[256];

	/*char modelName[128] = { 0 };
	V_ExtractFileBase( filePath, modelName, sizeof( modelName ) );*/

	pinmodel = (studiohdr_t *)(byte *)buf;

	id = LittleLong (pinmodel->id);
	if (id != IDSTUDIOHEADER)
	{
		return false;
	}

	version = LittleLong (pinmodel->version);
	if (version != STUDIO_VERSION)
	{
		/*memset (pinmodel, 0, sizeof(studiohdr_t));
		strcpy (pinmodel->name, "bogus");
		pinmodel->length = sizeof(studiohdr_t);*/
		return false;
	}

	BuildGammaTable();

	assert( pinmodel->numbones < MAXSTUDIOBONES );
	assert( pinmodel->numbonecontrollers < MAXSTUDIOCONTROLLERS );
	assert( pinmodel->numseq < MAXSTUDIOSEQUENCES );
	assert( pinmodel->numtextures < MAXSTUDIOSKINS );

	m_studioHdr = (studiohdr_t *)Sys_Malloc( pinmodel->length );
	memcpy( m_studioHdr, buf, pinmodel->length );

	m_textureList = (qShader_t **)Sys_Malloc( pinmodel->numtextures * sizeof( qShader_t * ) );

	ptexture = (mstudiotexture_t *)((byte *)m_studioHdr + pinmodel->textureindex);
	for ( i = 0; i < pinmodel->numtextures; i++, ptexture++ )
	{
		int s = ptexture->width * ptexture->height;

		data = (byte *)m_studioHdr + ptexture->index;
		pPal = (byte *)data + s;

		/*strcpy (identifier, modelName);
		strcat (identifier, ptexture->name);*/

		qShader_t *shaderDef = Shader_Create( NULL, NULL, 0 );
		if ( shaderDef )
		{
			SetBits( shaderDef->m_flags, SHADER_FLAG_BIT6 | SHADER_FLAG_BIT13 );

			// GL_Upload16
			{
				//
				// Gamma correct palette
				//
				for (j=0 ; j<768 ; j++)
				{
					pPal[j*3+0] = g_gammatable[pPal[j*3+0]];
					pPal[j*3+1] = g_gammatable[pPal[j*3+1]];
					pPal[j*3+2] = g_gammatable[pPal[j*3+2]];
				}

				imgdata = (byte *)TempBuffer_GetSpace( 0, s * 3 ); // RGB Image Data

				for (j=0 ; j<s ; j++)
				{
					p = data[j];

					imgdata[j*3+0] = pPal[p*3+0];
					imgdata[j*3+1] = pPal[p*3+1];
					imgdata[j*3+2] = pPal[p*3+2];
				}
			}

			qShaderStage_t shaderStage;
			memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

			qTexture_t *textureDef = Shader_UploadTexture( shaderDef, NULL, GL_RGB, GL_COMPRESSED_RGB, 3, ptexture->width, ptexture->height, true, imgdata );

			shaderDef->m_texture = shaderStage.m_currentTexture = textureDef;
			shaderStage.m_currentTexture->m_refCount++;

			Shader_AddStage( shaderDef, &shaderStage );
			Shader_Finish( shaderDef );
		}

		ptexture->index = i;

		m_textureList[ptexture->index] = shaderDef;
	}

	return true;
}
