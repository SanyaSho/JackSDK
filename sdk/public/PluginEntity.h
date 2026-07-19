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


#define ENT_BLDFLG_FULLBUILD				( 1 << 0  ) // Will append bits [4, 5, 6, 7, 9, 11, 12] to the flags
#define ENT_BLDFLG_BIT1						( 1 << 1  )
#define ENT_BLDFLG_BRUSH_FACESNADOVERLAYS	( 1 << 2  ) // Will build Faces and Overlays for brush inside an entity
#define ENT_BLDFLG_BIT3						( 1 << 3  )
#define ENT_BLDFLG_BIT4						( 1 << 4  ) // 0x10
#define ENT_BLDFLG_PARSE_RENDERMODE			( 1 << 5  )
#define ENT_BLDFLG_PARSE_ENTITYSTATE		( 1 << 6  )
#define ENT_BLDFLG_UPDATE_SKY_PARAMS		( 1 << 7  ) // Will work only if entity's editorFlags has 0x20
#define ENT_BLDFLG_REBUILD_USERDATA			( 1 << 8  )
#define ENT_BLDFLG_UPDATE_CLASSINFO			( 1 << 9  ) // Will update the entity class info (only if FULLBUILD is not set) and will link the targets
#define ENT_BLDFLG_BRUSH_DONTSORTFACES		( 1 << 10 ) // If not set, will sort the faces that brush has
#define ENT_BLDFLG_PATCH_CALCANDOPTIMIZE	( 1 << 11 ) // Will calculate the points and optimize patch applied to brush
#define ENT_BLDFLG_BIT12					( 1 << 12 )
#define ENT_BLDFLG_BIT13					( 1 << 13 )
#define ENT_BLDFLG_BRUSH_UPDATESHADER		( 1 << 14 ) // Will rebuild the shader used by patches and overlays


typedef struct epair_s
{
	struct epair_s *next;
	char *key;
	char *value;
} epair_t;
COMPILE_TIME_ASSERT( sizeof( epair_t ) == SIZEOF_EPAIR_S );

FORCEINLINE epair_t *AllocEpair( const char *key, const char *value )
{
	epair_t *pair = (epair_t *)Sys_Malloc( sizeof( epair_t ) );

	pair->next = NULL;
	pair->key = Sys_AllocString( key );
	pair->value = Sys_AllocString( value );

	return pair;
}

FORCEINLINE void FreeEpairList( epair_t *list )
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
#if defined( JACK_64BIT )
	char gap[8];
#else
	char gap[4];
#endif // JACK_64BIT
	struct qEntity_s *owner;
	char gap2[24];
	vec3_t m_vecForward;
	vec3_t m_vecRight;
	vec3_t m_vecUp;
	char gap3[48];
	float speed;
	float yaw_speed;
	float fov;
} qSplineNode_t;
COMPILE_TIME_ASSERT( sizeof( qSplineNode_t ) == SIZEOF_QSPLINENODE_S );


typedef struct qEntityKeys_s
{
	const char *key;
} qEntityKeys_t;


/* This struct is written as a 16 byte blob to the JMF */
typedef struct qEntityRenderMode_s
{
	/* Editor-specific rendering flags */
	/*
	 Normal			- GLS_DEPTHWRITE
	 Color/Texture	- GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_ATEST_ALWAYS
	 Glow/Additive	- GLS_DSTBLEND_ONE | GLS_SRCBLEND_SRC_ALPHA
	 Solid			- GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_DEPTHWRITE | GLS_ATEST_ALWAYS
	*/
	int m_renderFlags;

	/* Render color */
	rgba_t m_renderColor;

	/* Render mode (affects m_renderFlags above) */
	int m_renderMode;

	/* Render effect */
	int m_renderFx;
} qEntityRenderMode_t;
COMPILE_TIME_ASSERT( sizeof( qEntityRenderMode_t ) == SIZEOF_QENTITYRENDERMODE_S /* Always 16, serialized */ );


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
COMPILE_TIME_ASSERT( sizeof( qEntityState_t ) == SIZEOF_QENTITYSTATE_S /* Always 48, serialized */ );


/*typedef struct targetInfo_s
{
	char *a;
	char *b;
} targetInfo_t;
COMPILE_TIME_ASSERT( sizeof( targetInfo_t ) == 16 );*/


typedef enum modtype_s
{
	mod_brush = 0,
	mod_sprite,
	mod_decal,
	mod_studio,
	mod_particles,
	mod_unknown
} modtype_t;

class CMapEntity;

typedef struct qEntity_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapEntity *m_mapEntity;

	/* Next/Previous entity list. First element never has ->prev set and last element never has ->next set */
	struct qEntity_s *next;
	struct qEntity_s *prev;

	/* Next/Previous selected entity list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qEntity_s *selectedNext;
	struct qEntity_s *selectedPrev;

	/* List of groups this entity belongs to */
	struct qGroup_s *m_groupList;
	struct qGroup_s *m_lastGroup;

	/* List of brushes this entity has attached to it */
	struct qBrush_s *m_brushList;
	struct qBrush_s *m_lastBrush;

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	/* Entity KeyValues */
	struct epair_s *epairs;

	struct qSplineNode_s *m_splineNode;

	/* Entity ClassName */
	char *m_className;

	/* Entity TargetName */
	char *m_targetName;

	/* Used as a lock to tell renderer to not redraw the entity */
	int render_unkint;

	/* Used as a g_chromeage hash by StudioRender */
	int render_chromeage_unkint; // g_smodels_total?

	/* Entity SpawnFlags */
	int m_spawnflags;

	int m_editorFlags;

	/* Internal entity ID */
	int m_editorId;

	modtype_t m_modelType;

	vec3_t m_vecOrigin;
	vec3_t m_vecAngles;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	struct qEntityRenderMode_s m_renderMode;

	struct qEntityState_s m_entityState;

	struct qEntityKeys_s m_entityKeys[13];

	void *m_drawData; // This pointer size depends on the m_modelType value. See CMapEntity::rebuildUserData.
} qEntity_t;
COMPILE_TIME_ASSERT( sizeof( qEntity_t ) == SIZEOF_QENTITY_S );

FORCEINLINE epair_s *AddEpair( qEntity_s *entity, const char *key, const char *value )
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
