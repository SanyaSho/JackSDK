//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
// 
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINFACE_H )
#define PLUGINFACE_H

extern plugin_funcs_t gEditorfuncs;

// clang-format off

#define Sys_Printf					(gEditorfuncs.pfnSys_Printf)
#define Sys_DPrintf					(gEditorfuncs.pfnSys_DPrintf)
#define Sys_Warning					(gEditorfuncs.pfnSys_Warning)
#define Sys_Error					(gEditorfuncs.pfnSys_Error)

#define Sys_Free					(gEditorfuncs.pfnSys_Free)
#define Sys_Malloc					(gEditorfuncs.pfnSys_Malloc)

#define TempBuffer_GetSpace			(gEditorfuncs.pfnTempBuffer_GetSpace)
#define Sys_AllocString				(gEditorfuncs.pfnSys_AllocString)

#define Sys_FloatTime				(gEditorfuncs.pfnSys_FloatTime)

#define Sys_SetOption				(gEditorfuncs.pfnSys_SetOption)
#define Sys_GetOption				(gEditorfuncs.pfnSys_GetOption)

#define SC_Token					(gEditorfuncs.pfnSC_Token)
#define SC_Line						(gEditorfuncs.pfnSC_Line)
#define SC_ParseFromFile			(gEditorfuncs.pfnSC_ParseFromFile)
#define SC_ParseFromMemory			(gEditorfuncs.pfnSC_ParseFromMemory)
#define SC_CheckError				(gEditorfuncs.pfnSC_CheckError)
#define SC_ParseError				(gEditorfuncs.pfnSC_ParseError)
#define SC_ResetError				(gEditorfuncs.pfnSC_ResetError)
#define SC_SafeGetToken				(gEditorfuncs.pfnSC_SafeGetToken)
#define SC_GetToken					(gEditorfuncs.pfnSC_GetToken)
#define SC_TokenAvailable			(gEditorfuncs.pfnSC_TokenAvailable)
#define SC_UnGetToken				(gEditorfuncs.pfnSC_UnGetToken)
#define SC_MatchToken				(gEditorfuncs.pfnSC_MatchToken)
#define SC_SafeMatchToken			(gEditorfuncs.pfnSC_SafeMatchToken)
// SC_Parse3DMatrix
// SC_Parse2DMatrix
// SC_Parse1DMatrix
#define SC_SkipRestOfLine			(gEditorfuncs.pfnSC_SkipRestOfLine)
#define SC_EndOfParsing				(gEditorfuncs.pfnSC_EndOfParsing)
#define SC_GetParseFlags			(gEditorfuncs.pfnSC_GetParseFlags)
#define SC_SetParseFlags			(gEditorfuncs.pfnSC_SetParseFlags)
#define SC_ShouldQuote				(gEditorfuncs.pfnSC_ShouldQuote)
#define SC_CopyBlock				(gEditorfuncs.pfnSC_CopyBlock)
#define SC_SkipBlock				(gEditorfuncs.pfnSC_SkipBlock)
#define SC_SkipLineOrBlock			(gEditorfuncs.pfnSC_SkipLineOrBlock)
#define SC_GetBlockSize				(gEditorfuncs.pfnSC_GetBlockSize)

// PR[17]

/* FileSystem API */
#define Sys_GetBaseDirectory		(gEditorfuncs.pfnSys_GetBaseDirectory)
#define Sys_GetModDirectory			(gEditorfuncs.pfnSys_GetModDirectory)
#define Sys_GetFallbackDirectory	(gEditorfuncs.pfnSys_GetFallbackDirectory)
#define Sys_ExpandFileName			(gEditorfuncs.pfnSys_ExpandFileName)
#define Sys_MakeLocalFileName		(gEditorfuncs.pfnSys_MakeLocalFileName)
#define Sys_FileExists				(gEditorfuncs.pfnSys_FileExists)
#define Sys_LoadFile				(gEditorfuncs.pfnSys_LoadFile)
#define Sys_CreatePath				(gEditorfuncs.pfnSys_CreatePath)

/* Math API */
#define Sys_PrintValue				(gEditorfuncs.pfnSys_PrintValue)
#define Sys_PrintMapCoord			(gEditorfuncs.pfnSys_PrintMapCoord)
#define Sys_PrintAxis				(gEditorfuncs.pfnSys_PrintAxis)
#define Sys_SnapVertex				(gEditorfuncs.pfnSys_SnapVertex)
#define Sys_SnapAxis				(gEditorfuncs.pfnSys_SnapAxis)
#define Sys_SnapVertexToGrid		(gEditorfuncs.pfnSys_SnapVertexToGrid)
#define Sys_SnapMapVertex			(gEditorfuncs.pfnSys_SnapMapVertex)

