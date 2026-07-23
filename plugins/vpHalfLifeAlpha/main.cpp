//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>

// Plugin API
#include "PluginMeta.h"

plugin_funcs_t gEditorfuncs;

/*
===============
vpMain
Application entry point
===============
*/
DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
{
	if ( editorFuncs->m_intefaceVersion < sizeof( plugin_funcs_t ) )
		return -1;

	if ( editorPluginVersion != PLUGIN_VERSION )
		return PLUGIN_VERSION;

	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->m_intefaceVersion );
	setlocale( LC_ALL, "C" );
	return 0;
}
