//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Copyright (C) 2026  SanyaSho
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
// License along with this library; if not, see <https://www.gnu.org/licenses/>.
//
//=====================================================================================//

#if !defined( PLUGINMETA_H )
#define PLUGINMETA_H

/*
 Use this header to use all plugin features at once
*/

#include "BaseTypes.h" // Includes InternalStructSizeOfs.h and PluginMath.h

#include "PluginAPI.h" // Includes PluginEditorFunctions.h

#include "PluginActions.h"
#include "PluginCamera.h"
#include "PluginData.h"
#include "PluginEntity.h"
#include "PluginFaces.h"
#include "PluginGroups.h"
#include "PluginNodes.h"
#include "PluginPaths.h"
#include "PluginPrimitives.h"
#include "PluginProfile.h"
#include "PluginRender.h"
#include "PluginWorld.h"

// Include GLEW headers for Shader_UploadTexture
#include "GL/glew.h"

#endif // !PLUGINMETA_H
