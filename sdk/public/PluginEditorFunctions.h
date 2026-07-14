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

/* Misc API */
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

#define Steam_SetAchievemnt			(gEditorfuncs.pfnSteam_SetAchievemnt)

/* Parser API */
#define SC_Token					(gEditorfuncs.parserfuncs.pfnSC_Token)
#define SC_Line						(gEditorfuncs.parserfuncs.pfnSC_Line)
#define SC_ParseFromFile			(gEditorfuncs.parserfuncs.pfnSC_ParseFromFile)
#define SC_ParseFromMemory			(gEditorfuncs.parserfuncs.pfnSC_ParseFromMemory)
#define SC_CheckError				(gEditorfuncs.parserfuncs.pfnSC_CheckError)
#define SC_ParseError				(gEditorfuncs.parserfuncs.pfnSC_ParseError)
#define SC_ResetError				(gEditorfuncs.parserfuncs.pfnSC_ResetError)
#define SC_SafeGetToken				(gEditorfuncs.parserfuncs.pfnSC_SafeGetToken)
#define SC_GetToken					(gEditorfuncs.parserfuncs.pfnSC_GetToken)
#define SC_TokenAvailable			(gEditorfuncs.parserfuncs.pfnSC_TokenAvailable)
#define SC_UnGetToken				(gEditorfuncs.parserfuncs.pfnSC_UnGetToken)
#define SC_MatchToken				(gEditorfuncs.parserfuncs.pfnSC_MatchToken)
#define SC_SafeMatchToken			(gEditorfuncs.parserfuncs.pfnSC_SafeMatchToken)
#define SC_Parse3DMatrix			(gEditorfuncs.parserfuncs.pfnSC_Parse3DMatrix)
#define SC_Parse2DMatrix			(gEditorfuncs.parserfuncs.pfnSC_Parse2DMatrix)
#define SC_Parse1DMatrix			(gEditorfuncs.parserfuncs.pfnSC_Parse1DMatrix)
#define SC_SkipRestOfLine			(gEditorfuncs.parserfuncs.pfnSC_SkipRestOfLine)
#define SC_EndOfParsing				(gEditorfuncs.parserfuncs.pfnSC_EndOfParsing)
#define SC_GetParseFlags			(gEditorfuncs.parserfuncs.pfnSC_GetParseFlags)
#define SC_SetParseFlags			(gEditorfuncs.parserfuncs.pfnSC_SetParseFlags)
#define SC_ShouldQuote				(gEditorfuncs.parserfuncs.pfnSC_ShouldQuote)
#define SC_CopyBlock				(gEditorfuncs.parserfuncs.pfnSC_CopyBlock)
#define SC_SkipBlock				(gEditorfuncs.parserfuncs.pfnSC_SkipBlock)
#define SC_SkipLineOrBlock			(gEditorfuncs.parserfuncs.pfnSC_SkipLineOrBlock)
#define SC_GetBlockSize				(gEditorfuncs.parserfuncs.pfnSC_GetBlockSize)

/* Rendering API*/
#define PR_BindTexture				(gEditorfuncs.renderingfuncs.pfnPR_BindTexture)
#define PR_BindShader				(gEditorfuncs.renderingfuncs.pfnPR_BindShader)
#define PR_LineWidth				(gEditorfuncs.renderingfuncs.pfnPR_LineWidth)
#define PR_PointSize				(gEditorfuncs.renderingfuncs.pfnPR_PointSize)
#define PR_Begin					(gEditorfuncs.renderingfuncs.pfnPR_Begin)
#define PR_Color4ub					(gEditorfuncs.renderingfuncs.pfnPR_Color4ub)
#define PR_Color4ubv				(gEditorfuncs.renderingfuncs.pfnPR_Color4ubv)
#define PR_Normal3fv				(gEditorfuncs.renderingfuncs.pfnPR_Normal3fv)
#define PR_TexCoord2f				(gEditorfuncs.renderingfuncs.pfnPR_TexCoord2f)
#define PR_TexCoord2fv				(gEditorfuncs.renderingfuncs.pfnPR_TexCoord2fv)
#define PR_Vertex3fv				(gEditorfuncs.renderingfuncs.pfnPR_Vertex3fv)
#define PR_End						(gEditorfuncs.renderingfuncs.pfnPR_End)
#define PR_GetState					(gEditorfuncs.renderingfuncs.pfnPR_GetState)
#define PR_SetState					(gEditorfuncs.renderingfuncs.pfnPR_SetState)
#define PR_GetViewInfo				(gEditorfuncs.renderingfuncs.pfnPR_GetViewInfo)
#define PR_CalcLighting				(gEditorfuncs.renderingfuncs.pfnPR_CalcLighting)
#define PR_GetMinAlpha				(gEditorfuncs.renderingfuncs.pfnPR_GetMinAlpha)

