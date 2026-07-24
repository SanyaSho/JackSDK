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

#if !defined( INTERNALSTRUCTSIZEOFS_H )
#define INTERNALSTRUCTSIZEOFS_H

/*
 This header is used to define internal struct sizeofs to prevent going out of bounds when adding/removing new elements.
*/

// clang-format off

#if defined( JACK_64BIT )

#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define FIRSTPTR_OFFSET					0
#else
#define FIRSTPTR_OFFSET					8
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

// PluginActions.h (64Bit)
#define SIZEOF_PLUGINACTIONDESC_T		48

// PluginAPI.h (64Bit)
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_PLUGIN_FUNCS_T			1512
#elif JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
#define SIZEOF_PLUGIN_FUNCS_T			1464
#else
#error
#endif

// PluginCamera.h (64Bit)
#define SIZEOF_QCAMERA_S				72 - FIRSTPTR_OFFSET

// PluginData.h (64Bit)
#define SIZEOF_QSPRITEDATA_S			40
#define SIZEOF_QSTUDIODATA_S			72
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QPARTICLESDATA_S			64
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QARCHIVEDATA_S			64 /* TODO: Recheck */

// PluginEntity.h (64Bit)
#define SIZEOF_EPAIR_S					24
#define SIZEOF_QSPLINENODE_S			144
#define SIZEOF_QENTITYRENDERMODE_S		16
#define SIZEOF_QENTITYSTATE_S			48
#define SIZEOF_QENTITY_S				368 - FIRSTPTR_OFFSET

// PluginFaces.h (64Bit)
#define SIZEOF_QPLANE_S					20
#define SIZEOF_QTEXDEF_S				128
#define SIZEOF_QVERTEX_S				24
#define SIZEOF_QFACE_S					272 - FIRSTPTR_OFFSET

// PluginGroups.h (64Bit)
#define SIZEOF_QGROUPS_S				80 - FIRSTPTR_OFFSET

// PluginNodes.h (64Bit)
#define SIZEOF_QNODE_S					144 - FIRSTPTR_OFFSET

// PluginPaths.h (64Bit)
#define SIZEOF_QPATH_S					112 - FIRSTPTR_OFFSET

// PluginPrimitives.h (64Bit)
#define SIZEOF_PRIMITIVEDESC_S			40
#define SIZEOF_STPRIMITIVECREATESTRUCT	184

// PluginProfile.h (64Bit)
#define SIZEOF_MAPPROFILE_S				2960
#define SIZEOF_SHADEREDITORCALLBACKS_S	56 /* NOTE: Size is unknown */

// PluginRender.h (64Bit)
#define SIZEOF_QTEXTURE_S				48
#define SIZEOF_QSHADERSTAGESUBDATA_S	48 /* NOTE: Size is unknown */
#define SIZEOF_QSHADERSTAGEDATA_S		284 /* NOTE: Size is unknown */
#define SIZEOF_QSHADERSTAGE_S			384
#if defined( WIN32 )
#define SIZEOF_QSHADER_S				640
#else
#define SIZEOF_QSHADER_S				1408
#endif
#define SIZEOF_QSPRITEDRAWDATA_S		24
#define SIZEOF_QDECALDRAWDATA_S			40
#define SIZEOF_QSTUDIODRAWDATA_S		72
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QPARTICLESDRAWDATA_S		48
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

// PluginWorld.h (64Bit)
#define SIZEOF_QOVERLAYDATA_S			24
#define SIZEOF_QOVERLAY_S				304 - FIRSTPTR_OFFSET
#define SIZEOF_QDECALFRAGMENT_S			32
#define SIZEOF_QPATCHDATA_S				36 /* NOTE: Size is unknown */
#define SIZEOF_QPATCH_S					37128 - FIRSTPTR_OFFSET
#define SIZEOF_QBRUSH_S					152 - FIRSTPTR_OFFSET
#define SIZEOF_QTRANS_S					24
#define SIZEOF_QSKY_S					24
#define SIZEOF_QSKYDATA_S				24 /* NOTE: Size is unknown */
#define SIZEOF_QWORLD_S					192 - FIRSTPTR_OFFSET

