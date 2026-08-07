//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( LOGGER_H )
#define LOGGER_H

extern void Sys_Printf( const char *format, ... );
extern void Sys_DPrintf( const char *format, ... );
extern void Sys_Warning( const char *format, ... );
extern void Sys_Error( const char *format, ... );

#endif // !LOGGER_H
