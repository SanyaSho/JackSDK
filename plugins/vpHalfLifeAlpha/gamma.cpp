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

#include "gamma.h"

byte g_gammatable[256];

/*
===============
BuildGammaTable
===============
*/
void BuildGammaTable()
{
	static bool g_built_gamma_table = false;
	if ( g_built_gamma_table )
		return;
	g_built_gamma_table = true;

	/*float g = Sys_GetTextureGamma();
	g = std::clamp( g, 0.5f, 3.f );
	g /= 1.f;*/

	for ( int i = 0; i < 256; i++ )
	{
		g_gammatable[i] = i;
	}
}
