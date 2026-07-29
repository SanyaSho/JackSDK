//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>
#include <filesystem>

// Plugin API
#include "PluginMeta.h"

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;

#include "pythonhost.h"

extern void FreePythonActions();


/*
===============
stdout -> Sys_Printf
===============
*/
class PythonStdout : public IPythonOutput
{
public:
	PythonStdout() {}
	virtual ~PythonStdout() {}

	virtual void write( std::string str )
	{
		Sys_Printf( str.c_str() );
	}
	virtual void flush()
	{
		fflush( stdout );
	}
};

/*
===============
stderr -> Sys_Error
===============
*/
class PythonStderr : public IPythonOutput
{
public:
	PythonStderr() {}
	virtual ~PythonStderr() {}

	virtual void write( std::string str )
	{
		Sys_Error( str.c_str() );
	}
	virtual void flush()
	{
		fflush( stderr );
	}
};


PYBIND11_EMBEDDED_MODULE( PythonHost, m )
{
	py::class_<PythonStdout>( m, "stdout" ).def( "write", &PythonStdout::write ).def( "flush", &PythonStdout::flush );
	py::class_<PythonStderr>( m, "stderr" ).def( "write", &PythonStderr::write ).def( "flush", &PythonStderr::flush );
}


/*
===============
PythonHost
===============
*/
PythonHost::PythonHost()
	: m_stdout( NULL ), m_stderr( NULL )
{
}

/*
===============
~PythonHost
===============
*/
PythonHost::~PythonHost()
{
	if ( Py_IsInitialized() )
	{
		py::gil_scoped_acquire gil;

		try
		{
			py::module sys = py::module::import( "sys" );

			sys.attr( "stdout" ) = py::none();
			sys.attr( "stderr" ) = py::none();
		}
		catch ( std::exception &e )
		{
			Sys_Error( "%s", e.what() );
		}
	}

	FreePythonActions();

	if ( m_stderr )
	{
		delete m_stderr;
		m_stderr = NULL;
	}
	if ( m_stdout )
	{
		delete m_stdout;
		m_stdout = NULL;
	}

	// FIXME
	//py::finalize_interpreter();
}

/*
===============
Initialize
===============
*/
void PythonHost::Initialize()
{
	try
	{
		py::initialize_interpreter();
	}
	catch ( std::exception &e )
	{
		Sys_Error( "%s", e.what() );
	}

	m_stdout = new PythonStdout();
	m_stderr = new PythonStderr();
}

/*
===============
LoadScripts
===============
*/
void PythonHost::LoadScripts( const char *path )
{
	py::gil_scoped_acquire gil;

	py::module::import( "PythonHost" );

	try
	{
		py::module sys = py::module::import( "sys" );
		sys.attr( "path" ).attr( "append" )( path );

		sys.attr( "stdout" ) = m_stdout;
		sys.attr( "stderr" ) = m_stderr;
	}
	catch ( std::exception &e )
	{
		Sys_Error( "%s", e.what() );
	}

	try
	{
		py::module::import( "actions" );
	}
	catch ( std::exception &e )
	{
		Sys_Error( "%s", e.what() );
	}
}

PYBIND11_EMBEDDED_MODULE( JackSDK, m )
{
	AUTO_REGISTER( BaseTypes );
	AUTO_REGISTER( PluginActions );
	AUTO_REGISTER( PluginCamera );
	// PluginData
	AUTO_REGISTER( PluginEntity );
	AUTO_REGISTER( PluginFaces );
	AUTO_REGISTER( PluginGroups );
	AUTO_REGISTER( PluginMath );
	AUTO_REGISTER( PluginNodes );
	AUTO_REGISTER( PluginPaths );
	// PluginPrimitives
	// PluginProfile
	// PluginRender
	AUTO_REGISTER( PluginWorld );

	// Must be last registered item
	AUTO_REGISTER( PluginAPI );
}
