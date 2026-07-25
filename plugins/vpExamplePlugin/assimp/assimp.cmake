#=============================================================================
# assimp.cmake
#
# $
#=============================================================================

include_guard( GLOBAL )

function( target_use_assimp target assimpdir )
	set( ASSIMP_DIR "${assimpdir}" )

	set( ASSIMP_HEADER_FILES )
	BEGIN_SRC( ASSIMP_HEADER_FILES "Assimp Header Files" )
		SRC_GRP(
			SOURCES
			#{
				"${ASSIMP_DIR}/include/assimp/AssertHandler.h"
				"${ASSIMP_DIR}/include/assimp/Base64.hpp"
				"${ASSIMP_DIR}/include/assimp/BaseImporter.h"
				"${ASSIMP_DIR}/include/assimp/Bitmap.h"
				"${ASSIMP_DIR}/include/assimp/BlobIOSystem.h"
				"${ASSIMP_DIR}/include/assimp/ByteSwapper.h"
				"${ASSIMP_DIR}/include/assimp/ColladaMetaData.h"
				"${ASSIMP_DIR}/include/assimp/CreateAnimMesh.h"
				"${ASSIMP_DIR}/include/assimp/DefaultIOStream.h"
				"${ASSIMP_DIR}/include/assimp/DefaultIOSystem.h"
				"${ASSIMP_DIR}/include/assimp/DefaultLogger.hpp"
				"${ASSIMP_DIR}/include/assimp/Exceptional.h"
				"${ASSIMP_DIR}/include/assimp/Exporter.hpp"
				"${ASSIMP_DIR}/include/assimp/GenericProperty.h"
				"${ASSIMP_DIR}/include/assimp/GltfMaterial.h"
				"${ASSIMP_DIR}/include/assimp/Hash.h"
				"${ASSIMP_DIR}/include/assimp/IOStream.hpp"
				"${ASSIMP_DIR}/include/assimp/IOStreamBuffer.h"
				"${ASSIMP_DIR}/include/assimp/IOSystem.hpp"
				"${ASSIMP_DIR}/include/assimp/Importer.hpp"
				"${ASSIMP_DIR}/include/assimp/LineSplitter.h"
				"${ASSIMP_DIR}/include/assimp/LogAux.h"
				"${ASSIMP_DIR}/include/assimp/LogStream.hpp"
				"${ASSIMP_DIR}/include/assimp/Logger.hpp"
				"${ASSIMP_DIR}/include/assimp/MathFunctions.h"
				"${ASSIMP_DIR}/include/assimp/MemoryIOWrapper.h"
				"${ASSIMP_DIR}/include/assimp/NullLogger.hpp"
				"${ASSIMP_DIR}/include/assimp/ObjMaterial.h"
				"${ASSIMP_DIR}/include/assimp/ParsingUtils.h"
				"${ASSIMP_DIR}/include/assimp/Profiler.h"
				"${ASSIMP_DIR}/include/assimp/ProgressHandler.hpp"
				"${ASSIMP_DIR}/include/assimp/RemoveComments.h"
				"${ASSIMP_DIR}/include/assimp/SGSpatialSort.h"
				"${ASSIMP_DIR}/include/assimp/SceneCombiner.h"
				"${ASSIMP_DIR}/include/assimp/SkeletonMeshBuilder.h"
				"${ASSIMP_DIR}/include/assimp/SmallVector.h"
				"${ASSIMP_DIR}/include/assimp/SmoothingGroups.h"
				"${ASSIMP_DIR}/include/assimp/SmoothingGroups.inl"
				"${ASSIMP_DIR}/include/assimp/SpatialSort.h"
				"${ASSIMP_DIR}/include/assimp/StandardShapes.h"
				"${ASSIMP_DIR}/include/assimp/StreamReader.h"
				"${ASSIMP_DIR}/include/assimp/StreamWriter.h"
				"${ASSIMP_DIR}/include/assimp/StringComparison.h"
				"${ASSIMP_DIR}/include/assimp/StringUtils.h"
				"${ASSIMP_DIR}/include/assimp/Subdivision.h"
				"${ASSIMP_DIR}/include/assimp/TinyFormatter.h"
				"${ASSIMP_DIR}/include/assimp/Vertex.h"
				"${ASSIMP_DIR}/include/assimp/XMLTools.h"
				"${ASSIMP_DIR}/include/assimp/XmlParser.h"
				"${ASSIMP_DIR}/include/assimp/ZipArchiveIOSystem.h"
				"${ASSIMP_DIR}/include/assimp/aabb.h"
				"${ASSIMP_DIR}/include/assimp/ai_assert.h"
				"${ASSIMP_DIR}/include/assimp/anim.h"
				"${ASSIMP_DIR}/include/assimp/camera.h"
				"${ASSIMP_DIR}/include/assimp/cexport.h"
				"${ASSIMP_DIR}/include/assimp/cfileio.h"
				"${ASSIMP_DIR}/include/assimp/cimport.h"
				"${ASSIMP_DIR}/include/assimp/color4.h"
				"${ASSIMP_DIR}/include/assimp/color4.inl"
				"${ASSIMP_DIR}/include/assimp/commonMetaData.h"
				"${ASSIMP_DIR}/include/assimp/config.h"
				"${ASSIMP_DIR}/include/assimp/defs.h"
				"${ASSIMP_DIR}/include/assimp/fast_atof.h"
				"${ASSIMP_DIR}/include/assimp/importerdesc.h"
				"${ASSIMP_DIR}/include/assimp/light.h"
				"${ASSIMP_DIR}/include/assimp/material.h"
				"${ASSIMP_DIR}/include/assimp/material.inl"
				"${ASSIMP_DIR}/include/assimp/matrix3x3.h"
				"${ASSIMP_DIR}/include/assimp/matrix3x3.inl"
				"${ASSIMP_DIR}/include/assimp/matrix4x4.h"
				"${ASSIMP_DIR}/include/assimp/matrix4x4.inl"
				"${ASSIMP_DIR}/include/assimp/mesh.h"
				"${ASSIMP_DIR}/include/assimp/metadata.h"
				"${ASSIMP_DIR}/include/assimp/module.modulemap"
				"${ASSIMP_DIR}/include/assimp/pbrmaterial.h"
				"${ASSIMP_DIR}/include/assimp/postprocess.h"
				"${ASSIMP_DIR}/include/assimp/qnan.h"
				"${ASSIMP_DIR}/include/assimp/quaternion.h"
				"${ASSIMP_DIR}/include/assimp/quaternion.inl"
				"${ASSIMP_DIR}/include/assimp/revision.h"
				"${ASSIMP_DIR}/include/assimp/scene.h"
				"${ASSIMP_DIR}/include/assimp/texture.h"
				"${ASSIMP_DIR}/include/assimp/types.h"
				"${ASSIMP_DIR}/include/assimp/vector2.h"
				"${ASSIMP_DIR}/include/assimp/vector2.inl"
				"${ASSIMP_DIR}/include/assimp/vector3.h"
				"${ASSIMP_DIR}/include/assimp/vector3.inl"
				"${ASSIMP_DIR}/include/assimp/version.h"
			#}
		)

		SRC_GRP(
			SUBGROUP "Compiler"
			SOURCES
			#{
				"${ASSIMP_DIR}/include/assimp/Compiler/poppack1.h"
				"${ASSIMP_DIR}/include/assimp/Compiler/pstdint.h"
				"${ASSIMP_DIR}/include/assimp/Compiler/pushpack1.h"
			#}
		)

		SRC_GRP(
			SUBGROUP "Port"
			SOURCES
			#{
				"${ASSIMP_DIR}/include/assimp/port/AndroidJNI/AndroidJNIIOSystem.h"
				"${ASSIMP_DIR}/include/assimp/port/AndroidJNI/BundledAssetIOSystem.h"
			#}
		)
	END_SRC( ASSIMP_HEADER_FILES "Assimp Header Files" )

	target_sources(
		${target} PRIVATE

		${ASSIMP_HEADER_FILES}
	)

	target_include_directories(
		${target} PRIVATE

		"${ASSIMP_DIR}/include"
	)

	target_link_directories(
		${target} PRIVATE

		"$<${IS_32BIT}:${ASSIMP_DIR}/lib/x86>"
		"$<${IS_64BIT}:${ASSIMP_DIR}/lib/x64>"
	)

	if( WIN32 )
		target_link_libraries(
			${target} PRIVATE

			assimp-vc145-mt
		)

		if ( ${IS_32BIT} )
			install(
				FILES "${ASSIMP_DIR}/bin/x86/assimp-vc145-mt.dll"
				DESTINATION ${CMAKE_INSTALL_PREFIX}
				OPTIONAL
			)
		else()
			install(
				FILES "${ASSIMP_DIR}/bin/x64/assimp-vc145-mt.dll"
				DESTINATION ${CMAKE_INSTALL_PREFIX}
				OPTIONAL
			)
		endif()
	else()
		target_link_libraries(
			${target} PRIVATE

			assimp
		)
	endif()

endfunction()

