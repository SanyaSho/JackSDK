//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPCAMERA_H )
#define MAPCAMERA_H

#include "PluginCamera.h"

class CMapCamera
{
public:
	CMapCamera();
	virtual ~CMapCamera();

	void setup( const vec3_t &origin, const vec3_t &angles );

	static qCamera_s *create()
	{
		CMapCamera *mapCamera = new CMapCamera;
		return mapCamera->m_cameraData;
	}

public:
	qCamera_t *m_cameraData;
	int m_selectionId;
	rgba_t m_cbColor1;
	rgba_t m_cbColor2;
	vec3_t m_vecAngles;
	float m_length;
};

#endif // !MAPCAMERA_H
