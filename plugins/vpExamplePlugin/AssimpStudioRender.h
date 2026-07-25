//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( ASSIMPSTUDIORENDER_H )
#define ASSIMPSTUDIORENDER_H

#include "BaseTypes.h"

struct qStudioData_s;
struct qEntity_s;
struct qShader_s;

namespace Assimp
{
class Importer;
};

struct aiScene;
struct aiMesh;

class AssimpStudioRender
{
public:
	AssimpStudioRender();
	virtual ~AssimpStudioRender();

	bool LoadModel( const char *filePath, byte *buf, int bufSize, const char *hint, qStudioData_s *studioData );
	void GetModelBounds( int flags, vec3_t *bboxMin, vec3_t *bboxMax );

	void RenderModel( qEntity_s *ent, qStudioData_s *studioData, int renderFlags );
	void DrawMesh( qEntity_s *ent, const aiMesh *mesh );

	void R_RotateForEntity( qEntity_s *ent );

private:
	Assimp::Importer *m_importer;
	const aiScene *m_scene;

	qShader_s **m_textureList;

	int m_renderFlags;
};

#endif // !ASSIMPSTUDIORENDER_H
