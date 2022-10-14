
#include "ddk_lock_concepts.h"
#include "ddk_concepts.h"
#include <thread>

namespace ddk
{
namespace detail
{

TEMPLATE(typename T)
REQUIRES(IS_LOCKABLE(T))
inline bool unlock_lock(T* i_lock)
{
    if(i_lock)
    {
        i_lock->unlock();

        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
lock_guard_impl<mpl::sequence<>,T>::lock_guard_impl(T& i_lockableObject)
: m_lockableObject(i_lockableObject)
{
    m_lockableObject.lock();
}
template<typename T>
lock_guard_impl<mpl::sequence<>,T>::~lock_guard_impl()
{
    m_lockableObject.unlock();
}

template<size_t ... Indexs,typename T, typename ... TT>
lock_guard_impl<mpl::sequence<Indexs...>,T,TT...>::lock_guard_impl(T& i_lockableObject, TT& ... i_lockableObjects)
: m_primaryLock(i_lockableObject)
{
    static const size_t s_numBytes = (s_numSecondaryLocks / 8) + 1;
    static const unsigned int s_allIn = (1 << s_numSecondaryLocks) - 1;

    const void* _[] = { ( m_secondaryLocks[Indexs] = &i_lockableObjects ) ... };

acquire_locks:

    m_primaryLock.lock();

    const unsigned int tryLockRes = ((static_cast<unsigned int>(reinterpret_cast<TT*>(m_secondaryLocks[Indexs])->try_lock()) << Indexs) | ...);

    if(memcmp(&tryLockRes,&s_allIn,s_numBytes) != 0)
    {
        const bool _[] = { static_cast<bool>(tryLockRes & (1 << Indexs)) && unlock_lock(reinterpret_cast<TT*>(m_secondaryLocks[Indexs])) ... };

        m_primaryLock.unlock();

        std::this_thread::yield();

        goto acquire_locks;
    }
}
template<size_t ... Indexs,typename T, typename ... TT>
lock_guard_impl<mpl::sequence<Indexs...>,T,TT...>::~lock_guard_impl()
{
    const bool _[] = { unlock_lock(reinterpret_cast<TT*>(m_secondaryLocks[Indexs])) ... };

    m_primaryLock.unlock();
}

}
}
