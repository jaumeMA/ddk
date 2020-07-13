#pragma once

#include "ddk_shared_pointer_wrapper.h"

namespace ddk
{

template<typename T>
class shared_reference_wrapper : public shared_pointer_wrapper<T>
{
	template<typename>
	friend class shared_reference_wrapper;
	template<typename TTT, typename TT>
	friend shared_reference_wrapper<TTT> static_shared_cast(const shared_reference_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend shared_pointer_wrapper<TTT> dynamic_shared_cast(const shared_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const shared_reference_wrapper<TT>&);
	template<typename TT>
	friend shared_reference_wrapper<TT> __make_shared_reference(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);

	shared_reference_wrapper(T* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = NULL)
	: shared_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter)
	{
		DDK_ASSERT(i_data!=NULL, "Trying to construct non null reference from void pointer");
	}
    using shared_pointer_wrapper<T>::operator bool;
	using shared_pointer_wrapper<T>::empty;

public:
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
    using shared_pointer_wrapper<T>::operator*;
    using shared_pointer_wrapper<T>::operator->;
    using shared_pointer_wrapper<T>::get;
    using shared_pointer_wrapper<T>::clear;

	shared_reference_wrapper(const shared_reference_wrapper<T>& other)
	: shared_pointer_wrapper<T>(other)
	{
	}
	shared_reference_wrapper(shared_reference_wrapper<T>&& other)
	: shared_pointer_wrapper<T>(std::move(other))
	{
	}
	template<typename TT>
	shared_reference_wrapper(const shared_reference_wrapper<TT>& other)
	: shared_pointer_wrapper<T>(other)
	{
	}
	template<typename TT>
	shared_reference_wrapper(shared_reference_wrapper<TT>&& other)
	: shared_pointer_wrapper<T>(std::move(other))
	{
	}
    shared_reference_wrapper& operator=(const shared_reference_wrapper<T>& other)
	{
		shared_pointer_wrapper<T>::operator=(other);

		return *this;
	}
    shared_reference_wrapper& operator=(shared_reference_wrapper<T>&& other)
	{
		shared_pointer_wrapper<T>::operator=(std::move(other));

		return *this;
	}
};

}
