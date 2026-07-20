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
			#"DoomWadLoader.h"

			"MapReader.h"

			"../common/ImageLoader.h"
			"../common/vstdlib_static.h"
		#}
	)
END_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )

set( GOLDSRC_SOURCE_FILES )
BEGIN_SRC( GOLDSRC_SOURCE_FILES "GoldSrc Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"engine/mathlib.cpp"
			"engine/l_studio.cpp"
			"engine/r_studio.cpp"
		#}
	)
END_SRC( GOLDSRC_SOURCE_FILES "GoldSrc Source Files" )

set( GOLDSRC_HEADER_FILES )
BEGIN_SRC( GOLDSRC_HEADER_FILES "GoldSrc Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"engine/mathlib.h"
			"engine/quakedef.h"
			"engine/r_studio.h"
			"engine/studio.h"
		#}
	)
END_SRC( GOLDSRC_HEADER_FILES "GoldSrc Header Files" )

add_library(
	exampleplugin MODULE
	${EXAMPLEPLUGIN_SOURCE_FILES} ${EXAMPLEPLUGIN_HEADER_FILES}
	${GOLDSRC_SOURCE_FILES} ${GOLDSRC_HEADER_FILES}
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

#target_compile_options(
#	exampleplugin PRIVATE
#
#	/fsanitize=address
#)

target_use_jackpluginsdk( exampleplugin )

