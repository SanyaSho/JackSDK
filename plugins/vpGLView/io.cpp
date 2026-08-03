//=====================================================================================//
//
// Purpose: GLView plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <algorithm>

// Plugin API
#include "PluginMeta.h"

#include "DataSerializer.h"
#include "serializer_gl.h"

#include "vstdlib_static.h"

/*
===============
vpEnumImportFormats
===============
*/
DLL_EXPORT int vpEnumImportFormats( pfnRegisterIOFormat registerIOFormat, void *libraryHandle )
{
	return registerIOFormat( 0, "GLView", ".gl", libraryHandle ) ? 1 : 0;
}

/*
===============
vpImport
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
DLL_EXPORT bool vpImport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
#else
DLL_EXPORT bool vpImport( int formatIndex, const char *filePath, qWorld_s *worldDef )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
	if ( formatIndex != 0 )
	{
		return false;
	}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	return ImportGL( filePath, seekOffset, readLimit, worldDef );
#else
	return ImportGL( filePath, worldDef );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
}
