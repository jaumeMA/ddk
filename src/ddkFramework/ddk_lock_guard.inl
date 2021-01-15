
#include <thread>

namespace ddk
{
namespace detail
{

template<typename T>
struct lock_unlocker
{
public:
    inline bool unlock()
    {
        m_lock->unlock();
        return true;
    }

private:
    T* m_lock = nullptr;
};

}

template<typename T>
lockable_guard<T>::lockable_guard(T& i_lockableObject)
: m_lockableObject(i_lockableObject)
{
    m_lockableObject.lock();
}
template<typename T>
lockable_guard<T>::~lockable_guard()
{
    m_lockableObject.unlock();
}

template<typename T, typename ... TT>
lockable_guard<T,TT...>::lockable_guard(T& i_lockableObject, TT& ... i_lockableObjects)
: m_primaryLock(i_lockableObject)
{
   T* lockableObjects[s_numSecondaryLocks] = { &i_lockableObjects ...};

acquire_locks:

    m_primaryLock.lock();

    for(size_t index=0;index<s_numSecondaryLocks;++index)
    {
        if(lockableObjects[index]->try_lock() == false)
        {
            for(size_t iindex=index;iindex>0;iindex--)
            {
                lockableObjects[iindex-1]->unlock();
            }

            m_primaryLock.unlock();

            std::this_thread::yield();

            goto acquire_locks;
        }
    }

    memmove(m_secondaryLocks,lockableObjects,sizeof(void*) * s_numSecondaryLocks);
}
template<typename T, typename ... TT>
lockable_guard<T,TT...>::~lockable_guard()
{
    size_t index = s_numSecondaryLocks;
    bool _[] = { reinterpret_cast<detail::lock_unlocker<TT>*>(m_secondaryLocks[--index])->unlock() ... };

    m_primaryLock.unlock();
}

}