/* FileSystem API */
#define Sys_GetBaseDirectory		(gEditorfuncs.filesystemfuncs.pfnSys_GetBaseDirectory)
#define Sys_GetModDirectory			(gEditorfuncs.filesystemfuncs.pfnSys_GetModDirectory)
#define Sys_GetFallbackDirectory	(gEditorfuncs.filesystemfuncs.pfnSys_GetFallbackDirectory)
#define Sys_ExpandFileName			(gEditorfuncs.filesystemfuncs.pfnSys_ExpandFileName)
#define Sys_MakeLocalFileName		(gEditorfuncs.filesystemfuncs.pfnSys_MakeLocalFileName)
#define Sys_FileExists				(gEditorfuncs.filesystemfuncs.pfnSys_FileExists)
#define Sys_LoadFile				(gEditorfuncs.filesystemfuncs.pfnSys_LoadFile)
#define Sys_CreatePath				(gEditorfuncs.filesystemfuncs.pfnSys_CreatePath)

/* Math API */
#define Sys_PrintValue				(gEditorfuncs.mathfuncs.pfnSys_PrintValue)
#define Sys_PrintMapCoord			(gEditorfuncs.mathfuncs.pfnSys_PrintMapCoord)
#define Sys_PrintAxis				(gEditorfuncs.mathfuncs.pfnSys_PrintAxis)
#define Sys_SnapVertex				(gEditorfuncs.mathfuncs.pfnSys_SnapVertex)
#define Sys_SnapAxis				(gEditorfuncs.mathfuncs.pfnSys_SnapAxis)
#define Sys_SnapVertexToGrid		(gEditorfuncs.mathfuncs.pfnSys_SnapVertexToGrid)
#define Sys_SnapMapVertex			(gEditorfuncs.mathfuncs.pfnSys_SnapMapVertex)

#define V_VersionString				(gEditorfuncs.pfnV_VersionString)

#define Sys_GetTextureGamma			(gEditorfuncs.pfnSys_GetTextureGamma)

#define Global_GetCurrentWorld		(gEditorfuncs.pfnGlobal_GetCurrentWorld)

#define BuildPackageList			(gEditorfuncs.pfnBuildPackageList)

/* Entity API */
#define Entity_Create				(gEditorfuncs.entityfuncs.pfnEntity_Create)
#define Entity_Build				(gEditorfuncs.entityfuncs.pfnEntity_Build)
#define Entity_GetColor				(gEditorfuncs.entityfuncs.pfnEntity_GetColor)
#define Entity_SetColor				(gEditorfuncs.entityfuncs.pfnEntity_SetColor)
#define Entity_AddToVisGroup		(gEditorfuncs.entityfuncs.pfnEntity_AddToVisGroup)
#define Entity_RemoveFromVisGroup	(gEditorfuncs.entityfuncs.pfnEntity_RemoveFromVisGroup)
#define Entity_GetVisGroupIdent		(gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupIdent)
#define Entity_GetVisGroupCount		(gEditorfuncs.entityfuncs.pfnEntity_GetVisGroupCount)
#define Entity_FindByKeyValue		(gEditorfuncs.entityfuncs.pfnEntity_FindByKeyValue)
#define Entity_FindByClassname		(gEditorfuncs.entityfuncs.pfnEntity_FindByClassname)
#define Entity_FindByTargetname		(gEditorfuncs.entityfuncs.pfnEntity_FindByTargetname)

