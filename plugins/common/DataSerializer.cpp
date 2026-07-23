//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>

// Plugin API
#include "PluginAPI.h"

#include "DataSerializer.h"

/*
===============
Serializer
===============
*/
Serializer::Serializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef, int fileMode )
{
	m_parser = &gEditorfuncs.parserfuncs;

	m_worldDef = worldDef;

	m_fileHandle = NULL;

	m_seekOffset = seekOffset;
	m_currentOffset = 0;
	m_readLimit = readLimit;

	m_fileMode = fileMode;

	m_writeMode = false;

	m_filePath[0] = '\0';

	strncpy( m_filePath, filePath, sizeof( m_filePath ) );
}

/*
===============
~Serializer
===============
*/
Serializer::~Serializer()
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
OpenForWrite
===============
*/
bool Serializer::OpenForWrite()
{
	char errorStr[256] = { 0 };

	m_writeMode = true;

	assert( m_fileMode == FMODE_PARSERAPI );

	m_fileHandle = fopen( Sys_MakeLocalFileName( m_filePath ), m_fileMode == FMODE_STDIO_BINARY ? "wb" : "w" );
	if ( !m_fileHandle )
	{
		strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
		Sys_Error( "can't write \"%s\" (%s)", m_filePath, errorStr );
		return false;
	}

	return true;
}

/*
===============
OpenForRead
===============
*/
bool Serializer::OpenForRead()
{
	char errorStr[256] = { 0 };

	m_writeMode = false;

	switch ( m_fileMode )
	{
	case FMODE_STDIO_BINARY:
	case FMODE_STDIO:
	{
		m_fileHandle = fopen( Sys_MakeLocalFileName( m_filePath ), m_fileMode == FMODE_STDIO_BINARY ? "rb" : "r" );
		if ( !m_fileHandle )
		{
			strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
			Sys_Error( "can't read \"%s\" (%s)", m_filePath, errorStr );
			return false;
		}

		if ( m_seekOffset != 0 && fseek( m_fileHandle, m_seekOffset, SEEK_SET ) != 0 )
		{
			strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
			Sys_Error( "can't read \"%s\" (%s)", m_filePath, errorStr );
			return false;
		}
		break;
	}
	case FMODE_PARSERAPI:
	{
		if ( !m_parser->pfnSC_ParseFromFile( m_filePath, m_seekOffset, m_readLimit, 0 ) )
		{
			strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
			Sys_Error( "can't read \"%s\" (%s)", m_filePath, errorStr );
			return false;
		}
		break;
	}
	};

	return true;
}

/*
===============
WriteData
===============
*/
bool Serializer::WriteData( const void *data, size_t n )
{
	char errorStr[256] = { 0 };

	assert( m_fileHandle );
	assert( m_writeMode );

	if ( !m_fileHandle )
		return false;

	if ( m_readLimit != 0 && ( n + m_currentOffset ) > m_readLimit )
		return false;

	if ( fwrite( data, 1, n, m_fileHandle ) != n )
	{
		strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
		Sys_Error( "can't write %Iu bytes to file \"%s\" (%s)", n, m_filePath, errorStr );
		return false;
	}

	m_currentOffset += n;

	return true;
}

/*
===============
ReadData
===============
*/
bool Serializer::ReadData( void *data, size_t n, bool showerror /*= true*/ )
{
	char errorStr[256] = { 0 };

	assert( m_fileHandle );
	assert( !m_writeMode );

	if ( !m_fileHandle )
		return false;

	if ( m_readLimit != 0 && ( n + m_currentOffset ) > m_readLimit )
		return false;

	if ( fread( data, 1, n, m_fileHandle ) != n )
	{
		if ( showerror )
		{
			strncpy( errorStr, strerror( errno ), sizeof( errorStr ) );
			Sys_Error( "can't read %Iu bytes from file \"%s\" (%s)", n, m_filePath, errorStr );
		}

		return false;
	}

	m_currentOffset += n;

	return true;
}
