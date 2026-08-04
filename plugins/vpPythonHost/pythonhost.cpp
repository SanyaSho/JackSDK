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

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;

#include <filesystem>
namespace fs = std::filesystem;

#include "pythonhost.h"

extern void FreePythonActions();
extern void FreePythonIOFormats();


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
	FreePythonIOFormats();

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
void PythonHost::LoadScripts()
{
	py::gil_scoped_acquire gil;

	auto &searchPath = fs::current_path() / "plugins" / "pythonscripts";

	Sys_Printf( "Python: Loading Python modules from \"%s\"", searchPath.c_str() );

	py::module::import( "PythonHost" );

	try
	{
		py::module sys = py::module::import( "sys" );
		sys.attr( "path" ).attr( "append" )( searchPath.generic_string() );

		sys.attr( "stdout" ) = m_stdout;
		sys.attr( "stderr" ) = m_stderr;
	}
	catch ( std::exception &e )
	{
		Sys_Error( "%s", e.what() );
	}

	for ( auto &entry : fs::recursive_directory_iterator( searchPath ) )
	{
		if ( !entry.is_regular_file() )
			continue;

		if ( entry.path().extension() != ".py" )
			continue;

		fs::path relativePath = fs::relative( entry.path(), searchPath ).replace_extension( "" );

		std::string moduleName = relativePath.generic_string();
		std::replace( moduleName.begin(), moduleName.end(), '/', '.' );

		Sys_Printf( "Python: Loading module \"%s\"", moduleName.c_str() );

		try
		{
			py::module::import( moduleName.c_str() );
		}
		catch ( std::exception &e )
		{
			Sys_Error( "%s", e.what() );
		}
	}
}

PYBIND11_EMBEDDED_MODULE( JackSDK, m )
{
	AUTO_REGISTER( BaseTypes );
	AUTO_REGISTER( PluginActions );
	AUTO_REGISTER( PluginCamera );
	AUTO_REGISTER( PluginData );
	AUTO_REGISTER( PluginEntity );
	AUTO_REGISTER( PluginFaces );
	AUTO_REGISTER( PluginGroups );
	AUTO_REGISTER( PluginMath );
	AUTO_REGISTER( PluginNodes );
	AUTO_REGISTER( PluginPaths );
	// PluginPrimitives
	// PluginProfile
	AUTO_REGISTER( PluginRender );
	AUTO_REGISTER( PluginWorld );

	// Must be last registered item
	AUTO_REGISTER( PluginAPI );
}
