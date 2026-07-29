//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>
#include <vector>

// Plugin API
#include "PluginMeta.h"

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include "pythonhost.h"

namespace py = pybind11;

struct PythonAction
{
	int m_userData;
	py::function m_pyCallback;

	pluginActionDesc_t m_actionDesc;
};

static std::vector<std::unique_ptr<PythonAction>> s_pythonActions;
static unsigned int m_pythonActionsNextUserData = 0;

void FreePythonActions()
{
	if ( !Py_IsInitialized() )
		return;

	py::gil_scoped_acquire gil;

	for ( auto &action : s_pythonActions )
	{
		action->m_pyCallback = py::none();

		Sys_Free( (void *)action->m_actionDesc.m_actionName );
		Sys_Free( (void *)action->m_actionDesc.m_actionTitle );
		Sys_Free( (void *)action->m_actionDesc.m_actionDescription );
		Sys_Free( (void *)action->m_actionDesc.m_actionCategory );
	}
}

static void PythonActionDispatch( int actionUserData )
{
	py::gil_scoped_acquire gil;

	for ( auto &action : s_pythonActions )
	{
		if ( action->m_actionDesc.m_actionUserData != actionUserData )
			continue;

		try
		{
			action->m_pyCallback( action->m_userData );
		}
		catch ( std::exception &e )
		{
			Sys_Error( "%s", e.what() );
		}

		return;
	}
}

static void PythonRegisterAction(
	const char *actionName,
	const char *actionTitle,
	const char *actionDescription,
	const char *actionCategory,
	int actionFlags,
	int actionUserData,
	py::function pyCallback )
{
	auto pythonAction = std::make_unique<PythonAction>();

	pythonAction->m_userData = actionUserData;
	pythonAction->m_pyCallback = std::move( pyCallback );

	memset( &pythonAction->m_actionDesc, 0, sizeof( pluginActionDesc_t ) );
	pythonAction->m_actionDesc.m_actionName = Sys_AllocString( actionName );
	pythonAction->m_actionDesc.m_actionTitle = Sys_AllocString( actionTitle );
	pythonAction->m_actionDesc.m_actionDescription = Sys_AllocString( actionDescription );
	pythonAction->m_actionDesc.m_actionCategory = Sys_AllocString( actionCategory );
	pythonAction->m_actionDesc.m_actionFlags = actionFlags;
	pythonAction->m_actionDesc.m_actionUserData = ++m_pythonActionsNextUserData; // This value is used to determine which action must be executed
	pythonAction->m_actionDesc.m_dispatchFunc = PythonActionDispatch;

	s_pythonActions.push_back( std::move( pythonAction ) );
}

REGISTER( PluginActions )
{
	m.doc() = "PluginActions.h Bindings";

	m.def( "register_action", PythonRegisterAction );
}

/*
===============
vpEnumActions
Interactive menu actions
===============
*/
DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
{
	for ( auto &action : s_pythonActions )
	{
		registerAction( &action->m_actionDesc, pluginManager );
	}

	return s_pythonActions.size();
}
