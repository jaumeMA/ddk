#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_pointer_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_shared_from_this.h"
#include "ddk_distribute_from_this.h"
#include "ddk_lendable.h"
#include "ddk_atomic_shared_pointer_wrapper.h"
#include "ddk_atomic_shared_reference_wrapper.h"
#include "ddk_atomic_weak_pointer_wrapper.h"
#include "ddk_smart_ptr_template_helper.h"

namespace ddk
{

namespace detail
{

template<typename T>
unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	return unique_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter);
}
template<typename T>
unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	return unique_pointer_wrapper<T>(i_data,std::move(i_refCounter),i_refDeleter);
}
template<typename T>
unique_reference_wrapper<T> __make_unique_reference(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	return unique_reference_wrapper<T>(i_data,i_refCounter,i_refDeleter);
}
template<typename T>
unique_reference_wrapper<T> __make_unique_reference(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	return unique_reference_wrapper<T>(i_data,std::move(i_refCounter),i_refDeleter);
}

#ifdef DDK_DEBUG

template<typename T>
lent_reference_wrapper<T> __make_lent_reference(T* i_data,const tagged_pointer<lent_reference_counter>& i_refCounter)
{
	return lent_reference_wrapper<T>(i_data,i_refCounter);
}
template<typename T>
lent_pointer_wrapper<T> __make_lent_pointer(T* i_data,const tagged_pointer<lent_reference_counter>& i_refCounter)
{
	return lent_pointer_wrapper<T>(i_data,i_refCounter);
}

#else

template<typename T>
inline lent_reference_wrapper<T> __make_lent_reference(T* i_data)
{
	return lent_reference_wrapper<T>(i_data);
}
template<typename T>
inline lent_pointer_wrapper<T> __make_lent_pointer(T* i_data)
{
	return lent_pointer_wrapper<T>(i_data);
}

#endif

