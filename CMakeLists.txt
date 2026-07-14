cmake_minimum_required( VERSION 3.20 FATAL_ERROR )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

project( JackSDK CXX C )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set( SRCDIR "${CMAKE_CURRENT_LIST_DIR}" )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set( IS_32BIT 0 )
	set( IS_64BIT 1 )
else()
	set( IS_32BIT 1 )
	set( IS_64BIT 0 )
endif()

include( "cmake/srcgrp.cmake" )

include( "cmake/simpleinstall.cmake" )

add_subdirectory( "plugins/vpExamplePlugin" )
add_subdirectory( "utils/pluginrunner" )
