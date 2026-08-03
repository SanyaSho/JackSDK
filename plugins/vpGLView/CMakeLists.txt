#-----------------------------------------------------------------------------
#	vpGLView
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( GLVIEW_SOURCE_FILES )
BEGIN_SRC( GLVIEW_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"io.cpp"
			"main.cpp"

			"serializer_gl.cpp"

			"../common/DataSerializer.cpp"
			"../common/vstdlib_static.cpp"
		#}
	)
END_SRC( GLVIEW_SOURCE_FILES "Source Files" )

set( GLVIEW_HEADER_FILES )
BEGIN_SRC( GLVIEW_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"serializer_gl.h"

			"../common/DataSerializer.h"
			"../common/vstdlib_static.h"
		#}
	)
END_SRC( GLVIEW_HEADER_FILES "Header Files" )

add_library(
	glview MODULE
	${GLVIEW_SOURCE_FILES} ${GLVIEW_HEADER_FILES}
)

install_library(
	TARGET glview
	INSTALL_OUTNAME "vpGLView"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_include_directories(
	glview PRIVATE

	"."
	"../common"
)

target_use_jackpluginsdk( glview "${SRCDIR}/sdk" )
