//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
// 
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINEDITORFUNCTIONS_H )
#define PLUGINEDITORFUNCTIONS_H

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
#define SC_Parse3DMatrix			(gEditorfuncs.pfnSC_Parse3DMatrix)
#define SC_Parse2DMatrix			(gEditorfuncs.pfnSC_Parse2DMatrix)
#define SC_Parse1DMatrix			(gEditorfuncs.pfnSC_Parse1DMatrix)
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

#define BuildPackageList			(gEditorfuncs.pfnBuildPackageList)

/* Entity API */
#define Entity_Create				(gEditorfuncs.pfnEntity_Create)
#define Entity_Build				(gEditorfuncs.pfnEntity_Build)
#define Entity_GetColor				(gEditorfuncs.pfnEntity_GetColor)
#define Entity_SetColor				(gEditorfuncs.pfnEntity_SetColor)
#define Entity_AddToVisGroup		(gEditorfuncs.pfnEntity_AddToVisGroup)
#define Entity_RemoveFromVisGroup	(gEditorfuncs.pfnEntity_RemoveFromVisGroup)
#define Entity_GetVisGroupIdent		(gEditorfuncs.pfnEntity_GetVisGroupIdent)
#define Entity_GetVisGroupCount		(gEditorfuncs.pfnEntity_GetVisGroupCount)
#define Entity_FindByKeyValue		(gEditorfuncs.pfnEntity_FindByKeyValue)
#define Entity_FindByClassname		(gEditorfuncs.pfnEntity_FindByClassname)
#define Entity_FindByTargetname		(gEditorfuncs.pfnEntity_FindByTargetname)

/* Brush API */
#define Brush_Create				(gEditorfuncs.pfnBrush_Create)
#define Brush_Destroy				(gEditorfuncs.pfnBrush_Destroy)
#define Brush_GetColor				(gEditorfuncs.pfnBrush_GetColor)
#define Brush_SetColor				(gEditorfuncs.pfnBrush_SetColor)
#define Brush_AddToVisGroup			(gEditorfuncs.pfnBrush_AddToVisGroup)
#define Brush_RemoveFromVisGroup	(gEditorfuncs.pfnBrush_RemoveFromVisGroup)
#define Brush_GetVisGroupIdent		(gEditorfuncs.pfnBrush_GetVisGroupIdent)
#define Brush_GetVisGroupCount		(gEditorfuncs.pfnBrush_GetVisGroupCount)

/* Face API */
#define Face_Create					(gEditorfuncs.pfnFace_Create)
#define Face_Destroy				(gEditorfuncs.pfnFace_Destroy)

/* Overlay API */
#define Overlay_Create				(gEditorfuncs.pfnOverlay_Create)
#define Overlay_Destroy				(gEditorfuncs.pfnOverlay_Destroy)

/* Path API */
#define Path_Create					(gEditorfuncs.pfnPath_Create)
#define Path_Destroy				(gEditorfuncs.pfnPath_Destroy)
#define Path_Build					(gEditorfuncs.pfnPath_Build)

/* Node API */
#define Node_Insert					(gEditorfuncs.pfnNode_Insert)
#define Node_Append					(gEditorfuncs.pfnNode_Append)
#define Node_Destroy				(gEditorfuncs.pfnNode_Destroy)

/* Group API */
#define Group_Create				(gEditorfuncs.pfnGroup_Create)
#define Group_Destroy				(gEditorfuncs.pfnGroup_Destroy)
#define Group_AddGroup				(gEditorfuncs.pfnGroup_AddGroup)
#define Group_AddBrush				(gEditorfuncs.pfnGroup_AddBrush)
#define Group_AddEntity				(gEditorfuncs.pfnGroup_AddEntity)
#define Group_GetColor				(gEditorfuncs.pfnGroup_GetColor)
#define Group_SetColor				(gEditorfuncs.pfnGroup_SetColor)

/* Camera API */
#define Camera_Create				(gEditorfuncs.pfnCamera_Create)
#define Camera_Destroy				(gEditorfuncs.pfnCamera_Destroy)
#define Camera_GetColor				(gEditorfuncs.pfnCamera_GetColor)
#define Camera_SetColor				(gEditorfuncs.pfnCamera_SetColor)
#define Camera_Setup				(gEditorfuncs.pfnCamera_Setup)

