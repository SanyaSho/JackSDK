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

// clang-format off
mapProfile_t g_HalfLifeAlpha =
{
	NULL,

	NULL,

	PROFILE_DECALTOOL_USE_HL1_DECALS | PROFILE_ENTITY_HL1_RENDERPROPS | ( 1 << 10 ) | ( 1 << 14 ) | /*PROFILE_SKY_SHADER |*/ PROFILE_STUDIO_INVERT_PITCH_STATE | PROFILE_ENABLE_FALLBACK_DIR,

	"vpHalfLifeAlpha",
	"Half-Life Alpha",

	".bsp",
	"",
	".wad",
	".pak",
	".spr",
	".mdl",
	".wav",
	/*".aur"*/"",
	"",
	"",

	/*".tga"*/"",
	/*"ft;bk;rt;lf;up;dn"*/"",

	"sprites",
	"models",
	"sound",
	/*"aurora"*/"",
	"",
	"",
	"textures",
	"",
	/*"gfx/env"*/"",

	"-onlyents",
	"",
	"-fast",
	"-extra",
	"+map",

	"info_player_start",
	"",
	"light",
	"",
	"",
	"",
	"infodecal",
	"",

	/*"desert"*/"",
	"AAATRIGGER",
	"NULL",

	/*"C1A0_WX"*/"CRETE1_WALL01",
	/*"C2A4E_W1"*/"CRETE2_FLR01",
	/*"C1A0_LABFLRC"*/"CRETE2_FLR01",

	"+-",

	0,
	0,
	0
};
// clang-format on

/*
===============
vpEnumProfiles
===============
*/
DLL_EXPORT int vpEnumProfiles( pfnRegisterProfile registerProfile, void *libraryHandle )
{
	registerProfile( &g_HalfLifeAlpha, libraryHandle );
	return 1;
}
