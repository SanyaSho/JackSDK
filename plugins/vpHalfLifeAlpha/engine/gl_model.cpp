#include "quakedef.h"
#include "gl_model.h"

static byte *pspritepal;

/*
=================
Mod_LoadSpriteFrame
=================
*/
static void * Mod_LoadSpriteFrame (void * pin, mspriteframe_t **ppframe, int framenum, qShader_t *shaderDef, qShaderStage_t &shaderStage)
{
	dspriteframe_t		*pinframe;
	mspriteframe_t		*pspriteframe;
	int					i, width, height, size, origin[2];
	byte				*pdata, *imgdata;
	unsigned short		*ppixout;
	byte				*ppixin;
	//char				name[64];

	pinframe = (dspriteframe_t *)pin;

	width = LittleLong (pinframe->width);
	height = LittleLong (pinframe->height);
	size = width * height;

	pspriteframe = (mspriteframe_t *)TempBuffer_GetSpace (0, sizeof (mspriteframe_t));

	memset (pspriteframe, 0, sizeof (mspriteframe_t));

	*ppframe = pspriteframe;

	pspriteframe->width = width;
	pspriteframe->height = height;
	origin[0] = LittleLong (pinframe->origin[0]);
	origin[1] = LittleLong (pinframe->origin[1]);

	pspriteframe->up = origin[1];
	pspriteframe->down = origin[1] - height;
	pspriteframe->left = origin[0];
	pspriteframe->right = width + origin[0];

	pdata = (byte *)(pinframe + 1);

	// GL_Upload16
	{
		//
		// Gamma correct palette
		//
		for (i=0 ; i<768 ; i++)
		{
			pspritepal[i*3+0] = g_gammatable[pspritepal[i*3+0]];
			pspritepal[i*3+1] = g_gammatable[pspritepal[i*3+1]];
			pspritepal[i*3+2] = g_gammatable[pspritepal[i*3+2]];
		}

		imgdata = (byte *)TempBuffer_GetSpace( 0, size * 4 );
		for (i=0 ; i < size; i++)
		{
			int p = pdata[i];

			if (p == 255)
			{
				imgdata[i*4+0] = 0;
				imgdata[i*4+1] = 0;
				imgdata[i*4+2] = 0;
				imgdata[i*4+3] = 0;
			}
			else
			{
				imgdata[i*4+0] = pspritepal[p*3+0];
				imgdata[i*4+1] = pspritepal[p*3+1];
				imgdata[i*4+2] = pspritepal[p*3+2];
				imgdata[i*4+3] = 255;
			}
		}
	}

	//sprintf (name, "%s_%i", loadmodel->name, framenum);
	//pspriteframe->gl_texturenum = GL_LoadTexture (name, width, height, (byte *)(pinframe + 1), true, TEX_TYPE_ALPHA, bPal);

	qTexture_t *textureDef = Shader_UploadTexture (shaderDef, NULL, GL_RGBA, GL_COMPRESSED_RGBA, 4, width, height, true, imgdata);
	AddTextureToList (shaderStage.m_textureList, textureDef);

	return (void *)((byte *)pinframe + sizeof (dspriteframe_t) + size);
}


/*
=================
Mod_LoadSpriteGroup
=================
*/
static void * Mod_LoadSpriteGroup (void * pin, mspriteframe_t **ppframe, int framenum, qShader_t *shaderDef, qShaderStage_t &shaderStage)
{
	dspritegroup_t		*pingroup;
	mspritegroup_t		*pspritegroup;
	int					i, numframes;
	dspriteinterval_t	*pin_intervals;
	float				*poutintervals;
	void				*ptemp;

	pingroup = (dspritegroup_t *)pin;

	numframes = LittleLong (pingroup->numframes);

	pspritegroup = (mspritegroup_t *)TempBuffer_GetSpace (0, sizeof (mspritegroup_t) +
				(numframes - 1) * sizeof (pspritegroup->frames[0]));

	pspritegroup->numframes = numframes;

	*ppframe = (mspriteframe_t *)pspritegroup;

	pin_intervals = (dspriteinterval_t *)(pingroup + 1);

	poutintervals = (float *)TempBuffer_GetSpace (1, numframes * sizeof (float));

	pspritegroup->intervals = poutintervals;

	for (i=0 ; i<numframes ; i++)
	{
		*poutintervals = LittleFloat (pin_intervals->interval);
		if (*poutintervals <= 0.0)
			Sys_Error ("Mod_LoadSpriteGroup: interval<=0");

		poutintervals++;
		pin_intervals++;
	}

	ptemp = (void *)pin_intervals;

	for (i=0 ; i<numframes ; i++)
	{
		ptemp = Mod_LoadSpriteFrame (ptemp, &pspritegroup->frames[i], framenum * 100 + i, shaderDef, shaderStage);
	}

	return ptemp;
}


