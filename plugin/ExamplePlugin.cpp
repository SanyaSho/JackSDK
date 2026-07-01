#include <stdio.h>
#include <memory.h>
#include <locale.h>

#include "PluginAPI.h"

plugin_funcs_t gEditorfuncs;

void InternalCommand()
{
	char szText[256] = { 0 };
	Dialog_QueryArgument( "QueryArgumentTest", szText, sizeof( szText ) );

	float flValue = Dialog_QueryArgumentFloat( "QueryArgumentFloatTest" );
	Dialog_SetProgress( flValue, 1.f );

	int nValue = Dialog_QueryArgumentInt( "QueryArgumentIntTest" );

	Dialog_Printf( "QueryArgumentTest: %s\nQueryArgumentFloatTest: %f\nQueryArgumentIntTest: %d\n", szText, flValue, nValue );
}

void RunTests()
{
	Sys_Printf( "Sys_Printf" );
	Sys_DPrintf( "Sys_DPrintf" );
	Sys_Warning( "Sys_Warning" );
	Sys_Error( "Sys_Error" );

	void *ptr = Sys_Malloc( 32 );
	Sys_Free( ptr );

	const char *str = Sys_AllocString( "mystring" );
	Sys_Warning( str );
	Sys_Free( (void *)str );

	Sys_SetOption( SYS_OPTION_MAPGRID, 0 );

	for ( int i = 0; i < 6; i++ )
	{
		__int64 bOK = Sys_GetOption( i );

		Sys_Printf( "PLUGIN: %llu\n", bOK );
	}

	char out[260] = { 0 };
	bool bOK = Sys_GetBaseDirectory( out, sizeof( out ) );
	Sys_Warning( "Sys_GetBaseDirectory (%d): %s\n", bOK, out );

	bOK = Sys_GetModDirectory( out, sizeof( out ) );
	Sys_Warning( "Sys_GetModDirectory (%d): %s\n", bOK, out );

	bOK = Sys_GetFallbackDirectory( out, sizeof( out ) );
	Sys_Warning( "Sys_GetFallbackDirectory (%d): %s\n", bOK, out );

	Sys_Warning( "V_VersionString: %s\n", V_VersionString() );

	Sys_Warning( "Sys_GetTextureGamma: %f\n", Sys_GetTextureGamma() );

	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 3.7f ) );
	Sys_Warning( "Sys_PrintValue: %s\n", Sys_PrintValue( 2.4f ) );

	long bitmask = Dialog_CheckOptions( NULL, NULL, "MYOPTION1|TestOption2|TestOption3|CustomOption4" );
	Sys_Printf( "CheckOptions: %d\n", bitmask );

	//Dialog_MessageBox( "test1testtesttesttesttesttest", "test2testtesttesttesttesttesttesttest", DIALOG_TYPE_QUESTION );

	Dialog_Begin( "MyDialog" );
	{
		Dialog_AddTextEdit( "QueryArgumentTest", "Text", "TestText", 0 );
		Dialog_AddTextEdit( "QueryArgumentFloatTest", "Progress", "1.0", 0 );

		Dialog_AddRadioBox( "QueryArgumentIntTest", "RadioBox", false, 0 );

		Dialog_AddCheckBox( "CheckBoxTest", "CheckBox", true, 0 );

		Dialog_AddFileEdit( "FileEdit", "Select Map", "mapsrc", "*.jmf\n*.rmf\n*.*", DIALOG_FILE_OPEN );

		Dialog_AddFileList( "FileList", "test", "file1\nfile2\nfile3\nfile4", NULL, DIALOG_FILE_OPEN );

		Dialog_AddDirectoryEdit( "DirectoryEdit", "test", "C:\\JACK_10155", DIALOG_FILE_OPEN );

		Dialog_AddComboBox( "ComboBox", "My Combo Box", 0, "opt1\nopt2\nopt3\nopt4\nopt5", 0 );

		// SpinBox with values [0-15], step count 1 and initial value 1
		Dialog_AddSpinBox( "SpinBox", "My SpinBox", 1, 0, 15, 1, 0 );

		// SpinBox with values [0-15], step count 1 and initial value 1
		Dialog_AddSpinBoxFloat( "SpinBoxFloat", "My SpinBox", 1.f, 0.f, 100.f, 2.5f, 0 );

		Dialog_InitExternalCommand( "buttonTextExternal", "test" );
		Dialog_InitInternalCommand( "buttonTextInternal", InternalCommand );

		Dialog_Printf( "Dialog_Printf test" );
	}
	Dialog_End();
}

DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
{
	if ( editorFuncs->nIntefaceVersion < sizeof( plugin_funcs_t ) )
		return -1;

	if ( editorPluginVersion != PLUGIN_VERSION )
		return PLUGIN_VERSION;

	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->nIntefaceVersion );
	setlocale( LC_ALL, "C" );

	RunTests();

	return 0;
}