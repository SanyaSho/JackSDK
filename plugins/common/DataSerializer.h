//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( DATASERIALIZER_H )
#define DATASERIALIZER_H

#define FMODE_STDIO_BINARY 0
#define FMODE_STDIO 1
#define FMODE_PARSERAPI 2

class Serializer
{
public:
	Serializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef, int fileMode );
	virtual ~Serializer();

	bool OpenForWrite();
	bool OpenForRead();

	bool WriteData( const void *data, size_t n );
	bool ReadData( void *data, size_t n, bool showerror = true );

	virtual bool Export() = 0;
	virtual bool Import() = 0;

protected:
	parser_api_t *m_parser;

	struct qWorld_s *m_worldDef;

	FILE *m_fileHandle;

	size_t m_seekOffset;
	size_t m_currentOffset;
	size_t m_readLimit;

	int m_fileMode;

	bool m_writeMode;

	char m_filePath[MAX_PATH];
};

#endif // !DATASERIALIZER_H
