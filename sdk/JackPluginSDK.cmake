#=============================================================================
# JackPluginSDK.cmake
#
# $
#=============================================================================

include_guard( GLOBAL )

function( target_use_jackpluginsdk target )
	set( JACK_SDK_DIR "${SRCDIR}/sdk" )

	set( JACK_SDK_HEADER_FILES )
	BEGIN_SRC( JACK_SDK_HEADER_FILES "SDK Header Files" )
		SRC_GRP(
			SOURCES
			#{
				"${JACK_SDK_DIR}/public/BaseTypes.h"
				"${JACK_SDK_DIR}/public/PluginAPI.h"
				"${JACK_SDK_DIR}/public/PluginActions.h"
				"${JACK_SDK_DIR}/public/PluginCamera.h"
				"${JACK_SDK_DIR}/public/PluginData.h"
				"${JACK_SDK_DIR}/public/PluginEditorFunctions.h"
				"${JACK_SDK_DIR}/public/PluginEntity.h"
				"${JACK_SDK_DIR}/public/PluginFaces.h"
				"${JACK_SDK_DIR}/public/PluginMath.h"
				"${JACK_SDK_DIR}/public/PluginNodes.h"
				"${JACK_SDK_DIR}/public/PluginPaths.h"
				"${JACK_SDK_DIR}/public/PluginProfile.h"
				"${JACK_SDK_DIR}/public/PluginRender.h"
			#}
		)

		SRC_GRP(
			SUBGROUP "GLEW"
			SOURCES
			#{
				"${JACK_SDK_DIR}/GLEW/include/GL/eglew.h"
				"${JACK_SDK_DIR}/GLEW/include/GL/glew.h"
				"${JACK_SDK_DIR}/GLEW/include/GL/glxew.h"
				"${JACK_SDK_DIR}/GLEW/include/GL/wglew.h"
			#}
		)
	END_SRC( JACK_SDK_HEADER_FILES "SDK Header Files" )

	target_sources(
		${target} PRIVATE

		${JACK_SDK_HEADER_FILES}
	)

	target_include_directories(
		${target} PRIVATE

		${JACK_SDK_DIR}/public
		${JACK_SDK_DIR}/GLEW/include
	)

	target_compile_definitions(
		${target} PRIVATE

		PLUGINSDK
	)
endfunction()

