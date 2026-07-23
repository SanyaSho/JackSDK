#if !defined( R_STUDIO_H )
#define R_STUDIO_H

typedef struct
{
	float fv[3];
} auxvert_t;

#if 0
typedef struct alight_s
{
	int ambientlight;		// max 128
	int shadelight;
	vec3_t color;
	float *plightvec;
} alight_t;
#endif

//extern int r_dointerp;

#include "studio.h"

#if 0

// l_studio.c
void Mod_LoadStudioModel (model_t *mod, void *buffer);

// r_studio.c
qboolean R_StudioCheckBBox (void);
int R_StudioBodyVariations (model_t *model);
void R_StudioCalcRotations (vec3_t *pos, vec4_t *q, int sequence, float f);
void R_StudioSetupBones (void);
void R_StudioTransformAuxVert (auxvert_t *av, int bone, vec_t *vert);
void R_StudioLighting (float *lv, int bone, int flags, vec_t *normal);
void R_StudioSetupLighting (alight_t *plighting);
void R_StudioSetupModel (int bodypart);
void R_StudioDynamicLight (entity_t *ent, alight_t *plight);
void R_StudioRenderFinal (entity_t *ent, alight_t *plight);
void R_GLStudioDrawPoints (entity_t *ent, alight_t *plight);
void GLR_StudioDrawShadow (void);

#endif //

class StudioRender
{
public:
	StudioRender();
	virtual ~StudioRender();

	// l_studio.c
	bool Mod_LoadStudioModel( const char *filePath, byte *buf, int bufSize, qStudioData_s *outStudioData );

	// r_studio.c
	void R_StudioCalcRotations( vec3_t *pos, vec4_t *q, int sequence, float f );
	void R_StudioSetupBones( qEntity_s *ent, qStudioData_s *studioData );
	void R_StudioTransformAuxVert( auxvert_t *av, int bone, vec3_t vert );
	void R_StudioLighting( qEntity_t *ent, float *lv, int bone, int flags, int normidx, vec3_t normal );
	void R_StudioSetupModel( qEntity_t *ent, int bodypart );
	void R_StudioRenderFinal( qEntity_s *ent, qStudioData_s *studioData, int renderFlags /*, alight_t *plight*/ );
	void R_GLStudioDrawPoints( qEntity_s *ent, qStudioData_s *studioData /*, alight_t *plight*/ );
	void R_GLStudioDrawPointsWireframe( qEntity_s *ent, qStudioData_s *studioData /*, alight_t *plight*/ );

	void GetModelBounds( int flags, vec3_t *bboxMin, vec3_t *bboxMax );

	void R_RotateForEntity( qEntity_t *ent, bool doscale = false );

private:
	studiohdr_t *m_studioHdr;
	mstudiobodyparts_t *m_bodyPart;
	mstudiomodel_t *m_subModel;
	mstudiomesh_t *m_mesh;

	qShader_t **m_textureList;

	int m_renderFlags;

	float m_bonetransform[MAXSTUDIOBONES][3][4];
	float m_rootbonetransform[3][4]; // Position and angles of the root bone
	auxvert_t m_auxverts[MAXSTUDIOVERTS];
	int m_chrome[MAXSTUDIOVERTS][2];
	float m_vlightvalues[MAXSTUDIOVERTS];
};

#endif // R_STUDIO_H