#define V_VersionString				(gEditorfuncs.pfnV_VersionString)

#define Sys_GetTextureGamma			(gEditorfuncs.pfnSys_GetTextureGamma)

#define Global_GetCurrentWorld		(gEditorfuncs.pfnGlobal_GetCurrentWorld)

// BuildPackageList

/* Entity API */
#define Entity_Create				(gEditorfuncs.pfnEntity_Create)
#define Entity_Build				(gEditorfuncs.pfnEntity_Build)
#define Entity_GetColor				(gEditorfuncs.pfnEntity_GetColor)
#define Entity_SetColor				(gEditorfuncs.pfnEntity_SetColor)
#define Entity_AddToVisGroup		(gEditorfuncs.pfnEntity_AddToVisGroup)
#define Entity_RemoveFromVisGroup	(gEditorfuncs.pfnEntity_RemoveFromVisGroup)
#define Entity_GetVisGroupIdent		(gEditorfuncs.pfnEntity_GetVisGroupIdent)
#define Entity_GetVisGroupCount		(gEditorfuncs.pfnEntity_GetVisGroupCount)
#define Entity_FindByClassname		(gEditorfuncs.pfnEntity_FindByClassname)
#define Entity_FindByTargetname		(gEditorfuncs.pfnEntity_FindByTargetname)
#define Entity_FindByKeyValue		(gEditorfuncs.pfnEntity_FindByKeyValue)

/* Camera API */
#define Camera_Create				(gEditorfuncs.pfnCamera_Create)
#define Camera_Destroy				(gEditorfuncs.pfnCamera_Destroy)
#define Camera_GetColor				(gEditorfuncs.pfnCamera_GetColor)
#define Camera_SetColor				(gEditorfuncs.pfnCamera_SetColor)
#define Camera_Setup				(gEditorfuncs.pfnCamera_Setup)

/* Dialog API */
#define Dialog_CheckOptions			(gEditorfuncs.pfnDialog_CheckOptions)
#define Dialog_MessageBox			(gEditorfuncs.pfnDialog_MessageBox)
#define Dialog_Begin				(gEditorfuncs.pfnDialog_Begin)
#define Dialog_InitExternalCommand	(gEditorfuncs.pfnDialog_InitExternalCommand)
#define Dialog_InitInternalCommand	(gEditorfuncs.pfnDialog_InitInternalCommand)
#define Dialog_SetProgress			(gEditorfuncs.pfnDialog_SetProgress)
#define Dialog_AddTextEdit			(gEditorfuncs.pfnDialog_AddTextEdit)
#define Dialog_AddRadioBox			(gEditorfuncs.pfnDialog_AddRadioBox)
#define Dialog_AddCheckBox			(gEditorfuncs.pfnDialog_AddCheckBox)
#define Dialog_AddSpinBox			(gEditorfuncs.pfnDialog_AddSpinBox)
#define Dialog_AddSpinBoxFloat		(gEditorfuncs.pfnDialog_AddSpinBoxFloat)
#define Dialog_AddFileEdit			(gEditorfuncs.pfnDialog_AddFileEdit)
#define Dialog_AddFileList			(gEditorfuncs.pfnDialog_AddFileList)
#define Dialog_AddComboBox			(gEditorfuncs.pfnDialog_AddComboBox)
#define Dialog_AddDirectoryEdit		(gEditorfuncs.pfnDialog_AddDirectoryEdit)
#define Dialog_QueryArgument		(gEditorfuncs.pfnDialog_QueryArgument)
#define Dialog_QueryArgumentFloat	(gEditorfuncs.pfnDialog_QueryArgumentFloat)
#define Dialog_QueryArgumentInt		(gEditorfuncs.pfnDialog_QueryArgumentInt)
#define Dialog_End					(gEditorfuncs.pfnDialog_End)
#define Dialog_Printf				(gEditorfuncs.pfnDialog_Printf)
#define Dialog_BeginWait			(gEditorfuncs.pfnDialog_BeginWait)
#define Dialog_EndWait				(gEditorfuncs.pfnDialog_EndWait)

// clang-format on

#endif // !PLUGINFACE_H
