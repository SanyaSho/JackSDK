#include "quakedef.h"
#include "mathlib.h"

viewInfo_t gViewinfo;

#if !defined( PLUGINSDK )
extern void R_RotateForEntity (entity_t *e);
extern	cvar_t	gl_ztrick;
extern vec3_t			lightspot;
#endif // !PLUGINSDK

#if 0
int r_dointerp = 1;
#endif

// Model to world transformation
float rotationmatrix[3][4];

// Concatenated bone and light transforms
float bonetransform[MAXSTUDIOBONES][3][4];
#if 0
float lighttransform[MAXSTUDIOBONES][3][4];
#endif

int chrome[MAXSTUDIOVERTS][2];

#if 0
int r_ambientlight;
vec3_t r_plightvec;
float r_shadelight;
colorVec r_icolormix;
#endif

#if 0
studiohdr_t *pstudiohdr;
#endif
mstudiobodyparts_t *pbodypart;
mstudiomodel_t *psubmodel;
mstudiomesh_t *pmesh;

#if 0
int smodels_total;
#endif
int r_anumverts;

// Vert data, position and lighting
auxvert_t auxverts[MAXSTUDIOVERTS];
vec3_t lightvalues[MAXSTUDIOVERTS];

auxvert_t *pauxverts;

vec3_t *pvlightvalues;

#if 0
vec3_t shadevector;
#endif

#if !defined( PLUGINSDK )
qboolean R_StudioCheckBBox (void)
{
	return true;
}
#endif // !PLUGINSDK

StudioRender::StudioRender()
{
	m_studioHdr = NULL;

	m_textureList = NULL;

	m_renderFlags = 0;
}

StudioRender::~StudioRender()
{
	if ( m_textureList )
	{
		for ( int i = 0; i < m_studioHdr->numtextures; i++ )
		{
			if ( !m_textureList[i] )
				continue;

			Shader_Destroy( m_textureList[i] );
		}

		Sys_Free( m_textureList );
		m_textureList = NULL;
	}

	if ( m_studioHdr )
	{
		Sys_Free( m_studioHdr );
		m_studioHdr = NULL;
	}
}

#if 0
int R_StudioBodyVariations (model_t *model)
{
	studiohdr_t			*phdr;
	mstudiobodyparts_t	*pbodypart;

	if ( model->type != mod_studio )
		return 0;

	phdr = (studiohdr_t *)Mod_Extradata( model );
	if ( !phdr )
		return 0;

	pbodypart = (mstudiobodyparts_t *)((byte *)phdr + phdr->bodypartindex);

	return pbodypart->nummodels;
}
#endif

