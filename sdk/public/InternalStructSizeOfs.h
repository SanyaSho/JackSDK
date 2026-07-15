//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( INTERNALSTRUCTSIZEOFS_H )
#define INTERNALSTRUCTSIZEOFS_H

/*
 This header is used to define internal struct sizeofs to prevent going out of bounds when adding/removing new elements.
*/

// clang-format off

#if defined( JACK_64BIT )

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
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QCAMERA_S				72
#else
#define SIZEOF_QCAMERA_S				64
#endif

// PluginData.h (64Bit)
#define SIZEOF_QSPRITEDATA_S			40
#define SIZEOF_QSTUDIODATA_S			72
#define SIZEOF_QPARTICLESDATA_S			64
#define SIZEOF_QARCHIVEDATA_S			64

// PluginEntity.h (64Bit)
#define SIZEOF_EPAIR_S					24
#define SIZEOF_QSPLINENODE_S			144
#define SIZEOF_QENTITYRENDERMODE_S		16
#define SIZEOF_QENTITYSTATE_S			48
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QENTITY_S				368
#else
#define SIZEOF_QENTITY_S				360
#endif

// PluginFaces.h (64Bit)
#define SIZEOF_QPLANE_S					20
#define SIZEOF_QTEXDEF_S				128
#define SIZEOF_QVERTEX_S				24
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QFACE_S					272
#else
#define SIZEOF_QFACE_S					264
#endif

// PluginGroups.h (64Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QGROUPS_S				80
#else
#define SIZEOF_QGROUPS_S				72
#endif

// PluginNodes.h (64Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QNODE_S					144
#else
#define SIZEOF_QNODE_S					136
#endif

// PluginPaths.h (64Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QPATH_S					112
#else
#define SIZEOF_QPATH_S					104
#endif

// PluginPrimitives.h (64Bit)
#define SIZEOF_PRIMITIVEDESC_S			40
#define SIZEOF_STPRIMITIVECREATESTRUCT	184

// PluginProfile.h (64Bit)
#define SIZEOF_MAPPROFILE_S				2960
#define SIZEOF_SHADEREDITORCALLBACKS_S	56 /* NOTE: Size is unknown */

// PluginRender.h (64Bit)
#define SIZEOF_QTEXTURE_S				48
#define SIZEOF_QSHADERSTAGEDATA_S		284 /* NOTE: Size is unknown */
#define SIZEOF_QSHADERSTAGE_S			384
#if defined( WIN32 )
#define SIZEOF_QSHADER_S				640
#else
#define SIZEOF_QSHADER_S				1408
#endif

// PluginWorld.h (64Bit)
#define SIZEOF_QOVERLAYDATA_S			24
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QOVERLAY_S				304
#else
#define SIZEOF_QOVERLAY_S				296
#endif
#define SIZEOF_QDECALFRAGMENT_S			32
#define SIZEOF_QPATCHDATA_S				36 /* NOTE: Size is unknown */
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QPATCH_S					37128
#define SIZEOF_QBRUSH_S					152
#else
#define SIZEOF_QPATCH_S					37120
#define SIZEOF_QBRUSH_S					144
#endif
#define SIZEOF_QTRANS_S					24
#define SIZEOF_QSKY_S					24
#define SIZEOF_QSKYDATA_S				24 /* NOTE: Size is unknown */
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QWORLD_S					192
#else
#define SIZEOF_QWORLD_S					184
#endif

#elif defined( JACK_32BIT )

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
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QCAMERA_S				52
#else
#define SIZEOF_QCAMERA_S				48
#endif

// PluginData.h (32Bit)
#define SIZEOF_QSPRITEDATA_S			32 /* TODO: Recheck */
#define SIZEOF_QSTUDIODATA_S			56 /* TODO: Recheck */
#define SIZEOF_QPARTICLESDATA_S			56 /* TODO: Recheck */
#define SIZEOF_QARCHIVEDATA_S			36 /* TODO: Recheck */

// PluginEntity.h (32Bit)
#define SIZEOF_EPAIR_S					12
#define SIZEOF_QSPLINENODE_S			132
#define SIZEOF_QENTITYRENDERMODE_S		16
#define SIZEOF_QENTITYSTATE_S			48
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QENTITY_S				252
#else
#define SIZEOF_QENTITY_S				248
#endif

// PluginFaces.h (32Bit)
#define SIZEOF_QPLANE_S					20
#define SIZEOF_QTEXDEF_S				128
#define SIZEOF_QVERTEX_S				24
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QFACE_S					228
#else
#define SIZEOF_QFACE_S					224
#endif

// PluginGroups.h (32Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QGROUPS_S				44
#else
#define SIZEOF_QGROUPS_S				40
#endif

// PluginNodes.h (32Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QNODE_S					100
#else
#define SIZEOF_QNODE_S					96
#endif

// PluginPaths.h (32Bit)
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QPATH_S					72
#else
#define SIZEOF_QPATH_S					68
#endif

// PluginPrimitives.h (32Bit)
#define SIZEOF_PRIMITIVEDESC_S			28
#define SIZEOF_STPRIMITIVECREATESTRUCT	176

// PluginProfile.h (32Bit)
#define SIZEOF_MAPPROFILE_S				2952
#define SIZEOF_SHADEREDITORCALLBACKS_S	28 /* NOTE: Size is unknown */

// PluginRender.h (32Bit)
#define SIZEOF_QTEXTURE_S				40
#define SIZEOF_QSHADERSTAGEDATA_S		284 /* NOTE: Size is unknown*/
#define SIZEOF_QSHADERSTAGE_S			344
#if defined( WIN32 )
#define SIZEOF_QSHADER_S				584
#else
#define SIZEOF_QSHADER_S				1348
#endif

// PluginWorld.h (32Bit)
#define SIZEOF_QOVERLAYDATA_S			24
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QOVERLAY_S				288
#else
#define SIZEOF_QOVERLAY_S				284
#endif
#define SIZEOF_QDECALFRAGMENT_S			16
#define SIZEOF_QPATCHDATA_S				36 /* NOTE: Size is unknown */
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QPATCH_S					37084
#define SIZEOF_QBRUSH_S					96
#else
#define SIZEOF_QPATCH_S					37080
#define SIZEOF_QBRUSH_S					92
#endif
#define SIZEOF_QTRANS_S					16
#define SIZEOF_QSKY_S					12
#define SIZEOF_QSKYDATA_S				20 /* NOTE: Size is unknown */
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
#define SIZEOF_QWORLD_S					116
#else
#define SIZEOF_QWORLD_S					112
#endif

#else

#error

#endif // JACK_64BIT

// clang-format on

#endif // !INTERNALSTRUCTSIZEOFS_H
