cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

if( UNIX )
	set( CMAKE_SKIP_BUILD_RPATH FALSE )
	set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )
	set( CMAKE_INSTALL_RPATH "$\{ORIGIN\}" )
endif()

project( JackSDK CXX C )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set( SRCDIR "${CMAKE_CURRENT_LIST_DIR}" )

# 10 - Freeware
# 100 - Steam Public
# 121 - Steam Beta
option( OVERRIDE_API_VERSION "Change defininon of JACK_API_VERSION" -1 )
if( NOT ${OVERRIDE_API_VERSION} EQUAL -1 )
	add_compile_definitions( JACK_API_VERSION=${OVERRIDE_API_VERSION} )
endif()

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set( IS_32BIT 0 )
	set( IS_64BIT 1 )
else()
	set( IS_32BIT 1 )
	set( IS_64BIT 0 )
endif()

if( NOT WIN32 )
	add_compile_options( -ggdb )
endif()

include( "cmake/srcgrp.cmake" )

include( "cmake/simpleinstall.cmake" )

add_subdirectory( "plugins/vpExamplePlugin" )
add_subdirectory( "plugins/vpGLView" )
add_subdirectory( "plugins/vpHalfLifeAlpha" )
add_subdirectory( "plugins/vpPythonHost" )

add_subdirectory( "utils/pluginrunner" )
