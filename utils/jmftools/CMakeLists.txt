#-----------------------------------------------------------------------------
#	jmftools
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( JMFTOOLS_SOURCE_FILES )
BEGIN_SRC( JMFTOOLS_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"logger.cpp"
			"main.cpp"
			"math.cpp"
			"mem.cpp"

			"${SRCDIR}/plugins/common/DataSerializer.cpp"
			"${SRCDIR}/plugins/common/vstdlib_static.cpp"
		#}
	)

	SRC_GRP(
		SUBGROUP "Map Objects"
		SOURCES
		#{
			"mapbrush.cpp"
			"mapcamera.cpp"
			"mapentity.cpp"
			"mapface.cpp"
			"mapgroup.cpp"
			"mappath.cpp"
			"mapworld.cpp"
		#}
	)

	SRC_GRP(
		SUBGROUP "Serializers"
		SOURCES
		#{
			"serializer_jmf.cpp"
			"serializer_map.cpp"
		#}
	)
END_SRC( JMFTOOLS_SOURCE_FILES "Source Files" )

set( JMFTOOLS_HEADER_FILES )
BEGIN_SRC( JMFTOOLS_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"logger.h"
			"math.h"
			"mem.h"

			"${SRCDIR}/plugins/common/DataSerializer.h"
			"${SRCDIR}/plugins/common/vstdlib_static.h"
		#}
	)

	SRC_GRP(
		SUBGROUP "Map Objects"
		SOURCES
		#{
			"mapbrush.h"
			"mapcamera.h"
			"mapentity.h"
			"mapface.h"
			"mapgroup.h"
			"mappath.h"
			"mapworld.h"
		#}
	)

	SRC_GRP(
		SUBGROUP "Serializers"
		SOURCES
		#{
			"serializer_jmf.h"
			"serializer_map.h"
		#}
	)
END_SRC( JMFTOOLS_HEADER_FILES "Header Files" )

add_executable(
	jmftools
	${JMFTOOLS_SOURCE_FILES} ${JMFTOOLS_HEADER_FILES}
)

install_library(
	TARGET jmftools EXECUTABLE
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_include_directories(
	jmftools PRIVATE

	"${SRCDIR}/plugins/common"
)

target_use_jackpluginsdk( jmftools "${SRCDIR}/sdk" )
