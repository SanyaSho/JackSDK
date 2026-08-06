//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_RMF_H )
#define SERIALIZER_RMF_H

#define JHMFHEADER	(('F'<<24)+('M'<<16)+('H'<<8)+'J')	// little-endian "JHMF"

class Serializer;

class CBackgroundImage;
class CVisGroup;

class JMFSerializer : public Serializer
{
public:
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	JMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
	JMFSerializer( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	virtual ~JMFSerializer();

	virtual bool Export();
	virtual bool Import();

protected:
	bool WriteString( const char *data );
	bool ReadString( char **outBuf );

	bool WriteStringVersion( const char *data, int jmfVersion );
	bool ReadStringVersion( char **outBuf, int jmfVersion );

protected:
	bool ProcessHeader();

	bool SerializeBackground( CBackgroundImage *bgImage );
	bool SerializeGroup( qGroup_s *groupDef );
	bool SerializeVisGroup( CVisGroup *visGroupDef );
	bool SerializeCamera( qCamera_s *cameraDef );
	bool SerializePath( qPath_s *pathDef );
	bool SerializeNode( qNode_s *nodeDef, qPath_s *pathDef );
	bool SerializeEntity( qEntity_s *entityDef );
	bool SerializeBrush( qBrush_s *brushDef, qEntity_s *entityDef );
	bool SerializeFace( qFace_s *faceDef, qBrush_s *brushDef );

private:
	CMapWorld *m_mapWorld;
	int m_jmfVersion;
};

#endif // !SERIALIZER_RMF_H
