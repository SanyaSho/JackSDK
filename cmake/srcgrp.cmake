#-----------------------------------------------------------------------------
# SRCGRP.CMAKE
#
# SanyaSho (2024)
#-----------------------------------------------------------------------------

include_guard( GLOBAL )

# Utility function used by srcgrp.cmake and install_lib.cmake (NOTE: You can't use generator complex expressions (like $<AND:1,1>) in ifelse)
function( strip_generator_expressions input expression )
	if( ${input} MATCHES "\\$<(\\$<.*>):(.*)>$|\\$<([0-9]):(.*)>$" ) # Dual regex cuz idk how to combine it in one. First part is for large expressions, second for small.
		# CMAKE_MATCH_1 - Expression for CMAKE_MATCH_2
		# CMAKE_MATCH_2 - If "$<$<AND:$<OR:1,0>,1>:C:/proj/main.cpp>" or "$<$<AND:$<OR:1,0>,1>:/home/user/proj/main.cpp>"
		# CMAKE_MATCH_3 - Expression for CMAKE_MATCH_4
		# CMAKE_MATCH_4 - If "$<1:C:/proj/main.cpp>" or "$<1:/home/user/proj/main.cpp>"

		if( NOT "${CMAKE_MATCH_2}" STREQUAL "" AND "${CMAKE_MATCH_4}" STREQUAL "" )
			#message( STATUS "Using match2 ${CMAKE_MATCH_2} expr match1 ${CMAKE_MATCH_1}" )
			set( ${expression} ${CMAKE_MATCH_1} PARENT_SCOPE )
			set( ${input} "${CMAKE_MATCH_2}" PARENT_SCOPE )
		elseif( "${CMAKE_MATCH_2}" STREQUAL "" AND NOT "${CMAKE_MATCH_4}" STREQUAL "" )
			#message( STATUS "Using match4 ${CMAKE_MATCH_4} expr match3 ${CMAKE_MATCH_3}" )
			set( ${expression} ${CMAKE_MATCH_3} PARENT_SCOPE )
			set( ${input} "${CMAKE_MATCH_4}" PARENT_SCOPE )
		else()
			message( FATAL_ERROR "Failed to parse input!" )
		endif()
	else()
		set( ${expression} 1 PARENT_SCOPE ) # make expression check always valid if input is not captured by regex
	endif()
endfunction()

macro( BEGIN_SRC TARGET_SRC GROUP_NAME )
	set( _CURRENT_SRC	${TARGET_SRC} )
	set( _CURRENT_GROUP	${GROUP_NAME} )
endmacro()

macro( SRC_GRP )
	cmake_parse_arguments(
		GRP_ARGS
		""
		"TARGET_SRC;SUBGROUP"
		"SOURCES;NO_PCH"
		${ARGN}
	)

	if( NOT GRP_ARGS_TARGET_SRC )
		set( GRP_ARGS_TARGET_SRC "${_CURRENT_SRC}" )
	endif()

	if( NOT GRP_ARGS_SUBGROUP )
		set( GRP_ARGS_SUBGROUP "" )
	endif()

	if( NOT GRP_ARGS_SOURCES )
		message( "WARNING: No SOURCES set for SRC_GRP ${GRP_ARGS_TARGET_SRC}" )
	endif()

	#message( STATUS "GROUP: ${_CURRENT_GROUP}//${GRP_ARGS_SUBGROUP}" )

	foreach( FILE IN LISTS GRP_ARGS_SOURCES )
		#message( STATUS "\tSOURCES: ${FILE}" )

		set( GROUP_NAME "${_CURRENT_GROUP}//${GRP_ARGS_SUBGROUP}" )
		list( APPEND ${GRP_ARGS_TARGET_SRC} ${FILE} )

		set( FILE_EXPRESSION )
		strip_generator_expressions( FILE FILE_EXPRESSION )

		source_group( ${GROUP_NAME} FILES "${FILE}" )
	endforeach()

	foreach( FILE IN LISTS GRP_ARGS_NO_PCH )
		#message( STATUS "\tNO_PCH: ${FILE}" )
		set_source_files_properties( ${FILE} PROPERTIES SKIP_PRECOMPILE_HEADERS ON )
	endforeach()

	set( ${GRP_ARGS_TARGET_SRC} ${${GRP_ARGS_TARGET_SRC}} PARENT_SCOPE )
endmacro()

macro( END_SRC TARGET_SRC GROUP_NAME )
	unset( _CURRENT_SRC )
	unset( _CURRENT_GROUP )
endmacro()
