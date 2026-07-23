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

/*
===============
RMFSerializer
===============
*/
RMFSerializer::RMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
	: Serializer( filePath, seekOffset, readLimit, worldDef, FMODE_STDIO_BINARY )
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
	if ( m_fileHandle )
	{
		if ( m_writeMode )
		{
			fflush( m_fileHandle );
		}

		fclose( m_fileHandle );
		m_fileHandle = NULL;
	}
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