template<typename T>
shared_reference_wrapper_impl<T,true> __make_shared_reference(T* i_data,const tagged_pointer<shared_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	if constexpr(mpl::contains_symbol___shared_type_tag<T>::value)
	{
		i_data->set_reference_counter(i_refCounter);
		i_data->set_deleter(i_refDeleter);
	}

	return shared_reference_wrapper_impl<T,true>(i_data,i_refCounter,i_refDeleter);
}
template<typename T>
shared_reference_wrapper_impl<T,false> __make_shared_reference(T* i_data,const tagged_pointer<distributed_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	if constexpr(mpl::contains_symbol___distributed_type_tag<T>::value)
	{
		i_data->set_deleter(i_refDeleter);
	}

	return shared_reference_wrapper_impl<T,false>(i_data,i_refCounter,i_refDeleter);
}
template<typename T>
ddk::weak_pointer_wrapper<T> __make_weak_pointer(T* i_data,const tagged_pointer<shared_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
{
	return weak_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter);
}
template<typename T>
ddk::weak_pointer_wrapper<T> __weak(shared_pointer_wrapper_impl<T,true>& i_sharedPtr)
{
	return __make_weak_pointer(i_sharedPtr.m_data,i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename T, bool Weakable>
ddk::weak_pointer_wrapper<const T> __weak(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return __make_weak_pointer(i_sharedPtr.m_data,i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename TT,typename T,bool Weakable>
shared_pointer_wrapper_impl<TT,Weakable> __static_shared_cast(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return shared_pointer_wrapper_impl<TT,Weakable>(static_cast<TT*>(i_sharedRef.m_data),i_sharedRef.m_refCounter,i_sharedRef.m_deleter);
}
template<typename TT,typename T,bool Weakable>
shared_reference_wrapper_impl<TT,Weakable> __static_shared_cast(const shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return shared_reference_wrapper_impl<TT,Weakable>(static_cast<TT*>(i_sharedRef.m_data),i_sharedRef.m_refCounter,i_sharedRef.m_deleter);
}
template<typename TT,typename T,bool Weakable>
shared_pointer_wrapper_impl<TT,Weakable> __dynamic_shared_cast(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedRef)
{
	if(TT* tData = dynamic_cast<TT*>(const_cast<T*>(i_sharedRef.m_data)))
	{
		return shared_pointer_wrapper_impl<TT,Weakable>(tData,i_sharedRef.m_refCounter,i_sharedRef.m_deleter);
	}
	else
	{
		return nullptr;
	}
}
template<typename TT,typename T,bool Weakable>
shared_pointer_wrapper_impl<TT,Weakable> __reinterpret_shared_cast(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return shared_pointer_wrapper_impl<TT,Weakable>(reinterpret_cast<TT*>(const_cast<T*>(i_sharedPtr.m_data)),i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename TT,typename T,bool Weakable>
shared_reference_wrapper_impl<TT,Weakable> __reinterpret_shared_cast(const shared_reference_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return shared_reference_wrapper_impl<TT,Weakable>(reinterpret_cast<TT*>(const_cast<T*>(i_sharedPtr.m_data)),i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable> __const_shared_cast(const shared_reference_wrapper_impl<const T,Weakable>& i_sharedRef)
{
	return detail::shared_pointer_wrapper_impl<T,Weakable>(const_cast<T*>(i_sharedRef.m_data),i_sharedRef.m_refCounter,i_sharedRef.m_deleter);
}
template<typename T,bool Weakable>
shared_pointer_wrapper_impl<T,Weakable> __const_shared_cast(const shared_pointer_wrapper_impl<const T,Weakable>& i_sharedPtr)
{
	return shared_pointer_wrapper_impl<T,Weakable>(const_cast<T*>(i_sharedPtr.m_data),i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable> __promote_to_ref(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return shared_reference_wrapper_impl<T,Weakable>(i_sharedPtr.m_data,i_sharedPtr.m_refCounter,i_sharedPtr.m_deleter);
}
template<typename T,bool Weakable>
lent_pointer_wrapper<T> __lend(shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	#ifdef DDK_DEBUG
	return __make_lent_pointer(i_sharedPtr.m_data,i_sharedPtr.m_refCounter);
	#else
	return i_sharedPtr.m_data;
	#endif
}
template<typename T,bool Weakable>
lent_pointer_wrapper<T> __lend(const shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	#ifdef DDK_DEBUG
	return __make_lent_pointer(i_sharedPtr.m_data,i_sharedPtr.m_refCounter);
	#else
	return i_sharedPtr.m_data;
	#endif
}
template<typename T,bool Weakable>
lent_reference_wrapper<T> __lend(shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	#ifdef DDK_DEBUG
	return __make_lent_reference(i_sharedRef.m_data,i_sharedRef.m_refCounter);
	#else
	return i_sharedRef.m_data;
	#endif
}
template<typename T,bool Weakable>
lent_reference_wrapper<T> __lend(const shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	#ifdef DDK_DEBUG
	return __make_lent_reference(i_sharedRef.m_data,i_sharedRef.m_refCounter);
	#else
	return i_sharedRef.m_data;
	#endif
}

}

template<typename T, typename ... Args>
unique_reference_wrapper<T> make_unique_reference(Args&& ... i_args)
{
	typedef typename unique_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T) + sizeof(unique_reference_counter)));

	T* allocatedObject = new (allocatedMemory) T(std::forward<Args>(i_args) ...);

	unique_reference_counter* refCounter = new (allocatedMemory + sizeof(T)) unique_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter, ReferenceAllocationType::Contiguous);

	return detail::__make_unique_reference(allocatedObject, taggedRefCounter, NULL);
}

template<typename T>
unique_reference_wrapper<T> as_unique_reference(T* i_ptr, const IReferenceWrapperDeleter& i_refDeleter)
{
	typedef typename unique_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	DDK_ASSERT(i_ptr!=nullptr, "Trying to contruct unique reference from null pointer");

	unique_reference_counter* refCounter = new unique_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter, ReferenceAllocationType::Dynamic);

	return detail::__make_unique_reference(i_ptr, taggedRefCounter, &i_refDeleter);
}

template<typename T>
unique_reference_wrapper<T> as_unique_reference(T* i_ptr)
{
	typedef typename unique_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	DDK_ASSERT(i_ptr!=nullptr, "Trying to contruct unique reference from null pointer");

	unique_reference_counter* refCounter = new unique_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter, ReferenceAllocationType::Dynamic);

	return detail::__make_unique_reference(i_ptr, taggedRefCounter, nullptr);
}

template<typename T>
unique_reference_wrapper<T> as_unique_reference(T* i_ptr, const tagged_pointer<unique_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr)
{
	DDK_ASSERT(i_ptr!=nullptr, "Trying to contruct unique reference from null pointer");

	return detail::__make_unique_reference(i_ptr, i_refCounter,i_refDeleter);
}

template<typename T, typename ... Args>
shared_reference_wrapper<T> make_shared_reference(Args&& ... i_args)
{
	typedef typename shared_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T)));

	T* allocatedObject = new (allocatedMemory) T(std::forward<Args>(i_args) ...);

	// In the case of shared pointers we cannot group memory allocation into a single malloc since we have weak deps
	allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(shared_reference_counter)));

	shared_reference_counter* refCounter = new (allocatedMemory) shared_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter,ReferenceAllocationType::Dynamic);

	return detail::__make_shared_reference(allocatedObject,taggedRefCounter,nullptr);
}
template<typename T,typename ... Args>
distributed_reference_wrapper<T> make_distributed_reference(Args&& ... i_args)
{
	typedef typename distributed_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T) + sizeof(distributed_reference_counter)));

	T* allocatedObject = new (allocatedMemory) T(std::forward<Args>(i_args) ...);

	distributed_reference_counter* refCounter = new (allocatedMemory + sizeof(T)) distributed_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter,ReferenceAllocationType::Contiguous);

	return detail::__make_shared_reference(allocatedObject,taggedRefCounter,nullptr);
}

