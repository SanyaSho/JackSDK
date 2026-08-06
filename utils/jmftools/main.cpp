//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

#if defined( WIN32 )
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif // WIN32

// Plugin API
#define ENTITYAPI_NO_INLINEFUNCS
#define PLUGINEDITORFUNCTIONS_H
#include "PluginMeta.h"

#include "logger.h"
#include "math.h"
#include "mem.h"

#include "mapworld.h"

#include "DataSerializer.h"
#include "serializer_jmf.h"
#include "serializer_map.h"

/*
===============
V_VersionString
===============
*/
static const char *V_VersionString()
{
	return "jmftools";
}

/*
===============
Global_GetCurrentWorld
===============
*/
static CMapWorld *s_currentWorld = NULL;
static qWorld_s *Global_GetCurrentWorld()
{
	return (qWorld_s *)s_currentWorld;
}

/*
===============
BuildPackageList
===============
*/
static bool BuildPackageList( qWorld_s *worldDef, char **outBuf, bool listSeparator, int removeVolumePrefix )
{
	if ( !worldDef )
	{
		return false;
	}

	return worldDef->m_mapWorld->buildPackageList( outBuf, listSeparator, removeVolumePrefix );
}

/*
===============
Sys_MakeLocalFileName
===============
*/
static const char *Sys_MakeLocalFileName( const char *filePath )
{
	return filePath;
}

plugin_funcs_t gEditorfuncs;

static void BuildEditorFunctionsTable()
{
	memset( &gEditorfuncs, 0, sizeof( gEditorfuncs ) );
	gEditorfuncs.m_intefaceVersion = sizeof( plugin_funcs_t );

	gEditorfuncs.pfnSys_Printf = Sys_Printf;
	gEditorfuncs.pfnSys_DPrintf = Sys_DPrintf;
	gEditorfuncs.pfnSys_Warning = Sys_Warning;
	gEditorfuncs.pfnSys_Error = Sys_Error;

	gEditorfuncs.pfnSys_Malloc = Sys_Malloc;
	gEditorfuncs.pfnSys_Free = Sys_Free;

	gEditorfuncs.pfnSys_AllocString = Sys_AllocString;

	gEditorfuncs.filesystemfuncs.pfnSys_MakeLocalFileName = Sys_MakeLocalFileName;

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	gEditorfuncs.mathfuncs.pfnSys_PrintValue = Sys_PrintValue;
	gEditorfuncs.mathfuncs.pfnSys_PrintMapCoord = Sys_PrintMapCoord;
	gEditorfuncs.mathfuncs.pfnSys_PrintAxis = Sys_PrintAxis;
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	gEditorfuncs.mathfuncs.pfnSys_SnapVertex = Sys_SnapVertex;
	gEditorfuncs.mathfuncs.pfnSys_SnapAxis = Sys_SnapAxis;
	gEditorfuncs.mathfuncs.pfnSys_SnapVertexToGrid = Sys_SnapVertexToGrid;
	gEditorfuncs.mathfuncs.pfnSys_SnapMapVertex = Sys_SnapMapVertex;

	gEditorfuncs.pfnV_VersionString = V_VersionString;

	gEditorfuncs.pfnGlobal_GetCurrentWorld = Global_GetCurrentWorld;

	gEditorfuncs.pfnBuildPackageList = BuildPackageList;
}

static void PrintUsage( const char *exeName )
{
	Sys_Printf( "Usage: %s -i <inputfile> -o <outputfile>", exeName );
}

static char s_inputFile[MAX_PATH] = { 0 };
static char s_outputFile[MAX_PATH] = { 0 };

/*
===============
main
===============
*/
int main( int argc, char **argv )
{
	BuildEditorFunctionsTable();

	if ( argc == 1 )
	{
		PrintUsage( argv[0] );
		return 0;
	}

	for ( int i = 1; i < argc; i++ )
	{
		if ( !stricmp( argv[i], "-i" ) || !stricmp( argv[i], "--input" ) )
		{
			if ( i + 1 >= argc || argv[i + 1][0] == '-' )
			{
				PrintUsage( argv[0] );
				return 1;
			}

			strncpy( s_inputFile, argv[i + 1], sizeof( s_inputFile ) );
			s_inputFile[sizeof( s_inputFile ) - 1] = '\0';
			i++;
		}
		else if ( !stricmp( argv[i], "-o" ) || !stricmp( argv[i], "--output" ) )
		{
			if ( i + 1 >= argc || argv[i + 1][0] == '-' )
			{
				PrintUsage( argv[0] );
				return 1;
			}

			strncpy( s_outputFile, argv[i + 1], sizeof( s_outputFile ) );
			s_outputFile[sizeof( s_outputFile ) - 1] = '\0';
			i++;
		}
		else
		{
			break;
		}
	}

	if ( s_inputFile[0] == '\0' || s_outputFile[0] == '\0' )
	{
		Sys_Error( "Both input and output files arguments are required.\n" );

		PrintUsage( argv[0] );
		return 1;
	}

	// Spawn da world
	s_currentWorld = new CMapWorld;

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	JMFSerializer jmfSerializer( s_inputFile, 0, 0, s_currentWorld );
#else
	JMFSerializer jmfSerializer( s_inputFile, s_currentWorld );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	if ( !jmfSerializer.Import() )
	{
		Sys_Printf( "Failed to import \"%s\"\n", s_inputFile );
		return 1;
	}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	JMFSerializer mapSerializer( s_outputFile, 0, 0, s_currentWorld );
#else
	JMFSerializer mapSerializer( s_outputFile, s_currentWorld );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	if ( !mapSerializer.Export() )
	{
		Sys_Printf( "Failed to export \"%s\"", s_outputFile );
		return 1;
	}

	if ( s_currentWorld )
	{
		delete s_currentWorld;
		s_currentWorld = NULL;
	}

	return 0;
}
