//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_RMF_H )
#define SERIALIZER_RMF_H

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
extern bool ExportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
extern bool ImportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
extern bool ExportRMF( const char *filePath, struct qWorld_s *worldDef );
extern bool ImportRMF( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

class Serializer;

class RMFSerializer : public Serializer
{
public:
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	RMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
	RMFSerializer( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	virtual ~RMFSerializer();

public: // Serializer
	virtual bool Export();
	virtual bool Import();

protected:
	int m_rmfVersion;
};

#endif // !SERIALIZER_RMF_H