/* Shader API */
#define Shader_Create				(gEditorfuncs.pfnShader_Create)
#define Shader_Lookup				(gEditorfuncs.pfnShader_Lookup)
#define Shader_Destroy				(gEditorfuncs.pfnShader_Destroy)
#define Shader_AddStage				(gEditorfuncs.pfnShader_AddStage)
#define Shader_RemoveStage			(gEditorfuncs.pfnShader_RemoveStage)
#define Shader_Finish				(gEditorfuncs.pfnShader_Finish)
#define Shader_GetWhiteTexture		(gEditorfuncs.pfnShader_GetWhiteTexture)
#define Shader_GetBlackTexture		(gEditorfuncs.pfnShader_GetBlackTexture)
#define Shader_LookupTexture		(gEditorfuncs.pfnShader_LookupTexture)
#define Shader_UploadTexture		(gEditorfuncs.pfnShader_UploadTexture)
#define Shader_DestroyTexture		(gEditorfuncs.pfnShader_DestroyTexture)

/* VisGroup API */
#define VisGroup_Add				(gEditorfuncs.pfnVisGroup_Add)
#define VisGroup_Remove				(gEditorfuncs.pfnVisGroup_Remove)
#define VisGroup_Modify				(gEditorfuncs.pfnVisGroup_Modify)
#define VisGroup_GetCount			(gEditorfuncs.pfnVisGroup_GetCount)
#define VisGroup_GetIndex			(gEditorfuncs.pfnVisGroup_GetIndex)
#define VisGroup_GetData			(gEditorfuncs.pfnVisGroup_GetData)
#define VisGroup_RebuildIdents		(gEditorfuncs.pfnVisGroup_RebuildIdents)

/* Undo API*/
#define Undo_Start					(gEditorfuncs.pfnUndo_Start)
#define Undo_End					(gEditorfuncs.pfnUndo_End)
#define Undo_AddGroup				(gEditorfuncs.pfnUndo_AddGroup)
#define Undo_AddBrush				(gEditorfuncs.pfnUndo_AddBrush)
#define Undo_AddPath				(gEditorfuncs.pfnUndo_AddPath)
#define Undo_AddNode				(gEditorfuncs.pfnUndo_AddNode)
#define Undo_AddEntity				(gEditorfuncs.pfnUndo_AddEntity)
#define Undo_DeleteGroup			(gEditorfuncs.pfnUndo_DeleteGroup)
#define Undo_DeleteBrush			(gEditorfuncs.pfnUndo_DeleteBrush)
#define Undo_DeletePath				(gEditorfuncs.pfnUndo_DeletePath)
#define Undo_DeleteNode				(gEditorfuncs.pfnUndo_DeleteNode)
#define Undo_DeleteEntity			(gEditorfuncs.pfnUndo_DeleteEntity)
#define Undo_StoreFace				(gEditorfuncs.pfnUndo_StoreFace)
#define Undo_StoreBrush				(gEditorfuncs.pfnUndo_StoreBrush)
#define Undo_StorePath				(gEditorfuncs.pfnUndo_StorePath)
#define Undo_StoreNode				(gEditorfuncs.pfnUndo_StoreNode)
#define Undo_StoreEntity			(gEditorfuncs.pfnUndo_StoreEntity)
#define Undo_AddSelectedEntities	(gEditorfuncs.pfnUndo_AddSelectedEntities)
#define Undo_AddSelectedBrushes		(gEditorfuncs.pfnUndo_AddSelectedBrushes)
#define Undo_AddSelectedNodes		(gEditorfuncs.pfnUndo_AddSelectedNodes)
#define Undo_DeleteSelectedEntities	(gEditorfuncs.pfnUndo_DeleteSelectedEntities)
#define Undo_DeleteSelectedBrushes	(gEditorfuncs.pfnUndo_DeleteSelectedBrushes)
#define Undo_StoreSelectedEntities	(gEditorfuncs.pfnUndo_StoreSelectedEntities)
#define Undo_DeleteSelectedNodes	(gEditorfuncs.pfnUndo_DeleteSelectedNodes)
#define Undo_StoreSelectedBrushes	(gEditorfuncs.pfnUndo_StoreSelectedBrushes)
#define Undo_StoreSelectedNodes		(gEditorfuncs.pfnUndo_StoreSelectedNodes)
#define Undo_StoreSelectedFaces		(gEditorfuncs.pfnUndo_StoreSelectedFaces)

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

#endif // !PLUGINEDITORFUNCTIONS_H
