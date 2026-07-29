//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>

// Plugin API
#include "PluginMeta.h"

#include <pybind11/embed.h>
namespace py = pybind11;

#include "pythonhost.h"
static PythonHost gPythonHost;

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

	gPythonHost.Initialize();
	gPythonHost.LoadScripts( "E:\\SteamLibrary\\steamapps\\common\\JACK\\plugins\\pythonscripts" );

	return 0;
}