void StudioRender::R_StudioCalcRotations (vec3_t *pos, vec4_t *q, int sequence, float f)
{
	mstudioseqdesc_t		*pseqdesc;
	mstudioanim_t			*panim;
	mstudiobonecontroller_t	*pbonecontroller;
	mstudiobonerot_t		*pbonerot;
	mstudiobonepos_t		*pbonepos;
	float					dadt, value;
	int						i, j, k;
	byte					pcontroller1, pcontroller2;
	vec3_t					adj;
	int						frame, t;
	vec3_t					angle1, angle2;
	vec4_t					q1, q2;

	frame = (int)f;

	pseqdesc = (mstudioseqdesc_t *)((byte *)m_studioHdr + m_studioHdr->seqindex) + sequence;
	panim = (mstudioanim_t *)((byte *)m_studioHdr + pseqdesc->animindex);

	{
		dadt = 1.0;

		/*if (currententity->animtime >= currententity->prevanimtime + 0.01)
		{
			dadt = (cl.time - currententity->animtime) / 0.1;
		}*/

		if (dadt > 2.0)
		{
			dadt = 2.0;
		}
	}

	{
		pbonecontroller = (mstudiobonecontroller_t *)((byte *)m_studioHdr + m_studioHdr->bonecontrollerindex);
		for (i=0 ; i<m_studioHdr->numbones ; i++)
		{
			pbonerot = (mstudiobonerot_t *)((byte *)m_studioHdr + panim[i].rotindex);
			pbonepos = (mstudiobonepos_t *)((byte *)m_studioHdr + panim[i].posindex);

			// clear the vector
			VectorCopy (vec3_origin, adj);

			for (j=0 ; j<m_studioHdr->numbonecontrollers ; j++)
			{
				if (pbonecontroller[j].bone != i)
					continue;

				pcontroller1 = 0; //currententity->controller[j];
				pcontroller2 = 0; //currententity->prevcontroller[j];

				if (pbonecontroller[j].type & STUDIO_RLOOP)
				{
					if (abs(pcontroller1 - pcontroller2) > 128)
					{
						int a, b;
						a = (pcontroller1 + 128) % 256;
						b = (pcontroller2 + 128) % 256;
						value = ((a * dadt) + (b * (1 - dadt)) - 128) * (360.0 / 256.0) + pbonecontroller[j].start;
					}
					else
					{
						value = (pcontroller1 * dadt + pcontroller2 * (1 - dadt)) * (360.0 / 256.0) + pbonecontroller[j].start;
					}
				}
				else
				{
					value = (pcontroller1 * dadt + pcontroller2 * (1.0 - dadt)) / 255.0;
					if (value < 0.0) value = 0.0;
					if (value > 1.0) value = 1.0;
					value = (1 - value) * pbonecontroller[j].start + value * pbonecontroller[j].end;
				}

				switch (pbonecontroller[j].type & STUDIO_TYPES)
				{
				case STUDIO_XR:
					adj[0] += value;
					break;
				case STUDIO_YR:
					adj[1] += value;
					break;
				case STUDIO_ZR:
					adj[2] += value;
					break;
				}
			}

			int numPosFrames = panim[i].numpos;

			int posFrame1 = 0;
			for (k=1 ; k<numPosFrames ; k++)
			{
				if (pbonepos[k].frame > frame)
					break;

				posFrame1 = k;
			}

			if (posFrame1 + 1 >= numPosFrames)
			{
				VectorCopy (pbonepos[posFrame1].pos, pos[i]);
			}
			else
			{
				t = (frame - pbonepos[posFrame1].frame) / (float)(pbonepos[posFrame1 + 1].frame - pbonepos[posFrame1].frame);
				VectorLerp(pbonepos[posFrame1].pos, pbonepos[posFrame1 + 1].pos, pos[i], t);
			}

			int numRotFrames = panim[i].numrot;

			int rotFrame1 = 0;
			for (k=1 ; k<numRotFrames ; k++)
			{
				if (pbonerot[k].frame > frame)
					break;

				rotFrame1 = k;
			}

			if (rotFrame1 + 1 >= numRotFrames)
			{
				angle1[0] = pbonerot[rotFrame1].angle[0] / 100.0f + adj[0];
				angle1[1] = pbonerot[rotFrame1].angle[1] / 100.0f + adj[1];
				angle1[2] = pbonerot[rotFrame1].angle[2] / 100.0f + adj[2];
				V_AngleQuaternion (angle1, q[i]);
			}
			else
			{
				angle1[0] = pbonerot[rotFrame1].angle[0] / 100.0f + adj[0];
				angle1[1] = pbonerot[rotFrame1].angle[1] / 100.0f + adj[1];
				angle1[2] = pbonerot[rotFrame1].angle[2] / 100.0f + adj[2];
				V_AngleQuaternion(angle1, q1);

				angle2[0] = pbonerot[rotFrame1 + 1].angle[0] / 100.0f + adj[0];
				angle2[1] = pbonerot[rotFrame1 + 1].angle[1] / 100.0f + adj[1];
				angle2[2] = pbonerot[rotFrame1 + 1].angle[2] / 100.0f + adj[2];
				V_AngleQuaternion (angle2, q2);

				t = (frame - pbonerot[rotFrame1].frame) / (float)(pbonerot[rotFrame1 + 1].frame - pbonerot[rotFrame1].frame);
				V_QuaternionSlerp (q1, q2, t, q[i]);
			}
		}
	}

	{
		for (j = 0; j < m_studioHdr->numbonecontrollers; j++)
		{
			value = (0 / 255.0);//(currententity->controller[j] / 255.0);
			value = (1.0 - value) * pbonecontroller[j].start + value * pbonecontroller[j].end;

			switch (pbonecontroller[j].type)
			{
			case STUDIO_X:
				pos[pbonecontroller[j].bone][0] += value;
				break;
			case STUDIO_Y:
				pos[pbonecontroller[j].bone][1] += value;
				break;
			case STUDIO_Z:
				pos[pbonecontroller[j].bone][2] += value;
				break;
			};
		}
	}

	if (pseqdesc->motiontype & STUDIO_X)
		pos[pseqdesc->motionbone][0] = 0.0;
	if (pseqdesc->motiontype & STUDIO_Y)
		pos[pseqdesc->motionbone][1] = 0.0;
	if (pseqdesc->motiontype & STUDIO_Z)
		pos[pseqdesc->motionbone][2] = 0.0;
}