template<typename T>
shared_reference_wrapper<T> as_shared_reference(T* i_ptr)
{
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;

	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	shared_reference_counter* refCounter = new shared_reference_counter();

	return detail::__make_shared_reference(i_ptr,tagged_reference_counter{refCounter},nullptr);
}
template<typename T>
shared_reference_wrapper<T> as_shared_reference(T* i_ptr, const IReferenceWrapperDeleter* i_refDeleter)
{
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;

	DDK_ASSERT(i_ptr!=nullptr, "Trying to contruct shared reference from null pointer");

	shared_reference_counter* refCounter = new shared_reference_counter();

	return detail::__make_shared_reference(i_ptr,tagged_reference_counter{refCounter},i_refDeleter);
}
template<typename T>
shared_reference_wrapper<T> as_shared_reference(T* i_ptr,const tagged_pointer<shared_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter = nullptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");
	DDK_ASSERT(i_refCounter.get_tag() == ReferenceAllocationType::Dynamic,"Trying to contruct shared reference non dynamic reference counter");

	return detail::__make_shared_reference(i_ptr,i_refCounter,i_refDeleter);
}

template<typename T>
inline distributed_reference_wrapper<T> as_distributed_reference(T* i_ptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	if constexpr(mpl::contains_symbol___distributed_type_tag<T>::value)
	{
		return detail::__make_shared_reference(i_ptr,i_ptr->get_reference_counter(),nullptr);
	}
	else
	{
		distributed_reference_counter* refCounter = new distributed_reference_counter();

		return detail::__make_shared_reference(i_ptr,refCounter,nullptr);
	}
}
template<typename T>
inline distributed_reference_wrapper<T> as_distributed_reference(T* i_ptr,const IReferenceWrapperDeleter* i_refDeleter)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	if constexpr(mpl::contains_symbol___distributed_type_tag<T>::value)
	{
		return detail::__make_shared_reference(i_ptr,i_ptr->get_reference_counter(),i_refDeleter);
	}
	else
	{
		distributed_reference_counter* refCounter = new distributed_reference_counter();

		return detail::__make_shared_reference(i_ptr,refCounter,i_refDeleter);
	}
}
template<typename T>
distributed_reference_wrapper<T> as_distributed_reference(T* i_ptr, const tagged_pointer<distributed_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr)
{
	DDK_ASSERT(i_ptr!=nullptr, "Trying to contruct shared reference from null pointer");

	return detail::__make_shared_reference(i_ptr, i_refCounter,i_refDeleter);
}

