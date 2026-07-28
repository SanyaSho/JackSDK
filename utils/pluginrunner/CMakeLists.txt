#-----------------------------------------------------------------------------
#	pluginrunner
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( PLUGINRUNNER_SOURCE_FILES )
BEGIN_SRC( PLUGINRUNNER_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"PluginRunner.cpp"
		#}
	)
END_SRC( PLUGINRUNNER_SOURCE_FILES "Source Files" )

set( PLUGINRUNNER_HEADER_FILES )
BEGIN_SRC( PLUGINRUNNER_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
		#}
	)
END_SRC( PLUGINRUNNER_HEADER_FILES "Header Files" )

add_executable(
	PluginRunner WIN32
	${PLUGINRUNNER_SOURCE_FILES} ${PLUGINRUNNER_HEADER_FILES}
)

install_library(
	TARGET PluginRunner EXECUTABLE
	INSTALL_OUTNAME "PluginRunner"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_use_jackpluginsdk( PluginRunner "${SRCDIR}/sdk" )
