#include "any_value.h"

namespace ewas
{

any_value::any_value()
: m_currentAgnosticType(k_invalidType)
, m_arena(NULL)
{
}
any_value::~any_value()
{
	destroy();			
}
any_value::any_value(const any_value& other)
: m_currentAgnosticType(k_invalidType)
, m_arena(NULL)
{
	clone(other);
}
any_value& any_value::operator=(const any_value& other)
{
	destroy();

	clone(other);

	return *this;
}
any_value::operator bool() const
{
	return m_currentAgnosticType != k_invalidType;
}
size_t any_value::getCurrTypeId() const
{
	return m_currentAgnosticType;
}
void any_value::destroy()
{
	if(m_currentAgnosticType != k_invalidType)
	{
		const bool isTagged = m_arena.is_tagged();

		value_wrapper_interface* nestedPointer = m_arena.extract_pointer();

		if (isTagged)
		{
			nestedPointer->destroy();
		}
		else
		{
			delete nestedPointer;
		}

		m_currentAgnosticType = k_invalidType;
	}
}
void any_value::clone(const any_value& other)
{
	if(other.m_currentAgnosticType != k_invalidType)
	{
		const bool isTagged = m_arena.is_tagged();

		if(isTagged)
		{
			m_arena = tagged_pointer<value_wrapper_interface>(other.m_arena->clone(getArenaSize() >= other.getArenaSize() ? m_arena.get_pointer() : NULL),0x01);
		}
		else
		{
			m_arena = other.m_arena->clone();
		}
	}

	m_currentAgnosticType = other.m_currentAgnosticType;
}
size_t any_value::getArenaSize() const
{
	return (m_arena) ? m_arena->getSize() : 0;
}
void any_value::clear()
{
	destroy();
}

}