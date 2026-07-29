//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PYTHONHOST_H )
#define PYTHONHOST_H

#include <string>

class IPythonOutput
{
public:
	virtual ~IPythonOutput() {}

	virtual void write( std::string str ) = 0;
	virtual void flush() = 0;
};

class PythonHost
{
public:
	PythonHost();
	virtual ~PythonHost();

	virtual void Initialize();
	virtual void LoadScripts( const char *path );

private:
	IPythonOutput *m_stdout;
	IPythonOutput *m_stderr;
};

#define REGISTER( name ) void PluginAPI_Register_##name( py::module_ &m )
#define EXTERN_REGISTER( name ) extern void PluginAPI_Register_##name( py::module_ &m );

#define AUTO_REGISTER( name ) EXTERN_REGISTER( ##name ) py::module_ name = m.def_submodule( #name ); PluginAPI_Register_##name( name )

#endif // !PYTHONHOST_H
