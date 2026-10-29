//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MEM_H )
#define MEM_H

extern void *Sys_Malloc( size_t size );
extern void Sys_Free( void *ptr );

extern char *Sys_AllocString( const char *src );

#endif // !MEM_H
