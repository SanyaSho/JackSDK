//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// Versions: "J.A.C.K. 1.1.3773 Freeware"; "J.A.C.K. 1.1.3773"; "J.A.C.K. 1.2.4603"
//
// Copyright (C) 2026 SanyaSho
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <http://www.gnu.org/licenses/>.
//
//=====================================================================================//

#if !defined( PLUGINACTIONS_H )
#define PLUGINACTIONS_H

/*
 Plugin actions are handled by the vpEnumActions exported function.

 Example:
 pluginActionDesc_t firstAction = { "ActionName", "ActionTitle", "ActionDescription", "ActionCategory", 0, 0, []() { Sys_Printf( "ActionDispatch" ); } };

 DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
 {
 	registerAction( &firstAction, pluginManager );
 	return 1;
 }

 Retured value is the count of actions added by the plugin. Returned value is not checked by the editor.
 pluginManager must be redirected into the registerAction call.
*/

#include "BaseTypes.h"

#define ACTION_FLAG_HIDDEN	( 1 << 0 ) /* Completly hides the action until the level is loaded */
#define ACTION_FLAG_INLEVEL ( 1 << 1 ) /* Action is grayed out until the level is loaded */

/*
    if ( (v14 & 0xC00) != 0 )
      QAction::setEnabled(this: *i, a2: true);
    if ( (v14 & 0x400) != 0 && m_shaderEditorCallbacks == nullptr )
      QAction::setEnabled(this: *i, a2: false);
    if ( (v14 & 0x800) != 0 && !v8 )
      QAction::setEnabled(this: *i, a2: false);
*/

typedef struct pluginActionDesc_s
{
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	/* Action name. Used internally */
	const char *m_actionName;
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

	/* Action title. Contains actual action name that can be translated */
	const char *m_actionTitle;

	/* Action description. Used by the status bar */
	const char *m_actionDescription;

	/* Action category (ex. Quake) */
	const char *m_actionCategory;

#if JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
	/* Unknown. Must be set to 0 */
	size_t m_unknown;
#endif // JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC

	/* Action flags (see defines above) */
	unsigned int m_actionFlags;

	/* First argument of m_dispatchFunc */
	unsigned int m_state;

	/* Function called after action interaction */
	void (*m_dispatchFunc)( int state );
} pluginActionDesc_t;
COMPILE_TIME_ASSERT( sizeof( pluginActionDesc_t ) == SIZEOF_PLUGINACTIONDESC_T );

// clang-format off

// Prototype for the function that is used to register actions internally
typedef void (*pfnRegisterAction)( pluginActionDesc_t *actionInfo, void *pluginManager );

typedef int (*vpEnumActions_t)( pfnRegisterAction registerAction, void *pluginManager );

// clang-format on

#endif // !PLUGINACTIONS_H
