#-----------------------------------------------------------------------------
#	vpExamplePlugin
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )
include( "${CMAKE_CURRENT_LIST_DIR}/assimp/assimp.cmake" )

set( EXAMPLEPLUGIN_SOURCE_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"AssimpStudioRender.cpp"
			"CustomSpriteFormatExample.cpp"
			"CustomTextureFormatExample.cpp"

			"DialogsExample.cpp"
			#"DoomWadLoader.cpp"

			"ExamplePlugin.cpp"
			"PrimitivesExample.cpp"

			"MapReader.cpp"

			"../common/ImageLoader.cpp"
			"../common/vstdlib_static.cpp"
		#}
	)
END_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )

set( EXAMPLEPLUGIN_HEADER_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"AssimpStudioRender.h"
			#"DoomWadLoader.h"

			"MapReader.h"

			"../common/ImageLoader.h"
			"../common/vstdlib_static.h"
		#}
	)
END_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )

add_library(
	exampleplugin MODULE
	${EXAMPLEPLUGIN_SOURCE_FILES} ${EXAMPLEPLUGIN_HEADER_FILES}
)

install_library(
	TARGET exampleplugin
	INSTALL_OUTNAME "vpExamplePlugin"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_include_directories(
	exampleplugin PRIVATE

	"."
	"../common"

	"assimp/include"
)

target_compile_definitions(
	exampleplugin PRIVATE

	# ImageLoader
	ENABLE_STB_IMAGELOADER
)

#target_compile_options(
#	exampleplugin PRIVATE
#
#	/fsanitize=address
#)

target_use_jackpluginsdk( exampleplugin "${SRCDIR}/sdk" )

target_use_assimp( exampleplugin "${CMAKE_CURRENT_LIST_DIR}/assimp" )
