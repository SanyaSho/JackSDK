//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPREADER_H )
#define MAPREADER_H

#include "BaseTypes.h"

struct qEntity_s;
struct qBrush_s;
struct qWorld_s;

class MapReader
{
public:
	MapReader( const char *filePath, long seekOffset, long readLimit, qWorld_s *worldDef );
	virtual ~MapReader();

	bool LoadMap();

protected:
	bool ParseMap();
	bool ParseBrushes( qEntity_s *parentEntity );
	bool ParseFaces( qBrush_s *brushOwner );

private:
	char m_filePath[MAX_PATH];
	long m_seekOffset;
	long m_readLimit;

	int m_invalidSolids;

	qWorld_s *m_world;

	int m_mapVersion;
};

#endif // !MAPREADER_H