//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINENTITY_H )
#define PLUGINENTITY_H

/*
 Entity definition
*/

#include "BaseTypes.h"

struct qBrush_s;
struct qWorld_s;


#define ENT_BLDFLG_FULLBUILD			( 1 << 0  ) // Will append bits [4, 5, 6, 7, 9, 11, 12] to the flags
#define ENT_BLDFLG_BIT1					( 1 << 1  )
#define ENT_BLDFLG_BIT2					( 1 << 2  )
#define ENT_BLDFLG_BIT3					( 1 << 3  )
#define ENT_BLDFLG_BIT4					( 1 << 4  ) // 0x10
#define ENT_BLDFLG_PARSE_RENDERMODE		( 1 << 5  )
#define ENT_BLDFLG_PARSE_ENTITYSTATE	( 1 << 6  )
#define ENT_BLDFLG_UPDATE_SKY_PARAMS	( 1 << 7  ) // Will work only if entity's editorFlags has 0x20
#define ENT_BLDFLG_REBUILD_USERDATA		( 1 << 8  )
#define ENT_BLDFLG_UPDATE_CLASSINFO		( 1 << 9  ) // Will update the entity class info (only if FULLBUILD is not set) and will link the targets
#define ENT_BLDFLG_BIT10				( 1 << 10 )
#define ENT_BLDFLG_BIT11				( 1 << 11 )
#define ENT_BLDFLG_BIT12				( 1 << 12 )
#define ENT_BLDFLG_BIT13				( 1 << 13 )


typedef struct epair_s
{
	struct epair_s *next;
	char *key;
	char *value;
} epair_t;
COMPILE_TIME_ASSERT( sizeof( epair_t ) == 24 );

inline epair_t *AllocEpair( const char *key, const char *value )
{
	epair_t *pair = (epair_t *)Sys_Malloc( sizeof( epair_t ) );

	pair->next = NULL;
	pair->key = Sys_AllocString( key );
	pair->value = Sys_AllocString( value );

	return pair;
}

inline void FreeEpairList( epair_t *list )
{
	for ( epair_t *pair = list, *next; pair != NULL; pair = next )
	{
		next = pair->next;

		if ( pair->key )
			Sys_Free( pair->key );
		if ( pair->value )
			Sys_Free( pair->value );

		Sys_Free( pair );
	}
}


typedef struct qSplineNode_s
{
	struct qSplineNode_s *nextNode;
	char gap[8];
	struct qEntity_s *owner;
	char gap2[108];
	float speed;
	float yaw_speed;
	float fov;
} qSplineNode_t;
COMPILE_TIME_ASSERT( sizeof( qSplineNode_t ) == 144 );


typedef struct qEntityKeys_s
{
	const char *key;
} qEntityKeys_t;


/* This struct is written as a 16 byte blob to the JMF */
typedef struct qEntityRenderMode_s
{
	/* Editor-specific rendering flags */
	/*
	 Normal			- 256		0b100000000				[8]
	 Color/Texture	- 262245	0b1000000000001100101	[0, 2, 5, 6, 18]
	 Glow/Additive	- 37		0b100101				[0, 2, 5]
	 Solid			- 262501	0b111101101010111001	[0, 3, 4, 5, 7, 9, 11, 12, 14, 15, 16, 17]
	*/
	int m_renderFlags;

	/* Render color */
	rgba_t m_renderColor;

	/* Render mode (affects m_renderFlags above) */
	int m_renderMode;

	/* Render effect */
	int m_renderFx;
} qEntityRenderMode_t;
COMPILE_TIME_ASSERT( sizeof( qEntityRenderMode_t ) == 16 );


/* This struct is written as a 48 (previously 16) byte blob to the JMF */
typedef struct qEntityState_s
{
	/* Body */
	short m_body;

	/* Skin / Contents */
	short m_skin;

	/* Editor-only animation sequence */
	int m_sequence;

	/* Framerate (ex. env_sprite) */
	float m_framerate;

	/* Scale (ex. env_sprite) */
	float m_scale;

	/* Since JMF 114 */
	float m_radius;

	/* Reserved space for alignment */
	int __pad[7];
} qEntityState_t;
COMPILE_TIME_ASSERT( sizeof( qEntityState_t ) == 48 );


/*typedef struct targetInfo_s
{
	char *a;
	char *b;
} targetInfo_t;
COMPILE_TIME_ASSERT( sizeof( targetInfo_t ) == 16 );*/

typedef enum modtype_s
{
	mod_bad = 0, // maybe mod_brush
	mod_sprite,
	mod_decal,
	mod_studio,
	mod_particles,
	mod_unknown
} modtype_t;

class CMapEntity;

typedef struct qEntity_s
{
	void *firstPtr;

	CMapEntity *pMapEntity;

	struct qEntity_s *next;
	struct qEntity_s *prev;

	char gap2_1[24];

	void *unk_groups;

	struct qBrush_s *m_brushList;

	struct qBrush_s *m_lastBrush;

	struct qWorld_s *m_world;

	struct epair_s *epairs;

	struct qSplineNode_s *splineNode;

	char *m_className;

	char *m_targetName;

	char gap4[8];

	int m_spawnflags;

	int m_editorFlags;

	char gap5[4];

	int m_modelType; // modtype_t

	vec3_t m_vecOrigin;

	vec3_t m_vecAngles;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	struct qEntityRenderMode_s m_renderMode;

	struct qEntityState_s m_entityState;

	struct qEntityKeys_s m_entityKeys[13];

	void *m_drawData; // This pointer size depends on the m_modelType value. See CMapEntity::rebuildUserData.
} qEntity_t;
COMPILE_TIME_ASSERT( sizeof( qEntity_t ) == 368 );

inline epair_s *AddEpair( qEntity_s *entity, const char *key, const char *value )
{
	if ( !entity )
		return nullptr;

	epair_s *last = nullptr;

	for ( epair_s *epair = entity->epairs; epair; epair = epair->next )
	{
		if ( !stricmp( epair->key, key ) )
		{
			Sys_Free( epair->value ); // if needed
			epair->value = Sys_AllocString( value );
			return epair;
		}

		last = epair;
	}

	epair_s *epair = AllocEpair( key, value );

	if ( last )
		last->next = epair;
	else
		entity->epairs = epair;

	return epair;
}

// clang-format off

// clang-format on

#endif // !PLUGINENTITY_H