void StudioRender::R_StudioSetupBones (qEntity_s *ent, qStudioData_s *studioData)
{
	mstudioseqdesc_t	*pseqdesc;
	mstudiobone_t		*pbones;
	double				dfdt, f;
	int					i;

	static vec3_t		pos[MAXSTUDIOBONES];
	//static vec3_t		pos2[MAXSTUDIOBONES];
	static vec4_t		q[MAXSTUDIOBONES];
	//static vec4_t		q2[MAXSTUDIOBONES];
	float				bonematrix[3][4];

	int sequence = ent->m_entityState.m_sequence;

	if ( sequence < 0 || sequence >= m_studioHdr->numseq )
		sequence = 0;

	pseqdesc = (mstudioseqdesc_t *)((byte *)m_studioHdr + m_studioHdr->seqindex) + sequence;

#if 1
	qStudioDrawData_t *drawData = (qStudioDrawData_t *)ent->m_drawData;

	// Make the model loop current sequence
	if ( FBitSet(m_renderFlags, RFL_ANIMATEMODELS) )
		f = fmodf( drawData->m_frametime * pseqdesc->fps, pseqdesc->numframes );
	else
		f = 0;
#else
	{
		// Estimate current frame
		dfdt = (cl.time - currententity->animtime) * currententity->framerate * pseqdesc->fps;

		f = currententity->frame * (pseqdesc->numframes - 1) / 256.0;
		f += dfdt;

		if (pseqdesc->flags & STUDIO_LOOPING)
		{
			if (pseqdesc->numframes > 1)
			{
				f += (int)(f / (pseqdesc->numframes - 1)) * (pseqdesc->numframes - 1);
			}
		}
		else
		{
			if (f < 0.0)
			{
				f = 0.0;
			}

			if (f >= pseqdesc->numframes - 1.001)
			{
				f = pseqdesc->numframes - 1.001;
			}
		}
	}
#endif


	R_StudioCalcRotations (pos, q, sequence, f);


#if 0
	{
		if (currententity->sequencetime && (currententity->sequencetime + 0.2 > cl.time) && (currententity->prevsequence < pstudiohdr->numseq))
		{
			float blend = (cl.time - currententity->sequencetime) / 0.2f;
			float sT = 1.f - blend;

			R_StudioCalcRotations (pos2, q2, currententity->prevsequence, currententity->prevframe);

			for (i=0 ; i < pstudiohdr->numbones ; i++)
			{
				QuaternionSlerp (q[i], q2[i], sT, q[i]);

				pos[i][0] = ( pos2[i][0] * sT ) + ( pos[i][0] * blend );
				pos[i][1] = ( pos2[i][1] * sT ) + ( pos[i][1] * blend );
				pos[i][2] = ( pos2[i][2] * sT ) + ( pos[i][2] * blend );
			}
		}
		else
		{
			currententity->prevframe = f;
		}
	}
#endif


	pbones = (mstudiobone_t *)((byte *)m_studioHdr + m_studioHdr->boneindex);
	for (i=0 ; i < m_studioHdr->numbones ; i++)
	{
		V_QuaternionMatrix (q[i], bonematrix);

		bonematrix[0][3] = pos[i][0];
		bonematrix[1][3] = pos[i][1];
		bonematrix[2][3] = pos[i][2];

		if (pbones[i].parent == -1)
		{
			//memcpy (bonetransform[i], bonematrix, sizeof(bonematrix));
			////memcpy (lighttransform[i], bonematrix, sizeof(bonematrix));
			V_ConcatTransforms (rotationmatrix, bonematrix, bonetransform[i]);
		}
		else
		{
			V_ConcatTransforms (bonetransform[pbones[i].parent], bonematrix, bonetransform[i]);
			//V_ConcatTransforms (lighttransform[pbones[i].parent], bonematrix, lighttransform[i]);
		}
	}
}

void StudioRender::R_StudioTransformAuxVert (auxvert_t *av, int bone, vec3_t vert)
{
	av->fv[0] = DotProduct (vert, bonetransform[bone][0]) + bonetransform[bone][0][3];
	av->fv[1] = DotProduct (vert, bonetransform[bone][1]) + bonetransform[bone][1][3];
	av->fv[2] = DotProduct (vert, bonetransform[bone][2]) + bonetransform[bone][2][3];
}

void StudioRender::R_StudioLighting( qEntity_t *ent, float *lv, int bone, int flags, vec3_t normal )
{
	// Don't update lighting and calculate chrome if we're in "3D Filled Polygons" mode
	if ( FBitSet(m_renderFlags, RFL_NOTEXTURES) )
		return;

	vec3_t norm;
	VectorRotate( normal, bonetransform[bone], norm );

	// Scale the normal if needed
	float scale = ent->m_entityState.m_scale;
	if ( scale != 1.f )
	{
		norm *= scale;
	}

	if ( FBitSet(flags, STUDIO_NF_CHROME) )
	{
		int index = (int)(((byte *)lv - (byte *)pvlightvalues) / 4);

		float v = ( gViewinfo.b.x * norm.x + gViewinfo.b.y * norm.y + gViewinfo.b.z * norm.z ) * 2.0f;

		float s = v * norm.x - gViewinfo.b.x;
		float t = v * norm.z - gViewinfo.b.z;

		// calc s coord
		chrome[index][0] = (int)((s + 1.0f) * 32.0f);
		// calc t coord
		chrome[index][1] = (int)((t + 1.0f) * 32.0f);
	}

	// Don't want to update the lighting in fullbright mode
	if ( !FBitSet(m_renderFlags, RFL_FULLBRIGHT) )
	{
		*lv = PR_CalcLighting( norm.Base() );
	}
}

#if 0
void R_StudioLighting (float *lv, int bone, int flags, vec_t *normal)
{
	float		illum, lambert, lightcos;
	
	float lx = DotProduct (normal, lighttransform[bone][0]);
	float ly = DotProduct (normal, lighttransform[bone][1]);
	float lz = DotProduct (normal, lighttransform[bone][2]);

	illum = (float)r_ambientlight;

	// TODO
	if ((flags & STUDIO_NF_FLATSHADE) != 0 /*&& dword_48F068 != 1*/)
	{
		illum += r_shadelight * 0.5f;
	}
	else
	{
		lambert = 1.7f; //v_lambert.value;
		if (lambert <= 1.0f)
			lambert = 1.0f;

		// TODO
		float dot = r_plightvec[0] * lx +
			r_plightvec[1] * ly +
			r_plightvec[2] * lz;

		lightcos = (dot - (lambert - 1.0f)) / lambert;

		if (lightcos < 0.0f)
			illum -= r_shadelight * lightcos;
	}

	if ((flags & STUDIO_NF_CHROME) != 0)
	{
		int index = (int)(((byte *)lv - (byte *)pvlightvalues) / 4);

		float v = (vpn[0] * lx + vpn[1] * ly + vpn[2] * lz) * 2.0f;

		float s = v * lx - vpn[0];
		float t = v * lz - vpn[2];

		// calc s coord
		chrome[index][0] = (int)((s + 1.0f) * 32.0f);
		// calc t coord
		chrome[index][1] = (int)((t + 1.0f) * 32.0f);
	}

	if (illum > 255)
		illum = 255;

	*lv = lightgammatable[(int)illum * 4] / 1023.0;
}