template<typename T,typename ... Args>
atomic_shared_reference_wrapper<T> make_atomic_shared_reference(Args&& ... i_args)
{
	typedef typename shared_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T)));

	T* allocatedObject = new (allocatedMemory) T(std::forward<Args>(i_args) ...);

	// In the case of shared pointers we cannot group memory allocation into a single malloc since we have weak deps
	allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(shared_reference_counter)));

	shared_reference_counter* refCounter = new (allocatedMemory) shared_reference_counter();

	tagged_reference_counter taggedRefCounter(refCounter,ReferenceAllocationType::Dynamic);

	return detail::__make_shared_reference(allocatedObject,taggedRefCounter,nullptr);
}
template<typename T,typename ... Args>
atomic_distributed_reference_wrapper<T> make_atomic_distributed_reference(Args&& ... i_args)
{
	typedef typename distributed_reference_wrapper<T>::tagged_reference_counter tagged_reference_counter;

	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(distributed_reference_counter) + sizeof(T)));

	distributed_reference_counter* refCounter = new (allocatedMemory) distributed_reference_counter();

	T* allocatedObject = new (allocatedMemory + sizeof(distributed_reference_counter)) T(std::forward<Args>(i_args) ...);

	tagged_reference_counter taggedRefCounter(refCounter,ReferenceAllocationType::Contiguous);

	return detail::__make_shared_reference(allocatedObject,taggedRefCounter,nullptr);
}

template<typename T>
atomic_shared_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	shared_reference_counter* refCounter = new shared_reference_counter();

	return detail::__make_shared_reference(i_ptr,refCounter,nullptr);
}
template<typename T>
atomic_shared_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const IReferenceWrapperDeleter* i_refDeleter)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	shared_reference_counter* refCounter = new shared_reference_counter();

	return detail::__make_shared_reference(i_ptr,refCounter,i_refDeleter);
}
template<typename T>
atomic_distributed_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const tagged_pointer<shared_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter = nullptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");
	DDK_ASSERT(i_refCounter.get_tag() == ReferenceAllocationType::Dynamic,"Trying to contruct shared reference non dynamic reference counter");

	return detail::__make_shared_reference(i_ptr,i_refCounter,i_refDeleter);
}

template<typename T>
atomic_distributed_reference_wrapper<T> as_atomic_distributed_reference(T* i_ptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	distributed_reference_counter* refCounter = new distributed_reference_counter();

	return detail::__make_shared_reference(i_ptr,refCounter,nullptr);
}
template<typename T>
atomic_distributed_reference_wrapper<T> as_atomic_distributed_reference(T* i_ptr,const IReferenceWrapperDeleter* i_refDeleter)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	distributed_reference_counter* refCounter = new distributed_reference_counter();

	return detail::__make_shared_reference(i_ptr,refCounter,i_refDeleter);
}
template<typename T>
atomic_distributed_reference_wrapper<T> as_atomic_shared_reference(T* i_ptr,const tagged_pointer<distributed_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter = nullptr)
{
	DDK_ASSERT(i_ptr != nullptr,"Trying to contruct shared reference from null pointer");

	return detail::__make_shared_reference(i_ptr,i_refCounter,i_refDeleter);
}

template<typename T>
inline T* get_raw_ptr(lent_pointer_wrapper<T>& i_ref)
{
#if defined(DDK_DEBUG)
	return i_ref.get();
#else
	return i_ref;
#endif
}
template<typename T>
inline const T* get_raw_ptr(const lent_pointer_wrapper<T>& i_ref)
{
#if defined(DDK_DEBUG)
	return i_ref.get();
#else
	return i_ref;
#endif
}
template<typename T>
inline T* extract_raw_ptr(lent_pointer_wrapper<T>& i_ref)
{
#if defined(DDK_DEBUG)
	return i_ref.extract();
#else
	T* res = i_ref;
	i_ref = nullptr;

	return res;
#endif
}
template<typename T>
inline void clear_ptr(lent_pointer_wrapper<T>& i_ref)
{
#if defined(DDK_DEBUG)
	i_ref.clear();
#else
	i_ref = nullptr;
#endif
}

template<typename TT, typename T, bool Weakable>
inline auto dynamic_shared_cast(const detail::shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__dynamic_shared_cast<TT>(i_sharedRef);
}

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> dynamic_unique_cast(unique_reference_wrapper<T>&& i_uniqueRef)
{
	if (TT* tData = dynamic_cast<TT*>(i_uniqueRef.m_data))
	{
		return detail::__make_unique_pointer(tData, std::move(i_uniqueRef.m_refCounter), i_uniqueRef.m_deleter);
	}
	else
	{
		return nullptr;
	}
}

#ifdef DDK_DEBUG

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> dynamic_lent_cast(const lent_reference_wrapper<T>& i_lentRef)
{
	if (TT* tData = dynamic_cast<TT*>(const_cast<T*>(i_lentRef.m_data)))
	{
		return detail::__make_lent_pointer(tData, i_lentRef.m_refCounter);
	}
	else
	{
		return nullptr;
	}
}

