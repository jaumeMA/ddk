#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T, bool Weakable>
class shared_reference_wrapper_impl : public shared_pointer_wrapper_impl<T,Weakable>
{
	template<typename,bool>
	friend class shared_reference_wrapper_impl;
	template<typename TTT, typename TT, bool WWeakable>
	friend shared_reference_wrapper_impl<TTT,WWeakable> __static_shared_cast(const shared_reference_wrapper_impl<TT,WWeakable>&);
	template<typename TTT, typename TT,bool WWeakable>
	friend shared_reference_wrapper_impl<TTT,WWeakable> __dynamic_shared_cast(const shared_reference_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend lent_reference_wrapper<TT> __lend(const shared_reference_wrapper_impl<TT,WWeakable>&);
	template<typename TT>
	friend inline shared_reference_wrapper_impl<TT,true> __make_shared_reference(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
	template<typename TT>
	friend inline shared_reference_wrapper_impl<TT,false> __make_shared_reference(TT* i_data,const tagged_pointer<distributed_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter);
	typedef typename mpl::static_if<Weakable,shared_reference_counter,distributed_reference_counter>::type reference_counter;

	shared_reference_wrapper_impl(T* i_data, const tagged_pointer<reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr);

	using shared_pointer_wrapper_impl<T,Weakable>::operator bool;
	using shared_pointer_wrapper_impl<T,Weakable>::empty;

public:
	using typename shared_pointer_wrapper_impl<T,Weakable>::tagged_reference_counter;
    using shared_pointer_wrapper_impl<T,Weakable>::operator*;
    using shared_pointer_wrapper_impl<T,Weakable>::operator->;
    using shared_pointer_wrapper_impl<T,Weakable>::get;
    using shared_pointer_wrapper_impl<T,Weakable>::clear;

	shared_reference_wrapper_impl(const shared_reference_wrapper_impl& other);
	shared_reference_wrapper_impl(shared_reference_wrapper_impl&& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,WWeakable>& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,WWeakable>&& other);
    shared_reference_wrapper_impl& operator=(const shared_reference_wrapper_impl& other);
    shared_reference_wrapper_impl& operator=(shared_reference_wrapper_impl&& other);
};

}

template<typename T>
using shared_reference_wrapper = detail::shared_reference_wrapper_impl<T,true>;
template<typename T>
using distributed_reference_wrapper = detail::shared_reference_wrapper_impl<T,false>;

}

#include "ddk_shared_reference_wrapper.inl"