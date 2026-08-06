//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPENTITY_H )
#define MAPENTITY_H

#include <vector>

#define ENTITYAPI_NO_INLINEFUNCS
#include "PluginEntity.h"

#include "math.h"
#include "mem.h"

struct GDClass;
class CVisGroup; // mapworld.h

class CMapEntity
{
public:
	CMapEntity();
	virtual ~CMapEntity();

	static qEntity_t *create( const char *className, const vec3_t &origin, int editorFlags )
	{
		CMapEntity *mapEntity = new CMapEntity;
		mapEntity->m_entityData->m_editorId = 0;
		mapEntity->m_entityData->m_vecOrigin = origin;
		mapEntity->m_entityData->m_bboxMin = mapEntity->m_entityData->m_bboxMax = origin;
		mapEntity->m_entityData->m_className = Sys_AllocString( className );
		mapEntity->m_entityData->m_editorFlags = ( editorFlags & 0xFFFDFFFB ) | 4;
		return mapEntity->m_entityData;
	}

	static void clearEpairs( epair_s **epairList )
	{
		for ( epair_t *pair = *epairList, *next; pair != NULL; pair = next )
		{
			next = pair->next;

			Sys_Free( pair->key );
			Sys_Free( pair->value );

			Sys_Free( pair );
		}
		*epairList = NULL;
	}

public:
	void removeFromAllVisGroups();

protected:
	void reset();

	void clearEpairs() { clearEpairs( &m_entityData->epairs ); }

	void updateKeys( bool keepkeys );

	void freeUserData();

public:
	qEntity_s *m_entityData;
	GDClass *m_currentGDClass;
	int m_selectionId;
	rgba_t m_cbColor;
	void *m_vecData1;
	std::vector<CVisGroup *> m_visGroupList;
};

FORCEINLINE epair_t *AllocEpair( const char *key, const char *value )
{
	epair_t *pair = (epair_t *)Sys_Malloc( sizeof( epair_t ) );

	pair->next = NULL;
	pair->key = Sys_AllocString( key );
	pair->value = Sys_AllocString( value );

	return pair;
}

FORCEINLINE void AppendEpair( epair_t **list, const char *key, const char *value )
{
	while ( *list )
	{
		list = &( *list )->next;
	}

	*list = AllocEpair( key, value );
}

#endif // !MAPENTITY_H