#endif

template<typename TT, typename T, bool Weakable>
inline auto dynamic_shared_cast(const detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__dynamic_shared_cast<TT>(i_sharedRef);
}

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> dynamic_unique_cast(unique_pointer_wrapper<T>&& i_uniqueRef)
{
	unique_pointer_wrapper<TT> res;

	if (TT* tData = dynamic_cast<TT*>(i_uniqueRef.m_data))
	{
		res = detail::__make_unique_pointer(tData, std::move(i_uniqueRef.m_refCounter), i_uniqueRef.m_deleter);
		i_uniqueRef.m_data = nullptr;
	}
	else
	{
		res = nullptr;
	}

	return res;
}

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> dynamic_lent_cast(const lent_pointer_wrapper<T>& i_lentRef)
{
#ifdef DDK_DEBUG
	if (TT* tData = dynamic_cast<TT*>(const_cast<T*>(i_lentRef.m_data)))
	{
		return detail::__make_lent_pointer(tData, i_lentRef.m_refCounter);
	}
	else
	{
		return nullptr;
	}
#else
	return dynamic_cast<TT*>(const_cast<T*>(i_lentRef));
#endif
}

template<typename TT,typename T,bool Weakable>
inline auto static_shared_cast(const detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedRef)
{
	static_assert(std::is_base_of<T,TT>::value || std::is_base_of<TT,T>::value,"Trying to cast unrelated classes");

	return detail::__static_shared_cast<TT>(i_sharedRef);
}

template<typename TT, typename T, bool Weakable>
inline auto static_shared_cast(const detail::shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	typedef typename std::remove_const<T>::type non_const_T;
	typedef typename std::remove_const<TT>::type non_const_TT;
	static_assert(std::is_base_of<non_const_T, non_const_TT>::value || std::is_base_of<non_const_TT, non_const_T>::value, "Trying to cast unrelated classes");

	return detail::__static_shared_cast<TT>(i_sharedRef);
}

template<typename TT, typename T>
inline unique_reference_wrapper<TT> static_unique_cast(unique_reference_wrapper<T> i_uniqueRef)
{
	typedef typename std::remove_const<T>::type non_const_T;
	typedef typename std::remove_const<TT>::type non_const_TT;
	static_assert(std::is_base_of<non_const_T, non_const_TT>::value || std::is_base_of<non_const_TT, non_const_T>::value, "Trying to cast unrelated classes");

	return  detail::__make_unique_reference(static_cast<TT*>(i_uniqueRef.m_data), std::move(i_uniqueRef.m_refCounter), i_uniqueRef.m_deleter);
}

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> static_unique_cast(unique_pointer_wrapper<T> i_uniqueRef)
{
	static_assert(std::is_base_of<T, TT>::value || std::is_base_of<TT, T>::value, "Trying to cast unrelated classes");

	unique_pointer_wrapper<TT> res;

	return detail::__make_unique_pointer(static_cast<TT*>(i_uniqueRef.m_data), std::move(i_uniqueRef.m_refCounter), i_uniqueRef.m_deleter);
}

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> static_lent_cast(const lent_pointer_wrapper<T>& i_lentRef)
{
	static_assert(std::is_base_of<T, TT>::value || std::is_base_of<TT, T>::value, "Trying to cast unrelated classes");

#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(static_cast<TT*>(const_cast<T*>(i_lentRef.m_data)), i_lentRef.m_refCounter);
#else
	return static_cast<TT*>(const_cast<T*>(i_lentRef));
#endif
}

template<typename TT, typename T, bool Weakable>
inline detail::shared_pointer_wrapper_impl<TT,Weakable> reinterpret_shared_cast(const detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__reinterpret_shared_cast<TT>(i_sharedPtr);
}

template<typename TT,typename T,bool Weakable>
inline detail::shared_reference_wrapper_impl<TT,Weakable> reinterpret_shared_cast(const detail::shared_reference_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__reinterpret_shared_cast<TT>(i_sharedPtr);
}

template<typename TT, typename T>
inline unique_pointer_wrapper<TT> reinterpret_unique_cast(unique_pointer_wrapper<T> i_uniquePtr)
{
	unique_pointer_wrapper<TT> res = detail::__make_unique_pointer(reinterpret_cast<TT*>(const_cast<T*>(i_uniquePtr.m_data)), i_uniquePtr.m_refCounter, i_uniquePtr.m_deleter);

	i_uniquePtr.clear();

	return std::move(res);
}

