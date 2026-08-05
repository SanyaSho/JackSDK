//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#include "PluginMeta.h"

#include "DataSerializer.h"
#include "serializer_rmf.h"

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
===============
ExportRMF
===============
*/
bool ExportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	RMFSerializer rmfSerializer( filePath, seekOffset, readLimit, worldDef );
	return rmfSerializer.Export();
}

/*
===============
ImportRMF
===============
*/
bool ImportRMF( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	RMFSerializer rmfSerializer( filePath, seekOffset, readLimit, worldDef );
	return rmfSerializer.Import();
}
#else
/*
===============
ExportRMF
===============
*/
bool ExportRMF( const char *filePath, struct qWorld_s *worldDef )
{
	RMFSerializer rmfSerializer( filePath, worldDef );
	return rmfSerializer.Export();
}

/*
===============
ImportRMF
===============
*/
bool ImportRMF( const char *filePath, struct qWorld_s *worldDef )
{
	RMFSerializer rmfSerializer( filePath, worldDef );
	return rmfSerializer.Import();
}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

/*
===============
RMFSerializer
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
RMFSerializer::RMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
	: Serializer( filePath, seekOffset, readLimit, worldDef, FMODE_STDIO_BINARY )
#else
RMFSerializer::RMFSerializer( const char *filePath, struct qWorld_s *worldDef )
	: Serializer( filePath, worldDef, FMODE_STDIO_BINARY )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
	m_rmfVersion = 0;
}

/*
===============
~RMFSerializer
===============
*/
RMFSerializer::~RMFSerializer()
{
}

/*
===============
Export
===============
*/
bool RMFSerializer::Export()
{
	return false;
}

/*
===============
Import
===============
*/
bool RMFSerializer::Import()
{
	return false;
}
