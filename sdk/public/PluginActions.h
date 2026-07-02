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
 pluginActionInfo_t firstAction = { "ActionName", "ActionTitle", "ActionDescription", "ActionCategory", 0, 0, []() { Sys_Printf( "ActionDispatch\n" ); } };

 DLL_EXPORT int vpEnumActions( pfnRegisterAction registerAction, void *pluginManager )
 {
 	registerAction( &firstAction, pluginManager );
 	return 1;
 }

 Retured value is the count of actions added by the plugin. Returned value is not checked by the editor.
 pluginManager must be redirected into registerAction.
*/

#define ACTION_FLAG_HIDDEN	( 1 << 0 ) /* Completly hides the action until the level is loaded */
#define ACTION_FLAG_INLEVEL ( 1 << 1 ) /* Action is grayed out until the level is loaded */

// clang-format off

typedef struct pluginActionInfo_s
{
	const char *actionName;			/* Action name */
	const char *actionTitle;		/* Action title */
	const char *actionDescription;	/* Action description used by the status bar */
	const char *actionCategity;		/* Action category */
	int actionFlags;				/* Action flags */
	int b;
	void (*dispatchFunc)();			/* Function called after action interaction */
} pluginActionInfo_t;

COMPILE_TIME_ASSERT( sizeof( pluginActionInfo_t ) == 0x30 );

// Prototype for the function that is used to register actions internally
typedef void (*pfnRegisterAction)( pluginActionInfo_t *actionInfo, void *pluginManager );

// clang-format on

#endif // !PLUGINACTIONS_H
