## Plugin API TO-DO List

### General
- Steam version introduced a always-NULL pointer called firstPtr to most of the plugin structs, would be great to guess what is it used for.
- Some of the data structs have unkint_renderXXX fields used for rendering.

### PluginActions.h
- Would be great to get where is m_unknown field is used.
- A better name for m_state?

### PluginAPI.h
- More documentation on Undo API, Overlay API, Patch API, etc.

### PluginData.h
- vpFilterTextureName is never called, recheck the editor code.
- qPalette_s struct might contain other data, but it also might be the same as in Quake III Arena.
- qParticlesData_s, gap2 is probably the same as in qStudioData_s.
- Remove confusing documentation from Archive IO functions. It's useful only when creating a pluginloader implementation.
- Rename GCList_t?

### PluginEntity.h
- Guess the unknown ENT_BLDFLG_ bits.
- Finish qSplineNode_s struct.
- Add more documentation to the qEntity_s struct.

### PluginFaces.h
- qVertex_s uv thing.
- Guess the gaps in qFace_s. gap1 is probably m_parent.

### PluginGroups.h
- Recheck the m_parent.

### PluginProfile.h
- Guess the remaining profile flags.
- Reorganize mapProfile_s, maybe split to a small structs.
- Give mapProfile_s members better names.
- Firuge out the unknown fields inside mapProfile_s.

### PluginRender.h
- Rename viewInfo_s unknown members.
- Try to find more TEXTURE_FLAGs.
- Give better names to qShaderStageSubData_s, qShaderStageData_s members.
- Figure out what is qShaderStageSubData_s, qShaderStageData_s and where gaps are used.
- Guess gaps inside qShaderStage_s.
- Figure out missing MAT_ flags, SHADER_FLAGs.
- Guess gaps inside qShader_s.
- Give better names to qShader_s members.
- Figure out what data is stored in qSpriteDrawData_s, qStudioDrawData_s, qParticlesDrawData_s.

### PluginWorld.h
- qOverlayData_s is probably a qVertex_s
- Fix/remove qDecalFragment_s completly.
- Guess the missing fields inside qPatch_s.
- Fillup qTrans_s, qSky_s.
