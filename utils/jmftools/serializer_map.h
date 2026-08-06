//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_MAP_H )
#define SERIALIZER_MAP_H

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
extern bool ExportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
extern bool ImportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
extern bool ExportMAP( const char *filePath, struct qWorld_s *worldDef );
extern bool ImportMAP( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

class Serializer;

class MAPSerializer : public Serializer
{
public:
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	MAPSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef, int fileMode );
#else
	MAPSerializer( const char *filePath, struct qWorld_s *worldDef, int fileMode );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
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