void R_StudioSetupLighting (alight_t *plighting)
{
	r_ambientlight = plighting->ambientlight;
	r_shadelight = plighting->shadelight;

	//VectorCopy (plighting->plightvec, r_plightvec);
	r_plightvec = vec3_t( plighting->plightvec );

	r_icolormix.r = (int)( (plighting->color[0] * 0xC0FF) ) & 0xFF00;
	r_icolormix.g = (int)( (plighting->color[1] * 0xC0FF) ) & 0xFF00;
	r_icolormix.b = (int)( (plighting->color[2] * 0xC0FF) ) & 0xFF00;
}
#endif

void StudioRender::R_StudioSetupModel (qEntity_t *ent, int bodypart)
{
	int			index;

	if (m_studioHdr->numbodyparts < bodypart)
	{
		Sys_DPrintf ("R_StudioSetupModel: no such bodypart %d\n", bodypart);
		bodypart = 0;
	}

	pbodypart = (mstudiobodyparts_t *)((byte *)m_studioHdr + m_studioHdr->bodypartindex) + bodypart;

	assert( pbodypart->nummodels < MAXSTUDIOMODELS );

	index = ent->m_entityState.m_body / pbodypart->base;
	index = index % pbodypart->nummodels;

	psubmodel = (mstudiomodel_t *)((byte *)m_studioHdr + pbodypart->modelindex) + index;

	assert( psubmodel->nummesh < MAXSTUDIOMESHES );
	assert( psubmodel->numverts < MAXSTUDIOVERTS );
}

#if 0
colorVec R_LightVec (vec3_t start, vec3_t end);
void R_StudioDynamicLight (entity_t *ent, alight_t *plight)
{
	vec3_t		start, dir, end, lorg, lcolor;
	float		floor, dist, add, distscale;
	dlight_t	*dl;
	int			i;
	colorVec	lightColor;

	if (r_fullbright.value)
	{
		plight->shadelight = 0;
		plight->ambientlight = 192;

		plight->color[0] = plight->color[1] = plight->color[2] = 1;

		plight->plightvec[0] = 0;
		plight->plightvec[1] = 0;
		plight->plightvec[2] = -1;
		return;
	}

	dir[0] = 0.f;
	dir[1] = 0.f;
	dir[2] = -1.f;

	VectorCopy(ent->origin, start);
	start[2] -= dir[2] * 8; // TODO(SanyaSho): Revisit this line

	VectorScale(dir, 2048.0f, end);
	VectorAdd(start, end, end);

	lightColor = R_LightVec (start, end);
	currententity->cvFloorColor = lightColor;

	lcolor[0] = lightColor.r;
	lcolor[1] = lightColor.g;
	lcolor[2] = lightColor.b;

	floor = max(max(lcolor[0], lcolor[1]), lcolor[2]);
	if (floor == 0.f)
		floor = 1.f;

	VectorScale(dir, floor, dir);

	dl = cl_dlights;
	for (i=0 ; i<MAX_DLIGHTS ; i++, dl++)
	{
		if (dl->die < cl.time)
			continue;

		VectorSubtract (ent->origin, dl->origin, lorg);
		dist = Length (lorg);

		add = (dl->radius - dist);
		if (add > 0)
		{
			if (dist > 1)
				VectorScale(lorg, (add / dist), lorg);
			else
				VectorScale(lorg, add, lorg);

			floor += add;

			VectorAdd (dir, lorg, dir);

			lcolor[0] += dl->color[0] * (add / 256.0);
			lcolor[1] += dl->color[1] * (add / 256.0);
			lcolor[2] += dl->color[2] * (add / 256.0);
		}
	}

	if (floor >= 128)
		distscale = v_direct.value;
	else
		distscale = (floor * v_direct.value) / 128.0f;

	VectorScale (dir, distscale, dir);

	plight->shadelight = Length (dir);
	plight->ambientlight = (floor - plight->shadelight);

	floor = max(max(lcolor[0], lcolor[1]), lcolor[2]);
	if (floor == 0.f)
		floor = 1.f;

	plight->color[0] = lcolor[0] * (1.f / floor);
	plight->color[1] = lcolor[1] * (1.f / floor);
	plight->color[2] = lcolor[2] * (1.f / floor);

	if (plight->ambientlight > 128)
		plight->ambientlight = 128;
	if (plight->ambientlight + plight->shadelight > 256)
		plight->shadelight = 256 - plight->ambientlight;

	if (ent->effects & EF_MUZZLEFLASH)
	{
		ent->effects &= ~EF_MUZZLEFLASH;

		plight->ambientlight = plight->shadelight;
		plight->shadelight = 80;

		plight->color[0] = 1.0;
		plight->color[1] = 1.0;
		plight->color[2] = 0.6;
	}

	VectorNormalize (dir);
	VectorCopy (dir, plight->plightvec);
}
#endif