template<typename TT, typename T>
inline unique_reference_wrapper<TT> reinterpret_unique_cast(unique_reference_wrapper<T> i_uniqueRef)
{
	unique_reference_wrapper<TT> res = detail::__make_unique_reference(reinterpret_cast<TT*>(const_cast<T*>(i_uniqueRef.m_data)), i_uniqueRef.m_refCounter, i_uniqueRef.m_deleter);

	i_uniqueRef.clear();

	return std::move(res);
}

template<typename TT, typename T>
inline lent_pointer_wrapper<TT> reinterpret_lent_cast(const lent_pointer_wrapper<T>& i_lentRef)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(reinterpret_cast<TT*>(const_cast<T*>(i_lentRef.m_data)), i_lentRef.m_refCounter);
#else
	return reinterpret_cast<TT*>(const_cast<T*>(i_lentRef));
#endif
}

template<typename T>
inline unique_pointer_wrapper<T> const_unique_cast(unique_pointer_wrapper<const T> i_uniquePtr)
{
	unique_pointer_wrapper<typename std::add_const<T>::type> res = detail::__make_unique_pointer(const_cast<T*>(i_uniquePtr.m_data), i_uniquePtr.m_refCounter, i_uniquePtr.m_deleter);

	i_uniquePtr.clear();

	return std::move(res);
}
template<typename T>
inline unique_reference_wrapper<T> const_unique_cast(unique_reference_wrapper<const T> i_uniqueRef)
{
	unique_reference_wrapper<typename std::add_const<T>::type> res = detail::__make_unique_reference(const_cast<T*>(i_uniqueRef.m_data), i_uniqueRef.m_refCounter, i_uniqueRef.m_deleter);

	i_uniqueRef.clear();

	return std::move(res);
}

template<typename T, bool Weakable>
inline detail::shared_reference_wrapper_impl<T,Weakable> const_shared_cast(const detail::shared_reference_wrapper_impl<const T,Weakable>& i_sharedRef)
{
	return detail::__const_shared_cast(i_sharedRef);
}
template<typename T, bool Weakable>
inline detail::shared_pointer_wrapper_impl<T,Weakable> const_shared_cast(const detail::shared_pointer_wrapper_impl<const T,Weakable>& i_sharedPtr)
{
	return detail::__const_shared_cast(i_sharedRef);
}

template<typename T>
inline lent_pointer_wrapper<T> const_lent_cast(const lent_pointer_wrapper<const T>& i_lentPtr)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(const_cast<T*>(i_lentPtr.m_data), i_lentPtr.m_refCounter);
#else
	return const_cast<T*>(i_lentPtr);
#endif
}

#ifdef DDK_DEBUG

template<typename T>
inline lent_reference_wrapper<T> const_lent_cast(const lent_reference_wrapper<const T>& i_lentRef)
{
	return detail::__make_lent_reference(const_cast<T*>(i_lentRef.m_data), i_lentRef.m_refCounter);
}

#endif

template<typename T>
inline lent_pointer_wrapper<T> lend(unique_pointer_wrapper<T>& i_uniquePtr)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(i_uniquePtr.m_data, i_uniquePtr.m_refCounter);
#else
	return i_uniquePtr.m_data;
#endif
}
template<typename T>
inline lent_pointer_wrapper<T> lend(const unique_pointer_wrapper<T>& i_uniquePtr)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(i_uniquePtr.m_data, i_uniquePtr.m_refCounter);
#else
	return i_uniquePtr.m_data;
#endif
}

template<typename T>
inline lent_reference_wrapper<T> lend(unique_reference_wrapper<T>& i_uniqueRef)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_reference(i_uniqueRef.m_data, i_uniqueRef.m_refCounter);
#else
	return i_uniqueRef.m_data;
#endif
}
template<typename T>
inline lent_reference_wrapper<T> lend(const unique_reference_wrapper<T>& i_uniqueRef)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_reference(i_uniqueRef.m_data, i_uniqueRef.m_refCounter);
#else
	return i_uniqueRef.m_data;
#endif
}

template<typename T, bool Weakable>
inline lent_pointer_wrapper<T> lend(detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__lend(i_sharedPtr);
}
template<typename T, bool Weakable>
inline lent_pointer_wrapper<T> lend(const detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__lend(i_sharedPtr);
}

