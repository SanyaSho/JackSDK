#=============================================================================
# JackPluginSDK.cmake
#
# Use this file to include the SDK to your project
# You can define JACK_API_VERSION to select which API level will
# be implemented. See head of BaseTypes.h for more info.
#
# If you're using another buildsystem (not CMake) then you must define
# JACK_32BIT or JACK_64BIT depending on your platform.
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
				"${JACK_SDK_DIR}/public/InternalStructSizeOfs.h"
				"${JACK_SDK_DIR}/public/PluginAPI.h"
				"${JACK_SDK_DIR}/public/PluginActions.h"
				"${JACK_SDK_DIR}/public/PluginCamera.h"
				"${JACK_SDK_DIR}/public/PluginData.h"
				"${JACK_SDK_DIR}/public/PluginEditorFunctions.h"
				"${JACK_SDK_DIR}/public/PluginEntity.h"
				"${JACK_SDK_DIR}/public/PluginFaces.h"
				"${JACK_SDK_DIR}/public/PluginGroups.h"
				"${JACK_SDK_DIR}/public/PluginMath.h"
				"${JACK_SDK_DIR}/public/PluginMeta.h"
				"${JACK_SDK_DIR}/public/PluginNodes.h"
				"${JACK_SDK_DIR}/public/PluginPaths.h"
				"${JACK_SDK_DIR}/public/PluginPrimitives.h"
				"${JACK_SDK_DIR}/public/PluginProfile.h"
				"${JACK_SDK_DIR}/public/PluginRender.h"
				"${JACK_SDK_DIR}/public/PluginWorld.h"
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

	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		target_compile_definitions(
			${target} PRIVATE

			JACK_64BIT
		)
	else()
		target_compile_definitions(
			${target} PRIVATE

			JACK_32BIT
		)
	endif()

endfunction()