/* Brush API */
#define Brush_Create				(gEditorfuncs.brushfuncs.pfnBrush_Create)
#define Brush_Destroy				(gEditorfuncs.brushfuncs.pfnBrush_Destroy)
#define Brush_GetColor				(gEditorfuncs.brushfuncs.pfnBrush_GetColor)
#define Brush_SetColor				(gEditorfuncs.brushfuncs.pfnBrush_SetColor)
#define Brush_AddToVisGroup			(gEditorfuncs.brushfuncs.pfnBrush_AddToVisGroup)
#define Brush_RemoveFromVisGroup	(gEditorfuncs.brushfuncs.pfnBrush_RemoveFromVisGroup)
#define Brush_GetVisGroupIdent		(gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupIdent)
#define Brush_GetVisGroupCount		(gEditorfuncs.brushfuncs.pfnBrush_GetVisGroupCount)

/* Face API */
#define Face_Create					(gEditorfuncs.facefuncs.pfnFace_Create)
#define Face_Destroy				(gEditorfuncs.facefuncs.pfnFace_Destroy)

/* Patch API */
#define Patch_Create				(gEditorfuncs.patchfuncs.pfnPatch_Create)
#define Patch_CreateCap				(gEditorfuncs.patchfuncs.pfnPatch_CreateCap)
#define Patch_Destroy				(gEditorfuncs.patchfuncs.pfnPatch_Destroy)
#define Patch_NaturalizeTexture		(gEditorfuncs.patchfuncs.pfnPatch_NaturalizeTexture)
#define Patch_CapTexture			(gEditorfuncs.patchfuncs.pfnPatch_CapTexture)
#define Patch_FitTexture			(gEditorfuncs.patchfuncs.pfnPatch_FitTexture)
#define Patch_InterpolateExteriorPoints (gEditorfuncs.patchfuncs.pfnPatch_InterpolateExteriorPoints)

/* Overlay API */
#define Overlay_Create				(gEditorfuncs.overlayfuncs.pfnOverlay_Create)
#define Overlay_Destroy				(gEditorfuncs.overlayfuncs.pfnOverlay_Destroy)

/* Path API */
#define Path_Create					(gEditorfuncs.pathfuncs.pfnPath_Create)
#define Path_Destroy				(gEditorfuncs.pathfuncs.pfnPath_Destroy)
#define Path_Build					(gEditorfuncs.pathfuncs.pfnPath_Build)

/* Node API */
#define Node_Insert					(gEditorfuncs.nodefuncs.pfnNode_Insert)
#define Node_Append					(gEditorfuncs.nodefuncs.pfnNode_Append)
#define Node_Destroy				(gEditorfuncs.nodefuncs.pfnNode_Destroy)

/* Group API */
#define Group_Create				(gEditorfuncs.groupfuncs.pfnGroup_Create)
#define Group_Destroy				(gEditorfuncs.groupfuncs.pfnGroup_Destroy)
#define Group_AddGroup				(gEditorfuncs.groupfuncs.pfnGroup_AddGroup)
#define Group_AddBrush				(gEditorfuncs.groupfuncs.pfnGroup_AddBrush)
#define Group_AddEntity				(gEditorfuncs.groupfuncs.pfnGroup_AddEntity)
#define Group_GetColor				(gEditorfuncs.groupfuncs.pfnGroup_GetColor)
#define Group_SetColor				(gEditorfuncs.groupfuncs.pfnGroup_SetColor)

/* Camera API */
#define Camera_Create				(gEditorfuncs.camerafuncs.pfnCamera_Create)
#define Camera_Destroy				(gEditorfuncs.camerafuncs.pfnCamera_Destroy)
#define Camera_GetColor				(gEditorfuncs.camerafuncs.pfnCamera_GetColor)
#define Camera_SetColor				(gEditorfuncs.camerafuncs.pfnCamera_SetColor)
#define Camera_Setup				(gEditorfuncs.camerafuncs.pfnCamera_Setup)