template<typename T, bool Weakable>
inline lent_reference_wrapper<T> lend(detail::shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__lend(i_sharedRef);
}
template<typename T, bool Weakable>
inline lent_reference_wrapper<T> lend(const detail::shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__lend(i_sharedRef);
}

template<typename T,bool Weakable>
inline lent_pointer_wrapper<T> lend(detail::atomic_shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__lend(static_cast<detail::shared_pointer_wrapper_impl<T>&>(i_sharedPtr));
}
template<typename T,bool Weakable>
inline lent_pointer_wrapper<T> lend(const detail::atomic_shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__lend(static_cast<const detail::shared_pointer_wrapper_impl<T>&>(i_sharedPtr));
}
template<typename T,bool Weakable>
inline lent_reference_wrapper<T> lend(detail::atomic_shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__lend(static_cast<detail::shared_reference_wrapper_impl<T>&>(i_sharedPtr));
}
template<typename T,bool Weakable>
inline lent_reference_wrapper<T> lend(const detail::atomic_shared_reference_wrapper_impl<T,Weakable>& i_sharedRef)
{
	return detail::__lend(static_cast<const detail::shared_reference_wrapper_impl<T>&>(i_sharedPtr));
}

template<typename T, typename TT>
inline lent_reference_wrapper<TT> lend(lend_from_this<T,TT>& i_lendable)
{
	return i_lendable.ref_from_this();
}
template<typename T, typename TT>
inline lent_reference_wrapper<const TT> lend(const lend_from_this<T,TT>& i_lendable)
{
	return i_lendable.ref_from_this();
}

template<typename T>
inline lent_reference_wrapper<T> lend(lendable<T>& i_lendable)
{
	return i_lendable.ref_from_this();
}
template<typename T>
inline lent_reference_wrapper<const T> lend(const lendable<T>& i_lendable)
{
	return i_lendable.ref_from_this();
}

template<typename T,typename TT>
inline lent_reference_wrapper<TT> lend(share_from_this<T,TT>& i_sharedFromThis)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(static_cast<TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter);
#else
	return static_cast<TT*>(&i_sharedFromThis);
#endif
}
template<typename T,typename TT>
inline lent_reference_wrapper<const TT> lend(const share_from_this<T,TT>& i_sharedFromThis)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_pointer(static_cast<const TT*>(&i_sharedFromThis),i_sharedPtr.m_refCounter);
#else
	return static_cast<const TT*>(&i_sharedFromThis);
#endif
}

