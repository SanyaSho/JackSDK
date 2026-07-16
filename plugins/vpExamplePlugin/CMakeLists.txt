#-----------------------------------------------------------------------------
#	vpExamplePlugin
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( EXAMPLEPLUGIN_SOURCE_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"CustomSpriteFormatExample.cpp"
			"CustomTextureFormatExample.cpp"

			"DialogsExample.cpp"
			#"DoomWadLoader.cpp"

			"ExamplePlugin.cpp"
			"PrimitivesExample.cpp"

			"MapReader.cpp"

			"vstdlib_static.cpp"

			"../common/ImageLoader.cpp"
		#}
	)
END_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )

set( EXAMPLEPLUGIN_HEADER_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			#"DoomWadLoader.h"

			"MapReader.h"

			"vstdlib_static.h"

			"../common/ImageLoader.h"
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
)

target_compile_definitions(
	exampleplugin PRIVATE

	# ImageLoader
	ENABLE_STB_IMAGELOADER
)

target_use_jackpluginsdk( exampleplugin )
