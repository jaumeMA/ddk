#include "ddk_reference_counter.h"
#include "ddk_lock_guard.h"

#include <sstream>

#ifdef DDK_DEBUG

#if defined(WIN32)

#include <dbghelp.h>

#elif defined(__LINUX__)

#include <execinfo.h>

#endif

#endif

namespace ddk
{

#ifdef DDK_DEBUG

//#define TRACK_STACK

detail::symbol_cache_table lent_reference_counter::m_symbolInfoCache = detail::symbol_cache_table();


lent_reference_counter::lent_reference_counter()
: m_numLentReferences(0)
{
}
lent_reference_counter::lent_reference_counter(const lent_reference_counter& other)
: m_numLentReferences(other.m_numLentReferences)
{
}
lent_reference_counter::lent_reference_counter(lent_reference_counter&& other)
: m_numLentReferences(0)
{
	m_numLentReferences.set(other.m_numLentReferences.get());
	other.m_numLentReferences.set(0);
#ifdef TRACK_STACK
	std::swap(m_stackTraces,other.m_stackTraces);
#endif
}
unsigned int lent_reference_counter::incrementLentReference()
{
	return atomic_post_increment(m_numLentReferences);
}
unsigned int lent_reference_counter::decrementLentReference()
{
	return atomic_post_decrement(m_numLentReferences);
}
unsigned int lent_reference_counter::getNumLentReferences() const
{
	return m_numLentReferences.get();
}
bool lent_reference_counter::hasLentReferences() const
{
	return m_numLentReferences.get() > 0;
}
void lent_reference_counter::registerStackTrace(size_t i_id)
{
#ifdef TRACK_STACK
	typedef detail::symbol_cache_table::symbol_info symbol_info;

	void* stack[k_maxNumberOfStacks] = { NULL };

#ifdef WIN32
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	WORD numberOfFrames = CaptureStackBackTrace(3, k_maxNumberOfStacks, stack, NULL);

#elif defined (__LINUX__)
	int numberOfFrames = backtrace(stack,k_maxNumberOfStacks);
#else
#error "Pending to be implemented for this platform"
#endif

	mutex_guard lg(m_refMutex);

	stack_entry& currStackTrace = m_stackTraces[i_id];

	for (int i = 0; i < k_maxNumberOfStacks; i++)
	{
		currStackTrace[i] = (i < numberOfFrames) ? stack[i] : nullptr;
	}

#endif
}
void lent_reference_counter::unregisterStackTrace(size_t i_id)
{
#ifdef TRACK_STACK
	mutex_guard lg(m_refMutex);

	stack_container::iterator itStackTrace = m_stackTraces.find(i_id);
	if (itStackTrace != m_stackTraces.end())
	{
		m_stackTraces.erase(itStackTrace);
	}
#endif
}
void lent_reference_counter::copyStackTrace(size_t i_oldId, size_t i_newId)
{
#ifdef TRACK_STACK
	mutex_guard lg(m_refMutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if (itOldStackTrace != m_stackTraces.end())
	{
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
	}
#endif
}
void lent_reference_counter::reassignStackTrace(size_t i_oldId, size_t i_newId)
{
#ifdef TRACK_STACK
	mutex_guard lg(m_refMutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if (itOldStackTrace != m_stackTraces.end())
	{
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
		m_stackTraces.erase(itOldStackTrace);
	}
#endif
}
lent_reference_counter::stack_contents lent_reference_counter::dumpStackTrace()
{
	stack_contents res;

	mutex_guard lg(m_refMutex);

	stack_container::iterator itStackTrace = m_stackTraces.begin();
	for(;itStackTrace!=m_stackTraces.end();++itStackTrace)
	{
		stack_entry& currStack = itStackTrace->second;

		for (size_t i = 0; i < k_maxNumberOfStacks; i++)
		{
			if(void* currStackSymbol = currStack[i])
			{
				const detail::symbol_cache_table::symbol_info& symbolicatedSymbol = m_symbolInfoCache.symbolicate(currStackSymbol);

				//std stringstream is just too slow
				std::string resume = symbolicatedSymbol.m_filename + "(" + std::to_string(symbolicatedSymbol.m_lineNumber) + ") : " + symbolicatedSymbol.m_name;
				resume.resize(k_maxNumOfChars);

				res[itStackTrace->first].emplace_back(std::move(resume));
			}
		}
	}

	return res;
}

#endif

weak_reference_counter::weak_reference_counter()
: m_numWeakReferences(0)
{
}
weak_reference_counter::weak_reference_counter(const weak_reference_counter& other)
: m_numWeakReferences(other.m_numWeakReferences)
{
}
weak_reference_counter::weak_reference_counter(weak_reference_counter&& other)
: m_numWeakReferences(0)
{
	std::swap(m_numWeakReferences,other.m_numWeakReferences);
}
unsigned int weak_reference_counter::incrementWeakReference()
{
	return atomic_post_increment(m_numWeakReferences);
}
unsigned int weak_reference_counter::decrementWeakReference()
{
	return atomic_post_decrement(m_numWeakReferences);
}
unsigned int weak_reference_counter::getNumWeakReferences() const
{
	return m_numWeakReferences.get();
}
bool weak_reference_counter::hasWeakReferences() const
{
	return m_numWeakReferences.get() > 0;
}

distributed_reference_counter::distributed_reference_counter()
: m_numSharedReferences(0)
{
}
distributed_reference_counter::distributed_reference_counter(const distributed_reference_counter& other)
: m_numSharedReferences(other.m_numSharedReferences)
{
}
distributed_reference_counter::distributed_reference_counter(distributed_reference_counter&& other)
: m_numSharedReferences(0)
{
	std::swap(m_numSharedReferences,other.m_numSharedReferences);
}
unsigned int distributed_reference_counter::incrementSharedReference()
{
	return atomic_post_increment(m_numSharedReferences);
}
unsigned int distributed_reference_counter::decrementSharedReference()
{
	return atomic_post_decrement(m_numSharedReferences);
}
unsigned int distributed_reference_counter::getNumSharedReferences() const
{
	return m_numSharedReferences.get();
}
bool distributed_reference_counter::hasSharedReferences() const
{
	return m_numSharedReferences.get() > 0;
}
bool distributed_reference_counter::hasWeakReferences() const
{
	return false;
}
bool distributed_reference_counter::incrementSharedReferenceIfNonEmpty()
{
	unsigned int oldValue = 0;

	do
	{
		oldValue = m_numSharedReferences.get();

		if(oldValue == 0)
		{
			return false;
		}
	} while(atomic_compare_exchange(m_numSharedReferences,oldValue,oldValue + 1));

	return true;
}

unsigned int shared_reference_counter::incrementSharedReference()
{
	weak_reference_counter::incrementWeakReference();

	return distributed_reference_counter::incrementSharedReference();
}
bool shared_reference_counter::incrementSharedReferenceIfNonEmpty()
{
	if(distributed_reference_counter::incrementSharedReferenceIfNonEmpty())
	{
		weak_reference_counter::incrementWeakReference();

		return true;
	}

	return false;
}
unsigned int shared_reference_counter::decrementSharedReference()
{
	weak_reference_counter::decrementWeakReference();

	return distributed_reference_counter::decrementSharedReference();
}
bool shared_reference_counter::hasWeakReferences() const
{
	return weak_reference_counter::hasWeakReferences();
}

unique_reference_counter::unique_reference_counter()
: m_hasStrongReferences(false)
{
}
unique_reference_counter::unique_reference_counter(const unique_reference_counter& other)
#ifdef DDK_DEBUG
: lent_reference_counter(other)
, m_hasStrongReferences(other.m_hasStrongReferences)
#else
: m_hasStrongReferences(other.m_hasStrongReferences)
#endif
{
}
unique_reference_counter::unique_reference_counter(unique_reference_counter&& other)
#ifdef DDK_DEBUG
: lent_reference_counter(std::move(other))
,m_hasStrongReferences(false)
#else
: m_hasStrongReferences(false)
#endif
{
	std::swap(m_hasStrongReferences,other.m_hasStrongReferences);
}
bool unique_reference_counter::addStrongReference()
{
	DDK_ASSERT(m_hasStrongReferences == false,"Unique reference already incremented");

	m_hasStrongReferences = true;

	return m_hasStrongReferences;
}
bool unique_reference_counter::removeStrongReference()
{
	DDK_ASSERT(m_hasStrongReferences == true,"Trying to decrement empty reference counter");

	m_hasStrongReferences = false;

	return m_hasStrongReferences;
}
bool unique_reference_counter::hasStrongReferences() const
{
	return m_hasStrongReferences;
}

}
