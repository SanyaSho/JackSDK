//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPFACE_H )
#define MAPFACE_H

#include "PluginAPI.h"

class CMapFace
{
public:
	CMapFace( int vertexCount );
	virtual ~CMapFace();

	void setTexDef( const qTexDef_s &texDef );

	static qFace_s *create( int vertexCount )
	{
		CMapFace *mapFace = new CMapFace( vertexCount );
		SetBits( mapFace->m_faceData->m_editorFlags, EFL_DIRTY );
		return mapFace->m_faceData;
	}

public:
	qFace_s *m_faceData;
	int m_selectionId;
	rgba_t m_cbColor;
	float m_unknownFloat;
	int m_unknownInt;
	char gap[16];
};

#endif // !MAPFACE_H
