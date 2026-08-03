//=====================================================================================//
//
// Purpose: GLView plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SERIALIZER_GL_H )
#define SERIALIZER_GL_H

#include <vector>

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
extern bool ExportGL( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
extern bool ImportGL( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
extern bool ExportGL( const char *filePath, struct qWorld_s *worldDef );
extern bool ImportGL( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

class Serializer;

class GLSerializer : public Serializer
{
public:
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	GLSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef );
#else
	GLSerializer( const char *filePath, struct qWorld_s *worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	virtual ~GLSerializer();

public: // Serializer
	virtual bool Export();
	virtual bool Import();

protected:
	void SerializeGLViewFile( qEntity_s *worldSpawn, bool invert );

protected:
};

#endif // !SERIALIZER_GL_H