//void R_StudioRenderFinal (entity_t *ent, alight_t *plight)
void StudioRender::R_StudioRenderFinal (qEntity_s *ent, qStudioData_s *studioData, int renderFlags/*, alight_t *plight*/)
{
	float		clr, shade;
	int			i;

	PR_GetViewInfo( &gViewinfo );

	m_renderFlags = renderFlags;

	//smodels_total++;

	pauxverts = auxverts;
	pvlightvalues = lightvalues;

	// TODO
	//dword_CD921C = // ?

	//pstudiohdr = (studiohdr_t *)Mod_Extradata( currententity->model );

#if 0
	shade = -(ent->angle[1] / (M_PI*180));
	shadevector[0] = cos (shade);
	shadevector[1] = sin (shade);
	shadevector[2] = 1.f;
	VectorNormalize (shadevector);

	R_StudioSetupLighting (plight);

	glPushMatrix ();
#endif

	R_RotateForEntity (ent, !FBitSet(m_renderFlags, RFL_DISABLESCALING));

#if 0
	if (gl_smoothmodels.value)
		glShadeModel (GL_SMOOTH);

	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (gl_affinemodels.value)
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
#endif

	R_StudioSetupBones(ent, studioData);

	for (i = 0 ; i < m_studioHdr->numbodyparts ; i++)
	{
		R_StudioSetupModel (ent, i);

#if 0
		if (r_dointerp)
			ent->trivial_accept = 0;

		if (ent->rendermode != kRenderModeNormal)
		{
			switch (currententity->rendermode)
			{
			case kRenderModeColor:
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
				break;
			case kRenderModeAdditive:
				glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE);
				glColor4f (1,1,1,r_blend);
				break;
			default:
				glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f (1,1,1,r_blend);
			};

			glEnable (GL_BLEND);
		}
#endif

#if 1
		if ( FBitSet(m_renderFlags, RFL_WIREFRAME) )
			R_GLStudioDrawPointsWireframe (ent, studioData);
		else
			R_GLStudioDrawPoints (ent, studioData/*, plight*/);
#endif

#if 0
		if (r_shadows.value && (ent->rendermode != kRenderModeAdditive))
		{
			clr = 1.0 - (r_blend*0.5);

			glShadeModel (GL_FLAT);

			glDisable (GL_TEXTURE_2D);

			glBlendFunc (GL_ZERO, GL_SRC_COLOR);

			glEnable (GL_BLEND);

			glColor4f (clr,clr,clr,0.5);

			glDepthMask (GL_ZERO);

			glEnable (GL_POLYGON_OFFSET_FILL);

			if (!gl_ztrick.value || gldepthmin < 0.5)
				glPolygonOffset (1,-4);
			else
				glPolygonOffset (1,4);

			GLR_StudioDrawShadow ();

			glDisable (GL_POLYGON_OFFSET_FILL);

			glEnable (GL_TEXTURE_2D);

			glDisable (GL_BLEND);

			glColor4f (1,1,1,1);

			glDepthMask (GL_ONE);

			if (gl_smoothmodels.value)
				glShadeModel (GL_SMOOTH);
		}
#endif
	}

#if 0
	// draw bones
	//if (g_viewerSettings.showBones)
	{
		mstudiobone_t* pbones = (mstudiobone_t*)((byte*)m_studioHdr + m_studioHdr->boneindex);
		//glDisable(GL_DEPTH_TEST);

		for (i = 0; i < m_studioHdr->numbones; i++)
		{
			if (pbones[i].parent >= 0)
			{
				PR_PointSize(3.0f);
				PR_Color4ub(1 * 255, 0.7 * 255, 0, 255);

				PR_Begin(PRIMTYPE_LINES);
				float parentPos[3] = { bonetransform[pbones[i].parent][0][3], bonetransform[pbones[i].parent][1][3], bonetransform[pbones[i].parent][2][3] };
				PR_Vertex3fv(parentPos);
				float bonePos[3] = { bonetransform[i][0][3], bonetransform[i][1][3], bonetransform[i][2][3] };
				PR_Vertex3fv(bonePos);
				PR_End();

				PR_Color4ub(0, 0, 0.8f * 255, 255);

				PR_Begin(PRIMTYPE_POINTS);
				if ( pbones[pbones[i].parent].parent != -1 )
				{
					float newParentPos[3] = { bonetransform[pbones[i].parent][0][3], bonetransform[pbones[i].parent][1][3], bonetransform[pbones[i].parent][2][3] };
					PR_Vertex3fv( newParentPos );
				}
				float newBonePos[3] = { bonetransform[i][0][3], bonetransform[i][1][3], bonetransform[i][2][3] };
				PR_Vertex3fv(newBonePos);
				PR_End();
			}
			else
			{
				// draw parent bone node
				PR_PointSize(5.0f);
				PR_Color4ub(0.8f * 255, 0, 0, 255);
				PR_Begin(PRIMTYPE_POINTS);
				float bonePos[3] = { bonetransform[i][0][3], bonetransform[i][1][3], bonetransform[i][2][3] };
				PR_Vertex3fv( bonePos );
				PR_End();
			}
		}

		PR_PointSize(1.0f);
	}