template<typename T,typename TT>
inline weak_pointer_wrapper<TT> weak(share_from_this<T,TT>& i_sharedFromThis)
{
	return detail::__make_weak_pointer(static_cast<TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
}
template<typename T,typename TT>
inline weak_pointer_wrapper<const TT> weak(const share_from_this<TT>& i_sharedFromThis)
{
	return detail::__make_weak_pointer(static_cast<const TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
}
template<typename T>
inline weak_pointer_wrapper<T> weak(shared_pointer_wrapper<T>& i_sharedPtr)
{
	return detail::__weak(i_sharedPtr);
}
template<typename T>
inline weak_pointer_wrapper<const T> weak(const shared_pointer_wrapper<T>& i_sharedPtr)
{
	return detail::__weak(static_cast<shared_pointer_wrapper<const T>>(i_sharedPtr));
}

template<typename T>
inline atomic_weak_pointer_wrapper<T> weak(atomic_shared_pointer_wrapper<T>& i_sharedPtr)
{
	return detail::__weak(static_cast<shared_pointer_wrapper<T>&>(i_sharedPtr));
}
template<typename T>
inline atomic_weak_pointer_wrapper<const T> weak(const atomic_shared_pointer_wrapper<T>& i_sharedPtr)
{
	return detail::__weak(static_cast<const shared_pointer_wrapper<T>&>(i_sharedPtr));
}

template<typename T, typename TT>
inline shared_pointer_wrapper<const TT> share(const share_from_this<T,TT>& i_shareFromThis)
{
	return as_shared_reference(static_cast<const TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
}
template<typename T, typename TT>
inline shared_pointer_wrapper<TT> share(share_from_this<T,TT>& i_shareFromThis)
{
	return as_shared_reference(static_cast<TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
}

template<typename T>
inline shared_pointer_wrapper<const T> share(const weak_pointer_wrapper<T>& i_weakPtr)
{
	return i_weakPtr.share();
}
template<typename T>
inline shared_pointer_wrapper<T> share(weak_pointer_wrapper<T>& i_weakPtr)
{
	return i_weakPtr.share();
}

template<typename T,typename TT>
inline distributed_reference_wrapper<TT> distribute(distribute_from_this<T,TT>& i_distFromThis)
{
	return as_distributed_reference(static_cast<TT*>(&i_distFromThis),&i_distFromThis.m_refCounter,i_distFromThis.m_deleter);
}
template<typename T,typename TT>
inline distributed_reference_wrapper<const TT> distribute(const distribute_from_this<T,TT>& i_distFromThis)
{
	return as_distributed_reference(static_cast<const TT*>(&i_distFromThis),&i_distFromThis.m_refCounter,i_distFromThis.m_deleter);
}

template<typename T>
inline atomic_shared_pointer_wrapper<const T> share(const atomic_weak_pointer_wrapper<T>& i_weakPtr)
{
	return i_weakPtr.share();
}
template<typename T>
inline atomic_shared_pointer_wrapper<T> share(atomic_weak_pointer_wrapper<T>& i_weakPtr)
{
	return i_weakPtr.share();
}

template<typename T>
unique_reference_wrapper<T> promote_to_ref(unique_pointer_wrapper<T> i_uniquePtr)
{
	return detail::__make_unique_reference(i_uniquePtr.m_data, std::move(i_uniquePtr.m_refCounter), i_uniquePtr.m_deleter);
}
template<typename T, bool Weakable>
detail::shared_reference_wrapper_impl<T,Weakable> promote_to_ref(const detail::shared_pointer_wrapper_impl<T,Weakable>& i_sharedPtr)
{
	return detail::__promote_to_ref(i_sharedPtr);
}
template<typename T>
lent_reference_wrapper<T> promote_to_ref(const lent_pointer_wrapper<T>& i_lentPtr)
{
#ifdef DDK_DEBUG
	return detail::__make_lent_reference(i_lentPtr.m_data, i_lentPtr.m_refCounter);
#else
	return i_lentPtr;
#endif
}

template<typename T>
bool operator==(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() == i_rhs.get();
}
template<typename T>
bool operator==(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() == i_rhs.get();
#else
	return i_lhs.get() == i_rhs;
#endif
}
template<typename T>
bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() == i_rhs.get();
#else
	return i_lhs == i_rhs.get();
#endif
}
#ifdef DDK_DEBUG
template<typename T>
bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() == i_rhs.get();
}
#endif

template<typename T>
bool operator!=(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() != i_rhs.get();
}
template<typename T>
bool operator!=(const ddk::unique_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() != i_rhs.get();
#else
	return i_lhs.get() != i_rhs;
#endif
}
template<typename T>
bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::unique_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() != i_rhs.get();
#else
	return i_lhs != i_rhs.get();
#endif
}
#ifdef DDK_DEBUG
template<typename T>
bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() != i_rhs.get();
}
#endif

template<typename T>
bool operator==(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() == i_rhs.get();
}
template<typename T>
bool operator==(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() == i_rhs.get();
#else
	return i_lhs.get() == i_rhs;
#endif
}
template<typename T>
bool operator==(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() == i_rhs.get();
#else
	return i_lhs == i_rhs.get();
#endif
}

template<typename T>
bool operator!=(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs)
{
	return i_lhs.get() != i_rhs.get();
}
template<typename T>
bool operator!=(const ddk::shared_pointer_wrapper<T>& i_lhs, const ddk::lent_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() != i_rhs.get();
#else
	return i_lhs.get() == i_rhs;
#endif
}
template<typename T>
bool operator!=(const ddk::lent_pointer_wrapper<T>& i_lhs, const ddk::shared_pointer_wrapper<T>& i_rhs)
{
#ifdef DDK_DEBUG
	return i_lhs.get() != i_rhs.get();
#else
	return i_lhs != i_rhs.get();
#endif
}

template<typename T>
struct smart_pointer_compare
{
public:
	smart_pointer_compare() = default;
	inline bool operator()(const T& i_lhs, const T& i_rhs) const
	{
		return get_raw_ptr(i_lhs) < get_raw_ptr(i_rhs);
	}
};

}
