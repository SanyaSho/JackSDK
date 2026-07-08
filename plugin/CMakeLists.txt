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
			#"DoomWadLoader.cpp"

			"ExamplePlugin.cpp"

			"MapReader.cpp"
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

target_use_jackpluginsdk( exampleplugin )
