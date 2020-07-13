#pragma once

#include "ddk_atomics.h"
#include "ddk_macros.h"
#include "ddk_scoped_enum.h"
#include <map>
#include <vector>

#ifdef DDK_DEBUG

#include "ddk_symbol_cache_table.h"

#endif

namespace ddk
{

SCOPED_ENUM_DECL(Policy,
	Shared,
	Unique
);

SCOPED_ENUM_DECL(ReferenceAllocationType,
	Dynamic = 0,
	Contiguous,
	Embedded
);

class lent_reference_counter
{
#ifdef DDK_DEBUG
	static const size_t k_maxNumberOfStacks = 8;
	static const size_t k_maxNumOfChars = 64;
	typedef void* stack_entry[k_maxNumberOfStacks];
	typedef std::unordered_map<size_t, stack_entry> stack_container;
#endif

public:
	typedef std::map<size_t,std::vector<std::string>> stack_contents;

	lent_reference_counter();
	lent_reference_counter(const lent_reference_counter& other);
	lent_reference_counter(lent_reference_counter&& other);
	virtual ~lent_reference_counter();
#ifdef DDK_DEBUG
	size_t incrementWeakReference();
	size_t decrementWeakReference();
	size_t getNumWeakReferences() const;
	bool hasWeakReferences() const;
	void registerStackTrace(size_t i_id);
	void unregisterStackTrace(size_t i_id);
	void copyStackTrace(size_t i_oldId, size_t i_newId);
	void reassignStackTrace(size_t i_oldId, size_t i_newId);
	stack_contents dumpStackTrace();
#endif

private:
#ifdef DDK_DEBUG
	static detail::symbol_cache_table m_symbolInfoCache;
	stack_container m_stackTraces;
	pthread_mutex_t m_refMutex;
	atomic_size_t m_numWeakReferences;
#endif
};

template<int>
class reference_counter;

//reference counting for shared references
template<>
class reference_counter<Policy::Shared> : public lent_reference_counter
{
public:
	reference_counter()
	: lent_reference_counter()
	, m_numSharedReferences(0)
	{
	}
	reference_counter(const reference_counter& other)
	: lent_reference_counter(other)
	, m_numSharedReferences(other.m_numSharedReferences)
	{
	}
	reference_counter(reference_counter&& other)
	: lent_reference_counter(std::move(other))
	, m_numSharedReferences(0)
	{
		std::swap(m_numSharedReferences, other.m_numSharedReferences);
	}
	~reference_counter()
	{
	}
	size_t incrementSharedReference()
	{
		return atomic_post_increment(m_numSharedReferences);
	}
	size_t decrementSharedReference()
	{
		return atomic_post_decrement(m_numSharedReferences);
	}
	size_t getNumSharedReferences() const
	{
		return m_numSharedReferences.get();
	}
	bool hasSharedReferences() const
	{
		return m_numSharedReferences.get() > 0;
	}

private:
	atomic_size_t m_numSharedReferences;
};

//reference counting for unique references

template<>
class reference_counter<Policy::Unique> : public lent_reference_counter
{
public:
	reference_counter()
	: lent_reference_counter()
	, m_hasStrongReferences(false)
	{
	}
	reference_counter(const reference_counter& other)
	: lent_reference_counter(other)
	, m_hasStrongReferences(other.m_hasStrongReferences)
	{
	}
	reference_counter(reference_counter&& other)
	: lent_reference_counter(std::move(other))
	, m_hasStrongReferences(false)
	{
		std::swap(m_hasStrongReferences, other.m_hasStrongReferences);
	}
	bool addStrongReference()
	{
		DDK_ASSERT(m_hasStrongReferences == false, "Unique reference already incremented");

		m_hasStrongReferences = true;

		return m_hasStrongReferences;
	}
	bool removeStrongReference()
	{
		DDK_ASSERT(m_hasStrongReferences == true, "Unique reference already incremented");

		m_hasStrongReferences = false;

		return m_hasStrongReferences;
	}
	bool hasStrongReferences() const
	{
		return m_hasStrongReferences;
	}

private:
	bool m_hasStrongReferences;
};

typedef reference_counter<Policy::Shared> shared_reference_counter;
typedef reference_counter<Policy::Unique> unique_reference_counter;

}
