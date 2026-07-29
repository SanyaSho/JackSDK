#-----------------------------------------------------------------------------
#	vpPythonHost
#
#	Project Script
#-----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

if( NOT EXISTS "pybind11" )
	message( FATAL_ERROR "pybind11 was not found. Get it from https://github.com/pybind/pybind11/releases/tag/v3.0.4" )
endif()

add_subdirectory( "pybind11" )

include( "${SRCDIR}/sdk/JackPluginSDK.cmake" )

set( PYTHONHOST_SOURCE_FILES )
BEGIN_SRC( PYTHONHOST_SOURCE_FILES "Source Files" )
	SRC_GRP(
		SOURCES
		#{
			"main.cpp"
			"pythonhost.cpp"
			#"../common/vstdlib_static.cpp"
		#}
	)

	SRC_GRP(
		SUBGROUP "Bindings"
		SOURCES
		#{
			"bindings/BaseTypes.cpp"
			"bindings/BaseTypes.pyi"
			"bindings/PluginActions.cpp"
			"bindings/PluginActions.pyi"
			"bindings/PluginCamera.cpp"
			"bindings/PluginCamera.pyi"
			"bindings/PluginAPI.cpp"
			"bindings/PluginAPI.pyi"
			"bindings/PluginEntity.cpp"
			"bindings/PluginEntity.pyi"
			"bindings/PluginFaces.cpp"
			"bindings/PluginFaces.pyi"
			"bindings/PluginGroups.cpp"
			"bindings/PluginGroups.pyi"
			"bindings/PluginMath.cpp"
			"bindings/PluginMath.pyi"
			"bindings/PluginNodes.cpp"
			"bindings/PluginNodes.pyi"
			"bindings/PluginPaths.cpp"
			"bindings/PluginPaths.pyi"
			"bindings/PluginWorld.cpp"
			"bindings/PluginWorld.pyi"
		#}
	)
END_SRC( PYTHONHOST_SOURCE_FILES "Source Files" )

set( PYTHONHOST_HEADER_FILES )
BEGIN_SRC( PYTHONHOST_HEADER_FILES "Header Files" )
	SRC_GRP(
		SOURCES
		#{
			"iterator.h"
			"pythonhost.h"
			#"../common/vstdlib_static.h"
		#}
	)
END_SRC( PYTHONHOST_HEADER_FILES "Header Files" )

add_library(
	pythonhost MODULE
	${PYTHONHOST_SOURCE_FILES} ${PYTHONHOST_HEADER_FILES}
)

install_library(
	TARGET pythonhost
	INSTALL_OUTNAME "vpPythonHost"
	INSTALL_DEST "${CMAKE_INSTALL_PREFIX}"
)

target_include_directories(
	pythonhost PRIVATE

	"."
	"../common"
)

target_link_libraries(
	pythonhost PRIVATE

	pybind11::module pybind11::embed
)

target_use_jackpluginsdk( pythonhost "${SRCDIR}/sdk" )
