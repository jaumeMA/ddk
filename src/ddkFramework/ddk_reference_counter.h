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

#ifdef DDK_DEBUG

class lent_reference_counter
{
	static const size_t k_maxNumberOfStacks = 16;
	static const size_t k_maxNumOfChars = 64;
	typedef void* stack_entry[k_maxNumberOfStacks];
	typedef std::unordered_map<size_t, stack_entry> stack_container;

public:
	typedef std::map<size_t,std::vector<std::string>> stack_contents;

	lent_reference_counter();
	lent_reference_counter(const lent_reference_counter& other);
	lent_reference_counter(lent_reference_counter&& other);
	virtual ~lent_reference_counter() = default;

	unsigned int incrementLentReference();
	unsigned int decrementLentReference();
	unsigned int getNumLentReferences() const;
	bool hasLentReferences() const;
	void registerStackTrace(size_t i_id);
	void unregisterStackTrace(size_t i_id);
	void copyStackTrace(size_t i_oldId, size_t i_newId);
	void reassignStackTrace(size_t i_oldId, size_t i_newId);
	stack_contents dumpStackTrace();

private:
	static detail::symbol_cache_table m_symbolInfoCache;
	stack_container m_stackTraces;
	mutex m_refMutex;
	atomic_uint m_numLentReferences;
};

#endif

class weak_reference_counter
{
public:
	weak_reference_counter();
	weak_reference_counter(const weak_reference_counter& other);
	weak_reference_counter(weak_reference_counter&& other);

	unsigned int incrementWeakReference();
	unsigned int decrementWeakReference();
	unsigned int getNumWeakReferences() const;
	bool hasWeakReferences() const;

private:
	atomic_uint m_numWeakReferences;
};

class distributed_reference_counter 
#ifdef DDK_DEBUG
: public lent_reference_counter
#endif
{
public:
	distributed_reference_counter();
	distributed_reference_counter(const distributed_reference_counter& other);
	distributed_reference_counter(distributed_reference_counter&& other);
	unsigned int incrementSharedReference();
	bool incrementSharedReferenceIfNonEmpty();
	unsigned int decrementSharedReference();
	unsigned int getNumSharedReferences() const;
	bool hasSharedReferences() const;
	bool hasWeakReferences() const;

private:
	atomic_uint m_numSharedReferences;
};

class shared_reference_counter: public distributed_reference_counter, public weak_reference_counter
{
public:
	shared_reference_counter() = default;
	shared_reference_counter(const shared_reference_counter& other) = default;
	shared_reference_counter(shared_reference_counter&& other) = default;
	unsigned int incrementSharedReference();
	bool incrementSharedReferenceIfNonEmpty();
	unsigned int decrementSharedReference();
	bool hasWeakReferences() const;
};

//reference counting for unique references

class unique_reference_counter
#ifdef DDK_DEBUG
: public lent_reference_counter
#endif
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
