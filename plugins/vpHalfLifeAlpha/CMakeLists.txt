#-----------------------------------------------------------------------------
#	vpHalfLifeAlpha
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( HALFLIFEALPHA_SOURCE_FILES )
BEGIN_SRC( HALFLIFEALPHA_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"actions.cpp"
			"gamma.cpp"
			"io.cpp"
			"main.cpp"
			"profiles.cpp"

			"serializer_map.cpp"
			"serializer_rmf.cpp"

			"../common/DataSerializer.cpp"
			"../common/vstdlib_static.cpp"
		#}
	)
END_SRC( HALFLIFEALPHA_SOURCE_FILES "Source Files" )

set( HALFLIFEALPHA_HEADER_FILES )
BEGIN_SRC( HALFLIFEALPHA_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"gamma.h"

			"serializer_map.h"
			"serializer_rmf.h"

			"../common/DataSerializer.h"
			"../common/vstdlib_static.h"
		#}
	)
END_SRC( HALFLIFEALPHA_HEADER_FILES "Header Files" )

set( GOLDSRC_SOURCE_FILES )
BEGIN_SRC( GOLDSRC_SOURCE_FILES "GoldSrc Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"engine/gl_model.cpp"
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
			"engine/bspfile.h"
			"engine/gl_model.h"
			"engine/mathlib.h"
			"engine/quakedef.h"
			"engine/r_studio.h"
			"engine/spritegn.h"
			"engine/studio.h"
			"engine/wad.h"
		#}
	)
END_SRC( GOLDSRC_HEADER_FILES "GoldSrc Header Files" )

add_library(
	halflifealpha MODULE
	${HALFLIFEALPHA_SOURCE_FILES} ${HALFLIFEALPHA_HEADER_FILES}
	${GOLDSRC_SOURCE_FILES} ${GOLDSRC_HEADER_FILES}
)

install_library(
	TARGET halflifealpha
	# NOTE: This plugin has I in it's name because of one stupid J.A.C.K. bug
	# If vpHalfLife is loaded AFTER any custom plugin with .mdl support is loaded then it wont render the release studio models, however this happens only on Windows
	INSTALL_OUTNAME "vpIHalfLifeAlpha"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_include_directories(
	halflifealpha PRIVATE

	"."
	"../common"
)

target_use_jackpluginsdk( halflifealpha "${SRCDIR}/sdk" )
