//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( ITERATOR_H )
#define ITERATOR_H

// clang-format off

template<typename T>
struct Iterator
{
	T *m_current;

	T *operator*() const { return m_current; }

	Iterator &operator++() { m_current = m_current->next; return *this; }
	Iterator &operator--() { m_current = m_current->prev; return *this; }

	bool operator==( const Iterator &other ) const { return m_current == other.m_current; }
	bool operator!=( const Iterator &other ) const { return !(*this == other); }
};

template<typename T>
struct SelectedIterator
{
	T *m_current;

	T *operator*() const { return m_current; }

	SelectedIterator &operator++() { m_current = m_current->selectedNext; return *this; }
	SelectedIterator &operator--() { m_current = m_current->selectedPrev; return *this; }

	bool operator==( const SelectedIterator &other ) const { return m_current == other.m_current; }
	bool operator!=( const SelectedIterator &other ) const { return !(*this == other); }
};

// Example: EXPOSE_ITERATOR( worldDef, qEntity_s, worldDef.m_entityList, worldDef.m_lastEntity, "entities" );
#define EXPOSE_ITERATOR( class_, ownertype, type, field, lastfield, pyname ) class_.def( ##pyname, []( ownertype &class_ ) { return py::make_iterator( Iterator<type> { ##field }, Iterator<type> { ##lastfield } ); }, py::keep_alive<0, 1>() );

// Example: EXPOSE_SELECTEDITERATOR( worldDef, qEntity_s, worldDef.m_selEntityList, NULL, "selected_entities" );
#define EXPOSE_SELECTEDITERATOR( class_, ownertype, type, field, lastfield, pyname ) class_.def( ##pyname, []( ownertype &class_ ) { return py::make_iterator( SelectedIterator<type> { ##field }, SelectedIterator<type> { ##lastfield } ); }, py::keep_alive<0, 1>() );

// clang-format on

#endif // !ITERATOR_H