#endif

#if 0
	if (ent->rendermode != kRenderModeNormal)
		glDisable (GL_BLEND);

	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glShadeModel (GL_FLAT);

	if (gl_affinemodels.value)
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPopMatrix ();
#endif
}

//void R_GLStudioDrawPoints (entity_t *ent, alight_t *plight)
void StudioRender::R_GLStudioDrawPoints (qEntity_s *ent, qStudioData_s *studioData/*, alight_t *plight*/)
{
	byte				*pvertbone, *pnormbone;
	mstudiomodeldata_t	*pmodeldata;
	vec3_t				*pstudioverts, *pstudionorms;
	mstudiotexture_t	*ptexture;
	mstudiotrivert_t	*ptricmds;
	auxvert_t			*av;
	float				*lv;
	int					flags;
	float				ss, st;
	float				l;
	int					i, j, t, k;

	pvertbone = (byte *)m_studioHdr + psubmodel->vertinfoindex;
	pnormbone = (byte *)m_studioHdr + psubmodel->norminfoindex;

	pmodeldata = (mstudiomodeldata_t *)((byte *)m_studioHdr + psubmodel->modeldataindex);
	pstudioverts = (vec3_t *)((byte *)m_studioHdr + pmodeldata->vertindex);
	pstudionorms = (vec3_t *)((byte *)m_studioHdr + pmodeldata->normindex);

	ptexture = (mstudiotexture_t *)((byte *)m_studioHdr + m_studioHdr->textureindex);

	pmesh = (mstudiomesh_t *)((byte *)m_studioHdr + psubmodel->meshindex);
	ptricmds = (mstudiotrivert_t*)((byte*)m_studioHdr + pmesh->triindex);

	r_anumverts = psubmodel->numverts;

	for (i=0 ; i<r_anumverts ; i++)
	{
		av = &pauxverts[i];
		R_StudioTransformAuxVert (av, pvertbone[i], pstudioverts[i]);
	}

	lv = (float *)pvlightvalues;
	for (i=0 ; i<psubmodel->nummesh ; i++)
	{
		flags = ptexture[pmesh[i].skinref].flags;

		for (j=0 ; j<pmesh[i].numnorms ; j++, lv++, pstudionorms++, pnormbone++)
		{
			R_StudioLighting (ent, lv, *pnormbone, flags, pstudionorms->Base());
		}
	}

	// TODO
	//dword_CD9238 = 1;

	byte alpha = 255;
	int renderFlags = 0;

	if ( FBitSet(ent->m_editorFlags, EFL_TRANSPARENT) )
	{
		byte minAlpha = (byte)( PR_GetMinAlpha() * 255.f );
		alpha = std::min( minAlpha, ent->m_renderMode.m_renderColor.a );

		renderFlags = ent->m_renderMode.m_renderFlags;
	}

	if ( ( m_renderFlags & RFL_NOTEXTURES | RFL_FULLBRIGHT ) == RFL_FULLBRIGHT && alpha != 255 )
	{
		PR_Color4ub( 255, 255, 255, alpha );
	}

	for (i=0 ; i<psubmodel->nummesh ; i++, pmesh++)
	{
		ss = 1.0f / (float)ptexture[pmesh->skinref].width;
		st = 1.0f / (float)ptexture[pmesh->skinref].height;

#if 1
		PR_SetState( renderFlags | GLS_DEPTHWRITE );
		PR_BindShader( m_textureList[ptexture[pmesh->skinref].index] );
#else
		GL_Bind (ptexture[pmesh->skinref].index);
#endif

#if 1
		for (t=0 ; t<pmesh->numtris ; t++, ptricmds += 3)
		{
			PR_Begin( PRIMTYPE_TRIANGLES );

			for (k=0 ; k < 3 ; ++k)
			{
				// Don't want to update the lighting in fullbright mode
				if ( !FBitSet(m_renderFlags, RFL_NOTEXTURES | RFL_FULLBRIGHT) )
				{
					l = (*pvlightvalues)[ptricmds[k].normindex] * 255.f;
					PR_Color4ub( (byte)l, (byte)l, (byte)l, alpha );
				}

				if ((ptexture[pmesh->skinref].flags & STUDIO_NF_CHROME) != 0)
				{
					PR_TexCoord2f(
						chrome[ptricmds[k].normindex][0] * ss,
						chrome[ptricmds[k].normindex][1] * st
					);
				}
				else
				{
					PR_TexCoord2f(
						ptricmds[k].s * ss,
						ptricmds[k].t * st
					);
				}

				PR_Vertex3fv(
					pauxverts[ptricmds[k].vertindex].fv
				);
			}

			PR_End();
		}
#else
		if ((ptexture[pmesh->skinref].flags & STUDIO_NF_CHROME) != 0)
		{
			for (t=0 ; t<pmesh->numtris ; t++, ptricmds += 3)
			{
				glBegin (GL_TRIANGLES);


				/// vert1
				glTexCoord2f (
					chrome[ptricmds[0].normindex][0] * ss,
					chrome[ptricmds[0].normindex][1] * st
				);
				l = (*pvlightvalues)[ptricmds[0].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[0].vertindex].fv[0],
					pauxverts[ptricmds[0].vertindex].fv[1],
					pauxverts[ptricmds[0].vertindex].fv[2]
				);
				/// vert1


				/// vert2
				glTexCoord2f (
					chrome[ptricmds[1].normindex][0] * ss,
					chrome[ptricmds[1].normindex][1] * st
				);
				l = (*pvlightvalues)[ptricmds[1].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[1].vertindex].fv[0],
					pauxverts[ptricmds[1].vertindex].fv[1],
					pauxverts[ptricmds[1].vertindex].fv[2]
				);
				/// vert2


				/// vert3
				glTexCoord2f (
					chrome[ptricmds[2].normindex][0] * ss,
					chrome[ptricmds[2].normindex][1] * st
				);
				l = (*pvlightvalues)[ptricmds[2].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[2].vertindex].fv[0],
					pauxverts[ptricmds[2].vertindex].fv[1],
					pauxverts[ptricmds[2].vertindex].fv[2]
				);
				/// vert3


				glEnd ();
			}
		}
		else*/
		{
			for (t=0 ; t<pmesh->numtris ; t++, ptricmds += 3)
			{
				glBegin (GL_TRIANGLES);


				/// vert1
				glTexCoord2f (
					(float)ptricmds[0].s * ss,
					(float)ptricmds[0].t * st
				);
				l = (*pvlightvalues)[ptricmds[0].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[0].vertindex].fv[0],
					pauxverts[ptricmds[0].vertindex].fv[1],
					pauxverts[ptricmds[0].vertindex].fv[2]
				);
				/// vert1


				/// vert2
				glTexCoord2f (
					(float)ptricmds[1].s * ss,
					(float)ptricmds[1].t * st
				);
				l = (*pvlightvalues)[ptricmds[1].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[1].vertindex].fv[0],
					pauxverts[ptricmds[1].vertindex].fv[1],
					pauxverts[ptricmds[1].vertindex].fv[2]
				);
				/// vert2


				/// vert3
				glTexCoord2f (
					(float)ptricmds[2].s * ss,
					(float)ptricmds[2].t * st
				);
				l = (*pvlightvalues)[ptricmds[2].normindex];
				glColor4f (
					plight->color[0] * l,
					plight->color[1] * l,
					plight->color[2] * l,
					r_blend
				);
				glVertex3f (
					pauxverts[ptricmds[2].vertindex].fv[0],
					pauxverts[ptricmds[2].vertindex].fv[1],
					pauxverts[ptricmds[2].vertindex].fv[2]
				);
				/// vert3


				glEnd();
			}
		}