/* Shader API */
#define Shader_Create				(gEditorfuncs.shaderfuncs.pfnShader_Create)
#define Shader_Lookup				(gEditorfuncs.shaderfuncs.pfnShader_Lookup)
#define Shader_Destroy				(gEditorfuncs.shaderfuncs.pfnShader_Destroy)
#define Shader_AddStage				(gEditorfuncs.shaderfuncs.pfnShader_AddStage)
#define Shader_RemoveStage			(gEditorfuncs.shaderfuncs.pfnShader_RemoveStage)
#define Shader_Finish				(gEditorfuncs.shaderfuncs.pfnShader_Finish)
#define Shader_GetWhiteTexture		(gEditorfuncs.shaderfuncs.pfnShader_GetWhiteTexture)
#define Shader_GetBlackTexture		(gEditorfuncs.shaderfuncs.pfnShader_GetBlackTexture)
#define Shader_LookupTexture		(gEditorfuncs.shaderfuncs.pfnShader_LookupTexture)
#define Shader_UploadTexture		(gEditorfuncs.shaderfuncs.pfnShader_UploadTexture)
#define Shader_DestroyTexture		(gEditorfuncs.shaderfuncs.pfnShader_DestroyTexture)

/* VisGroup API */
#define VisGroup_Add				(gEditorfuncs.visgroupfuncs.pfnVisGroup_Add)
#define VisGroup_Remove				(gEditorfuncs.visgroupfuncs.pfnVisGroup_Remove)
#define VisGroup_Modify				(gEditorfuncs.visgroupfuncs.pfnVisGroup_Modify)
#define VisGroup_GetCount			(gEditorfuncs.visgroupfuncs.pfnVisGroup_GetCount)
#define VisGroup_GetIndex			(gEditorfuncs.visgroupfuncs.pfnVisGroup_GetIndex)
#define VisGroup_GetData			(gEditorfuncs.visgroupfuncs.pfnVisGroup_GetData)
#define VisGroup_RebuildIdents		(gEditorfuncs.visgroupfuncs.pfnVisGroup_RebuildIdents)

/* Undo API*/
#define Undo_Start					(gEditorfuncs.undofuncs.pfnUndo_Start)
#define Undo_End					(gEditorfuncs.undofuncs.pfnUndo_End)
#define Undo_AddGroup				(gEditorfuncs.undofuncs.pfnUndo_AddGroup)
#define Undo_AddBrush				(gEditorfuncs.undofuncs.pfnUndo_AddBrush)
#define Undo_AddPath				(gEditorfuncs.undofuncs.pfnUndo_AddPath)
#define Undo_AddNode				(gEditorfuncs.undofuncs.pfnUndo_AddNode)
#define Undo_AddEntity				(gEditorfuncs.undofuncs.pfnUndo_AddEntity)
#define Undo_DeleteGroup			(gEditorfuncs.undofuncs.pfnUndo_DeleteGroup)
#define Undo_DeleteBrush			(gEditorfuncs.undofuncs.pfnUndo_DeleteBrush)
#define Undo_DeletePath				(gEditorfuncs.undofuncs.pfnUndo_DeletePath)
#define Undo_DeleteNode				(gEditorfuncs.undofuncs.pfnUndo_DeleteNode)
#define Undo_DeleteEntity			(gEditorfuncs.undofuncs.pfnUndo_DeleteEntity)
#define Undo_StoreFace				(gEditorfuncs.undofuncs.pfnUndo_StoreFace)
#define Undo_StoreBrush				(gEditorfuncs.undofuncs.pfnUndo_StoreBrush)
#define Undo_StorePath				(gEditorfuncs.undofuncs.pfnUndo_StorePath)
#define Undo_StoreNode				(gEditorfuncs.undofuncs.pfnUndo_StoreNode)
#define Undo_StoreEntity			(gEditorfuncs.undofuncs.pfnUndo_StoreEntity)
#define Undo_AddSelectedEntities	(gEditorfuncs.undofuncs.pfnUndo_AddSelectedEntities)
#define Undo_AddSelectedBrushes		(gEditorfuncs.undofuncs.pfnUndo_AddSelectedBrushes)
#define Undo_AddSelectedNodes		(gEditorfuncs.undofuncs.pfnUndo_AddSelectedNodes)
#define Undo_DeleteSelectedEntities	(gEditorfuncs.undofuncs.pfnUndo_DeleteSelectedEntities)
#define Undo_DeleteSelectedBrushes	(gEditorfuncs.undofuncs.pfnUndo_DeleteSelectedBrushes)
#define Undo_StoreSelectedEntities	(gEditorfuncs.undofuncs.pfnUndo_StoreSelectedEntities)
#define Undo_DeleteSelectedNodes	(gEditorfuncs.undofuncs.pfnUndo_DeleteSelectedNodes)
#define Undo_StoreSelectedBrushes	(gEditorfuncs.undofuncs.pfnUndo_StoreSelectedBrushes)
#define Undo_StoreSelectedNodes		(gEditorfuncs.undofuncs.pfnUndo_StoreSelectedNodes)
#define Undo_StoreSelectedFaces		(gEditorfuncs.undofuncs.pfnUndo_StoreSelectedFaces)

