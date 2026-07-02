#-----------------------------------------------------------------------------
#	vpExamplePlugin
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

set( EXAMPLEPLUGIN_SOURCE_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"ExamplePlugin.cpp"
		#}
	)
END_SRC( EXAMPLEPLUGIN_SOURCE_FILES "Source Files" )

set( EXAMPLEPLUGIN_HEADER_FILES )
BEGIN_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
		#}
	)
END_SRC( EXAMPLEPLUGIN_HEADER_FILES "Header Files" )

set( SDK_HEADER_FILES )
BEGIN_SRC( SDK_HEADER_FILES "SDK Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"${SRCDIR}/sdk/public/BaseTypes.h"
			"${SRCDIR}/sdk/public/PluginAPI.h"
			"${SRCDIR}/sdk/public/PluginActions.h"
			"${SRCDIR}/sdk/public/PluginData.h"
			"${SRCDIR}/sdk/public/PluginEntity.h"
			"${SRCDIR}/sdk/public/PluginFace.h"
			"${SRCDIR}/sdk/public/PluginProfile.h"
		#}
	)
END_SRC( SDK_HEADER_FILES "SDK Header Files" )

add_library(
	exampleplugin MODULE
	${EXAMPLEPLUGIN_SOURCE_FILES} ${EXAMPLEPLUGIN_HEADER_FILES}
	${SDK_HEADER_FILES}
)

install_library(
	TARGET exampleplugin
	INSTALL_OUTNAME "vpExamplePlugin"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_compile_definitions(
	exampleplugin PRIVATE

	VPSDK
)

target_include_directories(
	exampleplugin PRIVATE

	"${SRCDIR}/sdk/public"
)