#endif
	}
}

/*
==================
R_GLStudioDrawPointsWireframe
Custom function used to draw 2D viewport meshes
Does not bind any shaders and does not calculate chrome
==================
*/
void StudioRender::R_GLStudioDrawPointsWireframe (qEntity_s *ent, qStudioData_s *studioData/*, alight_t *plight*/)
{
	byte				*pvertbone, *pnormbone;
	mstudiomodeldata_t	*pmodeldata;
	vec3_t				*pstudioverts;
	mstudiotexture_t	*ptexture;
	mstudiotrivert_t	*ptricmds;
	float				ss, st;
	int					i, t, k;

	pvertbone = (byte *)m_studioHdr + psubmodel->vertinfoindex;
	pnormbone = (byte *)m_studioHdr + psubmodel->norminfoindex;
	
	pmodeldata = (mstudiomodeldata_t *)((byte *)m_studioHdr + psubmodel->modeldataindex);
	pstudioverts = (vec3_t *)((byte *)m_studioHdr + pmodeldata->vertindex);

	ptexture = (mstudiotexture_t *)((byte *)m_studioHdr + m_studioHdr->textureindex);

	pmesh = (mstudiomesh_t *)((byte *)m_studioHdr + psubmodel->meshindex);
	ptricmds = (mstudiotrivert_t*)((byte*)m_studioHdr + pmesh->triindex);

	// We still want to build auxvert even in 2D view
	for (i=0 ; i<psubmodel->numverts ; i++)
	{
		R_StudioTransformAuxVert (&auxverts[i], pvertbone[i], pstudioverts[i]);
	}

	for (i=0 ; i<psubmodel->nummesh ; i++, pmesh++)
	{
		ss = 1.0f / (float)ptexture[pmesh->skinref].width;
		st = 1.0f / (float)ptexture[pmesh->skinref].height;

		for (t=0 ; t < pmesh->numtris ; t++, ptricmds += 3)
		{
			PR_Begin( PRIMTYPE_TRIANGLES );

			for (k=0 ; k < 3 ; ++k)
			{
				PR_TexCoord2f(
					ptricmds[k].s * ss,
					ptricmds[k].t * st
				);

				PR_Vertex3fv(
					auxverts[ptricmds[k].vertindex].fv
				);
			}

			PR_End();
		}
	}
}

