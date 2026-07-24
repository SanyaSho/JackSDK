//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Copyright (C) 2026  SanyaSho
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <https://www.gnu.org/licenses/>.
//
//=====================================================================================//

#if !defined( PLUGINFACES_H )
#define PLUGINFACES_H

/*
 Faces API
*/

#include "BaseTypes.h"

struct qShader_s;


/* This struct is written as a 20 byte blob into the JMF */
typedef struct qPlane_s
{
	vec3_t normal;
	float dist; // DotProduct of normal
	int alignedAxis;
} qPlane_t;
COMPILE_TIME_ASSERT( sizeof( qPlane_t ) == SIZEOF_QPLANE_S /* Always 20 */ );

FORCEINLINE void PlaneFromPoints( const vec3_t &p0, const vec3_t &p1, const vec3_t &p2, struct qPlane_s &planeOut )
{
	//CrossProduct( p2 - p1, p0 - p1, planeOut.normal ); // winbspc
	CrossProduct( p0 - p1, p2 - p0, planeOut.normal );
	VectorNormalize( planeOut.normal );

	planeOut.dist = DotProduct( planeOut.normal, p0 );

	if ( fabs( planeOut.normal.x ) == 1.f )
	{
		planeOut.alignedAxis = 0;
	}
	else if ( fabs( planeOut.normal.y ) == 1.f )
	{
		planeOut.alignedAxis = 1;
	}
	else if ( fabs( planeOut.normal.z ) == 1.f )
	{
		planeOut.alignedAxis = 2;
	}
	else
	{
		planeOut.alignedAxis = 3;
	}
}


#define TEXALIGN_NONE			(      0 )
#define TEXALIGN_WORLD			( 1 << 0 )
#define TEXALIGN_FACE			( 1 << 1 )
#define TEXALIGN_QUAKE			( 1 << 2 )

/* This struct is written as a 128 byte blob to the JMF */
typedef struct qTexDef_s
{
	vec3_t m_UAxis;
	float m_xShift;

	vec3_t m_VAxis;
	float m_yShift;

	vec2_t m_scale;

	float m_rotate;

	/* See TEXALIGN_ defines above */
	int m_textureAlignment;

	int m_value; // Quake II "Value"
	int m_lightmapScale; // HL2 addition
	int m_surfaceFlags; // Quake II SurfaceFlags
	int m_contentFlags; // Quake II ContentFlags

	char m_textureName[64];
} qTexDef_t;
COMPILE_TIME_ASSERT( sizeof( qTexDef_t ) == SIZEOF_QTEXDEF_S /* Always 128 */ );


typedef struct qVertex_s
{
	vec3_t coords;
	vec3_t uv;

	// There's no proofs that selectionState is the actual field and I couldn't find where it can be used (except for being copied as a part of qVertex_s)
	//vec2_t uv;
	//int selectionState;
} qVertex_t;
COMPILE_TIME_ASSERT( sizeof( qVertex_t ) == SIZEOF_QVERTEX_S /* Always 24 */ );


class CMapFace;

typedef struct qFace_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapFace *m_mapFace;

	/* Next/Previous face list. First element never has ->prev set and last element never has ->next set */
	struct qFace_s *next;
	struct qFace_s *prev;

	/* Next/Previous selected face list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qFace_s *selectedNext;
	struct qFace_s *selectedPrev;

	/* A pointer? */
#if defined( JACK_64BIT )
	char gap1[8];
#else
	char gap1[4];
#endif // JACK_64BIT

	/* Brush this face belongs to */
	struct qBrush_s *m_ownerBrush;

	struct qShader_s *m_shaderInfo;

	char gap2[4];

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	/* Internal face ID */
	int m_editorId;

	struct qPlane_s m_plane;

	struct qTexDef_s m_texDef;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	int m_vertexCount;
	struct qVertex_s *m_vertices;
} qFace_t;
COMPILE_TIME_ASSERT( sizeof( qFace_t ) == SIZEOF_QFACE_S );

// clang-format off

// clang-format on

#endif // !PLUGINFACES_H