static int s_orientationTable[5] = { FACING_UPRIGHT, VP_PARALLEL_UPRIGHT, ORIENTED, VP_PARALLEL, VP_PARALLEL_ORIENTED };

/*
=================
Mod_LoadSpriteModel
=================
*/
bool Mod_LoadSpriteModel (const char *filePath, byte *buf, qSpriteData_t *spriteData)
{
	int					i;
	int					id, version;
	dsprite_t			*pin;
	msprite_t			*psprite;
	int					numframes;
	int					palsize;
	int					size;
	dspriteframetype_t	*pframetype;
	
	pin = (dsprite_t *)buf;

	id = LittleLong (pin->ident);
	if (id != IDSPRITEHEADER)
	{
		return false;
	}

	version = LittleLong (pin->version);
	if (version != SPRITE_VERSION)
	{
		return false;
	}

	numframes = LittleLong (pin->numframes);
	if (numframes < 1)
	{
		return false;
	}

	palsize = LittleShort (*(short *)((byte *)pin + sizeof(dsprite_t)));
	if (palsize != 256)
	{
		return false;
	}

	palsize = 2 + palsize * 3;

	size = sizeof (msprite_t) +	(numframes - 1) * sizeof (psprite->frames);

	psprite = (msprite_t *)Sys_Malloc (size + palsize);

	spriteData->m_dataPtr = psprite;

	psprite->type = LittleLong (pin->type);
	psprite->maxwidth = LittleLong (pin->width);
	psprite->maxheight = LittleLong (pin->height);
	psprite->beamlength = LittleFloat (pin->beamlength);
	//mod->synctype = LittleLong (pin->synctype);
	psprite->numframes = numframes;

	spriteData->m_spriteOrientation = s_orientationTable[psprite->type];

	qShader_t *shaderDef = Shader_Create( filePath, NULL, 0 );
	SetBits( shaderDef->m_flags, SHADER_FLAG_BIT6 | SHADER_FLAG_BIT13 );

	shaderDef->m_textureWidth = psprite->maxwidth;
	shaderDef->m_textureHeight = psprite->maxheight;

	qShaderStage_t shaderStage;
	memset( &shaderStage, 0, sizeof( qShaderStage_t ) );
	memset( &shaderStage.m_data, 0, sizeof( qShaderStageData_t ) );

	shaderStage.m_framerate = 1.f;

	//mod->mins[0] = mod->mins[1] = -psprite->maxwidth/2;
	//mod->maxs[0] = mod->maxs[1] = psprite->maxwidth/2;
	//mod->mins[2] = -psprite->maxheight/2;
	//mod->maxs[2] = psprite->maxheight/2;

	psprite->paloffset = size + 2;
	pspritepal = (byte *)psprite + size;

	memcpy ((byte *)psprite + size, (byte *)(pin + 1) + 2, palsize);
	
//
// load the frames
//
	//if (numframes < 1)
	//	Sys_Error ("Mod_LoadSpriteModel: Invalid # of frames: %d\n", numframes);

	//mod->numframes = numframes;

	BuildGammaTable();

	pframetype = (dspriteframetype_t *)((byte *)(pin + 1) + palsize);

	for (i=0 ; i<numframes ; i++)
	{
		spriteframetype_t	frametype;

		frametype = LittleLong (pframetype->type);
		psprite->frames[i].type = frametype;

		if (frametype == SPR_SINGLE)
		{
			pframetype = (dspriteframetype_t *)
					Mod_LoadSpriteFrame (pframetype + 1,
										 &psprite->frames[i].frameptr, i,
										 shaderDef, shaderStage);
		}
		else
		{
			pframetype = (dspriteframetype_t *)
					Mod_LoadSpriteGroup (pframetype + 1,
										 &psprite->frames[i].frameptr, i,
										 shaderDef, shaderStage);
		}
	}

	shaderDef->m_texture = shaderStage.m_textureList;
	shaderStage.m_currentTexture = shaderStage.m_textureList;
	shaderStage.m_textureList->m_refCount++;

	Shader_AddStage( shaderDef, &shaderStage );
	Shader_Finish( shaderDef );

	//mod->type = mod_sprite;
	spriteData->m_spriteShader = shaderDef;

	return true;
}