#if 0
void GLR_StudioDrawShadow()
{
	// TODO
	mstudiotrivert_t *ptricmds;

	float lheight = currententity->origin[2] - lightspot[2];

	for ( int i = 0; i < psubmodel->nummesh; i++ )
	{
		pmesh = (mstudiomesh_t *)((byte *)pstudiohdr + psubmodel->meshindex) + i;
		ptricmds = (mstudiotrivert_t *)((byte *)pstudiohdr + pmesh->triindex);

		for ( int j = 0; j < pmesh->numtris * 3; j++ )
		{
			glBegin( GL_TRIANGLES );

			for ( int k = 0; k < 3; k++ )
			{
				auxvert_t *pvert = &pauxverts[ptricmds[k].vertindex];

				vec3_t drawvert;
				drawvert[0] = pvert->fv[0];
				drawvert[1] = pvert->fv[1];

				float verz = pvert->fv[2];
				float shadowZ = 1.0f - lheight;
				drawvert[2] = shadowZ;

				float offset = lheight + verz;
				drawvert[0] -= offset * shadevector[0];
				drawvert[1] -= offset * shadevector[1];

				glVertex3fv( drawvert );
			}

			glEnd();

			ptricmds++;
		}
	}
}
#endif

void StudioRender::GetModelBounds( qEntity_t *ent, int flags, vec3_t *bboxMin, vec3_t *bboxMax )
{
	if ( !ent )
	{
		if ( bboxMin )
			*bboxMin = vec3_t( -16, -16, 0 );
		if ( bboxMax )
			*bboxMax = vec3_t( 16, 16, 72 );
		return;
	}

	mstudioseqdesc_t	*pseqdesc;
	mstudiobone_t		*pbones;
	double				dfdt, f;
	int					i, j, k;

	mstudiobodyparts_t	*pbodypart;
	mstudiomodel_t		*psubmodel;

	byte				*pvertbone;
	mstudiomodeldata_t	*pmodeldata;
	vec3_t				*pstudioverts;

	static vec3_t		pos[MAXSTUDIOBONES];
	static vec4_t		q[MAXSTUDIOBONES];
	float				bonematrix[3][4];

	//R_RotateForEntity (ent, !FBitSet(m_renderFlags, RFL_DISABLESCALING));

	int sequence = ent->m_entityState.m_sequence;

	if ( sequence < 0 || sequence >= m_studioHdr->numseq )
		sequence = 0;

	pseqdesc = (mstudioseqdesc_t *)((byte *)m_studioHdr + m_studioHdr->seqindex) + sequence;

	qStudioDrawData_t *drawData = (qStudioDrawData_t *)ent->m_drawData;

	// Make the model loop current sequence
	if ( FBitSet(m_renderFlags, RFL_ANIMATEMODELS) )
		f = fmodf( drawData->m_frametime * pseqdesc->fps, pseqdesc->numframes );
	else
		f = 0;

	R_StudioCalcRotations (pos, q, sequence, f);

	pbones = (mstudiobone_t *)((byte *)m_studioHdr + m_studioHdr->boneindex);
	for (i=0 ; i < m_studioHdr->numbones ; i++)
	{
		V_QuaternionMatrix (q[i], bonematrix);

		bonematrix[0][3] = pos[i][0];
		bonematrix[1][3] = pos[i][1];
		bonematrix[2][3] = pos[i][2];

		if (pbones[i].parent == -1)
		{
			memcpy (bonetransform[i], bonematrix, sizeof(bonematrix));
			////memcpy (lighttransform[i], bonematrix, sizeof(bonematrix));
			//V_ConcatTransforms (rotationmatrix, bonematrix, bonetransform[i]);
		}
		else
		{
			V_ConcatTransforms (bonetransform[pbones[i].parent], bonematrix, bonetransform[i]);
			//V_ConcatTransforms (lighttransform[pbones[i].parent], bonematrix, lighttransform[i]);
		}
	}

	vec3_t mins( FLT_MAX, FLT_MAX, FLT_MAX );
	vec3_t maxs( -FLT_MAX, -FLT_MAX, -FLT_MAX );

	pbodypart = (mstudiobodyparts_t *)((byte *)m_studioHdr + m_studioHdr->bodypartindex);
	for (i=0 ; i < m_studioHdr->numbodyparts ; i++, pbodypart++)
	{
		psubmodel = (mstudiomodel_t *)((byte *)m_studioHdr + pbodypart->modelindex);
		for (j=0 ; j < pbodypart->nummodels ; j++, psubmodel++)
		{
			pvertbone = (byte *)m_studioHdr + psubmodel->vertinfoindex;

			pmodeldata = (mstudiomodeldata_t *)((byte *)m_studioHdr + psubmodel->modeldataindex);
			pstudioverts = (vec3_t *)((byte *)m_studioHdr + pmodeldata->vertindex);

			for (k=0 ; k < psubmodel->numverts; k++)
			{
				vec3_t modelPos;
				VectorTransform( pstudioverts[k], bonetransform[pvertbone[k]], modelPos );

				V_AddPointToBounds( modelPos, mins, maxs );
			}
		}
	}

	if ( bboxMin )
		*bboxMin = mins;
	if ( bboxMax )
		*bboxMax = maxs;
}

void StudioRender::R_RotateForEntity( qEntity_t *ent, bool doscale /* = false */ )
{
	float scale = 1.f;
	if ( doscale )
	{
		scale = ent->m_entityState.m_scale;
	}

	vec3_t angles = ent->m_vecAngles;

	if ( FBitSet( m_renderFlags, RFL_INVERTSTUDIOPITCH ) )
	{
		angles[PITCH] *= -1;
	}

	V_BuildTransformStudioMatrix( angles[YAW], angles[PITCH], angles[ROLL], ent->m_vecOrigin, scale, rotationmatrix );
}