/* Dialog API */
#define Dialog_CheckOptions			(gEditorfuncs.dialogfuncs.pfnDialog_CheckOptions)
#define Dialog_MessageBox			(gEditorfuncs.dialogfuncs.pfnDialog_MessageBox)
#define Dialog_Begin				(gEditorfuncs.dialogfuncs.pfnDialog_Begin)
#define Dialog_InitExternalCommand	(gEditorfuncs.dialogfuncs.pfnDialog_InitExternalCommand)
#define Dialog_InitInternalCommand	(gEditorfuncs.dialogfuncs.pfnDialog_InitInternalCommand)
#define Dialog_SetProgress			(gEditorfuncs.dialogfuncs.pfnDialog_SetProgress)
#define Dialog_AddTextEdit			(gEditorfuncs.dialogfuncs.pfnDialog_AddTextEdit)
#define Dialog_AddRadioBox			(gEditorfuncs.dialogfuncs.pfnDialog_AddRadioBox)
#define Dialog_AddCheckBox			(gEditorfuncs.dialogfuncs.pfnDialog_AddCheckBox)
#define Dialog_AddSpinBox			(gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBox)
#define Dialog_AddSpinBoxFloat		(gEditorfuncs.dialogfuncs.pfnDialog_AddSpinBoxFloat)
#define Dialog_AddFileEdit			(gEditorfuncs.dialogfuncs.pfnDialog_AddFileEdit)
#define Dialog_AddFileList			(gEditorfuncs.dialogfuncs.pfnDialog_AddFileList)
#define Dialog_AddComboBox			(gEditorfuncs.dialogfuncs.pfnDialog_AddComboBox)
#define Dialog_AddDirectoryEdit		(gEditorfuncs.dialogfuncs.pfnDialog_AddDirectoryEdit)
#define Dialog_QueryArgument		(gEditorfuncs.dialogfuncs.pfnDialog_QueryArgument)
#define Dialog_QueryArgumentFloat	(gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentFloat)
#define Dialog_QueryArgumentInt		(gEditorfuncs.dialogfuncs.pfnDialog_QueryArgumentInt)
#define Dialog_End					(gEditorfuncs.dialogfuncs.pfnDialog_End)
#define Dialog_Printf				(gEditorfuncs.dialogfuncs.pfnDialog_Printf)
#define Dialog_BeginWait			(gEditorfuncs.dialogfuncs.pfnDialog_BeginWait)
#define Dialog_EndWait				(gEditorfuncs.dialogfuncs.pfnDialog_EndWait)

// clang-format on

#endif // !PLUGINEDITORFUNCTIONS_H
