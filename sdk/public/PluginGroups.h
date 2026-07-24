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

#if !defined( PLUGINGROUPS_H )
#define PLUGINGROUPS_H

/*
 Groups API

 Groups are stored as a linked list inside of qWorld_s and can be iterated using ->next and ->prev pointers.
*/

#include "BaseTypes.h"

struct qWorld_s;


class CMapGroup;

typedef struct qGroup_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapGroup *m_mapGroup;

	/* Next/Previous group list. First element never has ->prev set and last element never has ->next set */
	struct qGroup_s *next;
	struct qGroup_s *prev;

	/* Next/Previous selected group list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qGroup_s *selectedNext;
	struct qGroup_s *selectedPrev;

	struct qGroup_s *m_parent; // TODO: Recheck

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	/* Internal group ID */
	int m_editorId;

	/* Number of objects in this group */
	size_t m_objectCount;
} qGroup_t;
COMPILE_TIME_ASSERT( sizeof( qGroup_t ) == SIZEOF_QGROUPS_S );

// clang-format off

// clang-format on

#endif // !PLUGINGROUPS_H
