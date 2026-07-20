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
	byte *data, *pPal, *pb;
	char identifier[256];

	/*char modelName[128] = { 0 };
	V_ExtractFileBase( filePath, modelName, sizeof( modelName ) );*/

	pinmodel = (studiohdr_t *)(byte *)buf;

	id = LittleLong (pinmodel->id);
	if (id != IDSTUDIOHEADER)
	{
		Sys_Error ("%s has wrong header magic (%x should be %x)", filePath, id, IDSTUDIOHEADER);
		return false;
	}

	version = LittleLong (pinmodel->version);
	if (version != STUDIO_VERSION)
	{
		memset (pinmodel, 0, sizeof(studiohdr_t));
		strcpy (pinmodel->name, "bogus");
		pinmodel->length = sizeof(studiohdr_t);

		Sys_Error ("%s has wrong version number (%i should be %i)", filePath, version, STUDIO_VERSION);
	}

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
			byte *rgb = (byte *)TempBuffer_GetSpace( 0, s * 3 ); // RGB Image Data

			for ( j = 0; j < s; j++ )
			{
				p = data[j];

				rgb[j*3+0] = pPal[p*3+0];
				rgb[j*3+1] = pPal[p*3+1];
				rgb[j*3+2] = pPal[p*3+2];
			}

			qShaderStage_t shaderStage;
			memset( &shaderStage, 0, sizeof( qShaderStage_t ) );

			qTexture_t *textureDef = Shader_UploadTexture( shaderDef, NULL, GL_RGB, GL_COMPRESSED_RGB, 3, ptexture->width, ptexture->height, true, rgb );

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
