//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>

#include "PluginAPI.h"

static void ControlsTest_InternalCommand()
{
	char szText[256] = { 0 };
	Dialog_QueryArgument( "QueryArgumentTest", szText, sizeof( szText ) );

	float flValue = Dialog_QueryArgumentFloat( "QueryArgumentFloatTest" );
	Dialog_SetProgress( flValue, 1.f );

	int nValue = Dialog_QueryArgumentInt( "QueryArgumentIntTest" );

	Dialog_Printf( "QueryArgumentTest: %s\nQueryArgumentFloatTest: %f\nQueryArgumentIntTest: %d", szText, flValue, nValue );
}

#define TEST_MSGBOX_NORMAL ( 1 << 0 )
#define TEST_MSGBOX_OKCANCEL ( 1 << 1 )
#define TEST_MSGBOX_YESNO ( 1 << 2 )
#define TEXT_MSGBOX_CONTROLS ( 1 << 3 )

void MessageBoxTest( int type )
{
	int mask = Dialog_CheckOptions( "Dialog_CheckOptions", "Select which tests will be executed", "MessageBox (Normal)|MessageBox (OK/Cancel)|MessageBox (Yes/No)|Controls" );

	Sys_Printf( "mask: %d", mask );

	if ( mask <= 0 )
	{
		Dialog_MessageBox( "Warning", "No test cases were selected", DIALOG_MB_ICONWARNING );
		return;
	}

	bool ret = false;

	/*
	 Those dialogs will never return FALSE.
	*/
	if ( ( mask & TEST_MSGBOX_NORMAL ) != 0 )
	{
		ret = Dialog_MessageBox( "Error", "Error", DIALOG_MB_ICONERROR );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "Warning", "Warning", DIALOG_MB_ICONWARNING );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "Information", "Information", DIALOG_MB_ICONINFORMATION );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "Question", "Question", DIALOG_MB_ICONQUESTION );
		Sys_Printf( "ret: %d", ret );
	}

	/*
	 Those dialogs will return FALSE only if "Cancel" button is pressed or if the window is closed.
	*/
	if ( ( mask & TEST_MSGBOX_OKCANCEL ) != 0 )
	{
		ret = Dialog_MessageBox( "OkCancel | Error", "Error", DIALOG_MB_OKCANCEL | DIALOG_MB_ICONERROR );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "OkCancel | Warning", "Warning", DIALOG_MB_OKCANCEL | DIALOG_MB_ICONWARNING );
		Sys_Printf( "ret: %d", ret );

		ret  = Dialog_MessageBox( "OkCancel | Information", "Information", DIALOG_MB_OKCANCEL | DIALOG_MB_ICONINFORMATION );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "OkCancel | Question", "Question", DIALOG_MB_OKCANCEL | DIALOG_MB_ICONQUESTION );
		Sys_Printf( "ret: %d", ret );
	}

	/*
	 Same as with OkCancel dialogs.
	*/
	if ( ( mask & TEST_MSGBOX_YESNO ) != 0 )
	{
		ret = Dialog_MessageBox( "YesNo | Error", "Error", DIALOG_MB_YESNO | DIALOG_MB_ICONERROR );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "YesNo | Warning", "Warning", DIALOG_MB_YESNO | DIALOG_MB_ICONWARNING );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "YesNo | Information", "Information", DIALOG_MB_YESNO | DIALOG_MB_ICONINFORMATION );
		Sys_Printf( "ret: %d", ret );

		ret = Dialog_MessageBox( "YesNo | Question", "Question", DIALOG_MB_YESNO | DIALOG_MB_ICONQUESTION );
		Sys_Printf( "ret: %d", ret );
	}

	/*
	 Will spawn a dialog window with some various controls to play with
	*/
	if ( ( mask & TEXT_MSGBOX_CONTROLS ) != 0 )
	{
		Dialog_Begin( "MyDialog" );
		{
			Dialog_AddTextEdit( "QueryArgumentTest", "Text", "TestText", 0 );
			Dialog_AddTextEdit( "QueryArgumentFloatTest", "Progress", "1.0", 0 );

			Dialog_AddRadioBox( "QueryArgumentIntTest", "RadioBox", false, 0 );

			Dialog_AddCheckBox( "CheckBoxTest", "CheckBox", true, 0 );

			Dialog_AddFileEdit( "FileEdit", "Select Map", "mapsrc", "*.jmf\n*.rmf\n*.*", DIALOG_FILE_OPEN );

			Dialog_AddFileList( "FileList", "test", "file1\nfile2\nfile3\nfile4", NULL, DIALOG_FILE_OPEN );

			Dialog_AddDirectoryEdit( "DirectoryEdit", "test", "C:\\JACK_10155", DIALOG_FILE_OPEN );

			Dialog_AddComboBox( "ComboBox", "My Combo Box", 0, "OPT1\n1\nOPT2\n2\nOPT3\n3", 0 );

			// SpinBox with values [0-15], step count 1 and initial value 1
			Dialog_AddSpinBox( "SpinBox", "My SpinBox", 1, 0, 15, 1, 0 );

			// SpinBox with values [0-15], step count 1 and initial value 1
			Dialog_AddSpinBoxFloat( "SpinBoxFloat", "My SpinBox", 1.f, 0.f, 100.f, 2.5f, 0 );

			//Dialog_InitExternalCommand( "buttonTextExternal", "test" );
			Dialog_InitInternalCommand( "buttonTextInternal", ControlsTest_InternalCommand );

			Dialog_Printf( "Dialog_Printf test" );
		}
		Dialog_End();
	}
}
