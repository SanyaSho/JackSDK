//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_MAP_H )
#define SERIALIZER_MAP_H

extern bool ExportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
extern bool ImportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );

class Serializer;

class MAPSerializer : public Serializer
{
public:
	MAPSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef, int fileMode );
	virtual ~MAPSerializer();

public: // Serializer
	virtual bool Export();
	virtual bool Import();

protected:
	void SerializeCordon();
	bool SerializeBrushFaces( struct qFace_s *faceDef, struct qBrush_s *brushDef );
	bool SerializeBrushes( struct qBrush_s *brushDef, struct qEntity_s *entityDef );
	bool SerializeEntities( struct qEntity_s *entityDef );
	bool SerializePathNodes( struct qPath_s *pathList );

protected:
	int m_mapVersion;
	int m_numInvalidSolid;
	char *m_packageList;
	bool m_cordon;
};

#endif // !SERIALIZER_MAP_H
