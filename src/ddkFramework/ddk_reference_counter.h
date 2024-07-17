//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_atomics.h"
#include "ddk_macros.h"
#include "ddk_scoped_enum.h"
#include "ddk_mutex.h"
#include "ddk_compressed_pair.h"
#include <map>
#include <vector>


#ifdef DDK_DEBUG

//#define TRACK_STACK

#include "ddk_memory_tracker.h"

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
public:
	typedef std::map<size_t,std::vector<std::string>> stack_contents;

	lent_reference_counter();
	lent_reference_counter(const lent_reference_counter& other);
	lent_reference_counter(lent_reference_counter&& other);
	virtual ~lent_reference_counter() = default;

	lent_reference_counter& operator=(const lent_reference_counter& other);
	lent_reference_counter& operator=(lent_reference_counter&& other);
	unsigned int incrementLentReference();
	unsigned int decrementLentReference();
	unsigned int getNumLentReferences() const;
	bool hasLentReferences() const;

#ifdef TRACK_STACK
	void registerStackTrace(size_t i_id);
	void unregisterStackTrace(size_t i_id);
	void copyStackTrace(size_t i_oldId, size_t i_newId);
	void reassignStackTrace(size_t i_oldId, size_t i_newId);
#endif

private:
#if defined(TRACK_STACK)
	memory_tracker m_memTracker;
#endif
	atomic_uint m_numLentReferences;
};

#endif

class weak_reference_counter
{
public:

	unsigned int incrementWeakReference();
	unsigned int decrementWeakReference();
	unsigned int getNumWeakReferences() const;
	bool hasWeakReferences() const;

protected:
	weak_reference_counter();
	weak_reference_counter(const weak_reference_counter& other);
	weak_reference_counter(weak_reference_counter&& other);

private:
	atomic_uint m_numWeakReferences;
};

class NO_VTABLE distributed_reference_counter
#ifdef DDK_DEBUG
: public lent_reference_counter
#endif
{
public:
	unsigned int incrementSharedReference();
	bool incrementSharedReferenceIfNonEmpty();
	unsigned int decrementSharedReference();
	unsigned int getNumSharedReferences() const;
	bool hasSharedReferences() const;
	bool hasWeakReferences() const;
	virtual void destroy_shared_resource(short i_tagCategory) = 0;

protected:
	distributed_reference_counter();
	distributed_reference_counter(const distributed_reference_counter& other);
	distributed_reference_counter(distributed_reference_counter&& other);

	distributed_reference_counter& operator=(const distributed_reference_counter& other);
	distributed_reference_counter& operator=(distributed_reference_counter&& other);

private:
	atomic_uint m_numSharedReferences;
};

class shared_reference_counter: public distributed_reference_counter, public weak_reference_counter
{
public:
	unsigned int incrementSharedReference();
	bool incrementSharedReferenceIfNonEmpty();
	unsigned int decrementSharedReference();
	bool hasWeakReferences() const;

protected:
	shared_reference_counter() = default;
	shared_reference_counter(const shared_reference_counter& other) = default;
	shared_reference_counter(shared_reference_counter&& other) = default;
};

template<typename T,typename Deleter,typename ReferenceCounter>
class share_control_block : public ReferenceCounter
{
public:
	typedef ReferenceCounter reference_counter;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Deleter,Args...))
	share_control_block(T* i_ptr, Args&& ... i_args);

protected:
	void destroy_shared_resource(short i_tagCategory) override;

private:
	detail::compressed_pair<T*,Deleter> m_data;
};
template<typename T,typename Deleter>
using distributed_control_block = share_control_block<T,Deleter,distributed_reference_counter>;
template<typename T,typename Deleter>
using shared_control_block = share_control_block<T,Deleter,shared_reference_counter>;

//reference counting for unique references

class NO_VTABLE unique_reference_counter
#ifdef DDK_DEBUG
: public lent_reference_counter
#endif
{
public:
	bool addStrongReference();
	bool removeStrongReference();
	bool hasStrongReferences() const;
	virtual void destroy_unique_resource(short i_tagCategory) = 0;

protected:
	unique_reference_counter();
	unique_reference_counter(const unique_reference_counter& other);
	unique_reference_counter(unique_reference_counter&& other);

private:
	bool m_hasStrongReferences;
};

template<typename T, typename Deleter>
class unique_control_block : public unique_reference_counter
{
public:
	typedef unique_reference_counter reference_counter;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Deleter,Args...))
	unique_control_block(T* i_ptr, Args&& ... i_args);

private:
	void destroy_unique_resource(short i_tagCategory) override;

	detail::compressed_pair<T*,Deleter> m_data;
};

}

#include "ddk_reference_counter.inl"
