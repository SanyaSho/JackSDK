//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINACTIONS_H )
#define PLUGINACTIONS_H

/*
 Plugin actions are handled by the vpEnumActions exported function.

 Example:
 pluginActionInfo_t firstAction = { "ActionName", "ActionTitle", "ActionDescription", "ActionCategory", 0, 0, []() { Sys_Printf( "ActionDispatch" ); } };

 DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
 {
 	registerAction( &firstAction, pluginManager );
 	return 1;
 }

 Retured value is the count of actions added by the plugin. Returned value is not checked by the editor.
 pluginManager must be redirected into registerAction call.
*/

#include "BaseTypes.h"

#define ACTION_FLAG_HIDDEN	( 1 << 0 ) /* Completly hides the action until the level is loaded */
#define ACTION_FLAG_INLEVEL ( 1 << 1 ) /* Action is grayed out until the level is loaded */

typedef struct pluginActionInfo_s
{
	/* Action name. Used internally */
	const char *m_actionName;

	/* Action title. Contains actual action name that can be translated */
	const char *m_actionTitle;

	/* Action description. Used by the status bar */
	const char *m_actionDescription;

	/* Action category (ex. Quake) */
	const char *m_actionCategory;

	/* Action flags (see defines above) */
	uint64 m_actionFlags;

	/* Function called after action interaction */
	void (*m_dispatchFunc)();
} pluginActionInfo_t;
COMPILE_TIME_ASSERT( sizeof( pluginActionInfo_t ) == SIZEOF_PLUGINACTIONINFO_T );

// clang-format off

// Prototype for the function that is used to register actions internally
typedef void (*pfnRegisterAction)( pluginActionInfo_t *actionInfo, void *pluginManager );

typedef int (*vpEnumActions_t)( pfnRegisterAction registerAction, void *pluginManager );

// clang-format on

#endif // !PLUGINACTIONS_H
