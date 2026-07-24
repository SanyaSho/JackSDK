//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// Versions: "J.A.C.K. 1.1.3773 Freeware"; "J.A.C.K. 1.1.3773"; "J.A.C.K. 1.2.4603"
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

#if !defined( PLUGINPATHS_H )
#define PLUGINPATHS_H

/*
 Paths API
*/

#include "BaseTypes.h"

class CMapPath;

typedef struct qPath_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapPath *m_mapPath;

	/* Next/Previous path list. First element never has ->prev set and last element never has ->next set */
	struct qPath_s *next;
	struct qPath_s *prev;

	/* List of nodes attached to the path */
	struct qNode_s *m_nodeList;
	struct qNode_s *m_lastNode;

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	char *m_pathName;
	char *m_pathClassname;
	int m_pathDirection;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	/* Internal path ID */
	int m_editorId;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
} qPath_t;
COMPILE_TIME_ASSERT( sizeof( qPath_t ) == SIZEOF_QPATH_S );

// clang-format off

// clang-format on

#endif // !PLUGINPATHS_H
