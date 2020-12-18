#pragma once

#include "ddk_atomics.h"
#include "ddk_macros.h"
#include "ddk_scoped_enum.h"
#include <map>
#include <vector>
#include "ddk_mutex.h"

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
	static const size_t k_maxNumberOfStacks = 16;
	static const size_t k_maxNumOfChars = 64;
	typedef void* stack_entry[k_maxNumberOfStacks];
	typedef std::unordered_map<size_t, stack_entry> stack_container;
#endif

public:
	typedef std::map<size_t,std::vector<std::string>> stack_contents;

	lent_reference_counter();
	lent_reference_counter(const lent_reference_counter& other);
	lent_reference_counter(lent_reference_counter&& other);
	virtual ~lent_reference_counter() = default;
#ifdef DDK_DEBUG
	size_t incrementLentReference();
	size_t decrementLentReference();
	size_t getNumLentReferences() const;
	bool hasLentReferences() const;
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
	mutex m_refMutex;
	atomic_size_t m_numLentReferences;
#endif
};

class shared_reference_counter: public lent_reference_counter
{
public:
	shared_reference_counter();
	shared_reference_counter(const shared_reference_counter& other);
	shared_reference_counter(shared_reference_counter&& other);
	size_t incrementSharedReference();
	bool incrementSharedReferenceIfNonEmpty();
	size_t decrementSharedReference();
	size_t getNumSharedReferences() const;
	bool hasSharedReferences() const;
	size_t incrementWeakReference();
	size_t decrementWeakReference();
	size_t getNumWeakReferences() const;
	bool hasWeakReferences() const;

private:
	atomic_size_t m_numSharedReferences;
	atomic_size_t m_numWeakReferences;
};

//reference counting for unique references

class unique_reference_counter : public lent_reference_counter
{
public:
	unique_reference_counter();
	unique_reference_counter(const unique_reference_counter& other);
	unique_reference_counter(unique_reference_counter&& other);
	bool addStrongReference();
	bool removeStrongReference();
	bool hasStrongReferences() const;

private:
	bool m_hasStrongReferences;
};

}
