//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINNODES_H )
#define PLUGINNODES_H

/*
 Nodes API
*/

#include "BaseTypes.h"

class CMapNode;

typedef struct qNode_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapNode *m_mapNode;

	/* Next/Previous node list. First element never has ->prev set and last element never has ->next set */
	struct qNode_s *next;
	struct qNode_s *prev;

	/* Next/Previous selected node list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qNode_s *selectedNext;
	struct qNode_s *selectedPrev;

	/* Owner of this node */
	struct qPath_s *m_ownerPath;

	/* Node KeyValues */
	struct epair_s *epairs;

	const char *m_nameOverride;
	const char *m_fireEntityOnPass;

	/* Used as a lock to tell renderer to not redraw the node */
	int render_unkint;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	/* Index of the node on a path */
	int m_nodeIndex;

	/* Internal node ID */
	int m_editorId;

	vec3_t m_vecOrigin;
	vec3_t m_vecAngles;

	/* New Train Speed */
	float m_speed;

	/* New Train rot. Speed */
	float m_yaw_speed;

	/* Wait here (secs) */
	float m_wait;

	float m_fov;

	/* Node spawnflags */
	int m_spawnflags;
} qNode_t;
COMPILE_TIME_ASSERT( sizeof( qNode_t ) == SIZEOF_QNODE_S );

// clang-format off

// clang-format on

#endif // !PLUGINNODES_H
