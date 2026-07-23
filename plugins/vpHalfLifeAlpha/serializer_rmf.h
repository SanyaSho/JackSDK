//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_RMF_H )
#define SERIALIZER_RMF_H

extern bool ExportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
extern bool ImportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );

class Serializer;

class RMFSerializer : public Serializer
{
public:
	RMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
	virtual ~RMFSerializer();

public: // Serializer
	virtual bool Export();
	virtual bool Import();

protected:
	int m_rmfVersion;
};

#endif // !SERIALIZER_RMF_H