#elif defined( JACK_32BIT )

#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define FIRSTPTR_OFFSET					0
#else
#define FIRSTPTR_OFFSET					4
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

// PluginActions.h (32Bit)
#define SIZEOF_PLUGINACTIONDESC_T		32

// PluginAPI.h (32Bit)
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_PLUGIN_FUNCS_T			756
#elif JACK_API_VERSION <= API_VERSION_STEAM_PUBLIC
#define SIZEOF_PLUGIN_FUNCS_T			732
#else
#error
#endif

// PluginCamera.h (32Bit)
#define SIZEOF_QCAMERA_S				52 - FIRSTPTR_OFFSET

// PluginData.h (32Bit)
#define SIZEOF_QSPRITEDATA_S			28
#define SIZEOF_QSTUDIODATA_S			56
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QPARTICLESDATA_S			52
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QARCHIVEDATA_S			36 /* TODO: Recheck */

// PluginEntity.h (32Bit)
#define SIZEOF_EPAIR_S					12
#define SIZEOF_QSPLINENODE_S			132
#define SIZEOF_QENTITYRENDERMODE_S		16
#define SIZEOF_QENTITYSTATE_S			48
#define SIZEOF_QENTITY_S				252 - FIRSTPTR_OFFSET

// PluginFaces.h (32Bit)
#define SIZEOF_QPLANE_S					20
#define SIZEOF_QTEXDEF_S				128
#define SIZEOF_QVERTEX_S				24
#define SIZEOF_QFACE_S					228 - FIRSTPTR_OFFSET

// PluginGroups.h (32Bit)
#define SIZEOF_QGROUPS_S				44 - FIRSTPTR_OFFSET

// PluginNodes.h (32Bit)
#define SIZEOF_QNODE_S					100 - FIRSTPTR_OFFSET

// PluginPaths.h (32Bit)
#define SIZEOF_QPATH_S					72 - FIRSTPTR_OFFSET

// PluginPrimitives.h (32Bit)
#define SIZEOF_PRIMITIVEDESC_S			28
#define SIZEOF_STPRIMITIVECREATESTRUCT	176

// PluginProfile.h (32Bit)
#define SIZEOF_MAPPROFILE_S				2952
#define SIZEOF_SHADEREDITORCALLBACKS_S	28 /* NOTE: Size is unknown */

// PluginRender.h (32Bit)
#define SIZEOF_QTEXTURE_S				40
#define SIZEOF_QSHADERSTAGESUBDATA_S	48 /* NOTE: Size is unknown */
#define SIZEOF_QSHADERSTAGEDATA_S		284 /* NOTE: Size is unknown*/
#define SIZEOF_QSHADERSTAGE_S			344
#if defined( WIN32 )
#define SIZEOF_QSHADER_S				584
#else
#define SIZEOF_QSHADER_S				1348
#endif
#define SIZEOF_QSPRITEDRAWDATA_S		20
#define SIZEOF_QDECALDRAWDATA_S			32
#define SIZEOF_QSTUDIODRAWDATA_S		64
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
#define SIZEOF_QPARTICLESDRAWDATA_S		40
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

// PluginWorld.h (32Bit)
#define SIZEOF_QOVERLAYDATA_S			24
#define SIZEOF_QOVERLAY_S				288 - FIRSTPTR_OFFSET
#define SIZEOF_QDECALFRAGMENT_S			16
#define SIZEOF_QPATCHDATA_S				36 /* NOTE: Size is unknown */
#define SIZEOF_QPATCH_S					37084 - FIRSTPTR_OFFSET
#define SIZEOF_QBRUSH_S					96 - FIRSTPTR_OFFSET
#define SIZEOF_QTRANS_S					16
#define SIZEOF_QSKY_S					12
#define SIZEOF_QSKYDATA_S				20 /* NOTE: Size is unknown */
#define SIZEOF_QWORLD_S					116 - FIRSTPTR_OFFSET

#else

#error

#endif // JACK_64BIT

// clang-format on

#endif // !INTERNALSTRUCTSIZEOFS_H
