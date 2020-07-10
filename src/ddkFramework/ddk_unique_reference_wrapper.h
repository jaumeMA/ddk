#pragma once

#include "unique_pointer_wrapper.h"

namespace ewas
{

template<typename T>
class unique_reference_wrapper : public unique_pointer_wrapper<T>
{
  template<typename TT>
  friend unique_reference_wrapper<TT> __make_unique_reference(TT* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
  template<typename TT>
  friend unique_reference_wrapper<TT> __make_unique_reference(TT* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);

	unique_reference_wrapper(const unique_reference_wrapper&);
	template<typename TT>
	unique_reference_wrapper(const unique_reference_wrapper<TT>&);
	unique_reference_wrapper(T* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = NULL)
	: unique_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter)
	{
		EWAS_ASSERT(i_data!=NULL, "Trying to construct non null reference from void pointer");
	}
	unique_reference_wrapper(T* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = NULL)
	: unique_pointer_wrapper<T>(i_data,std::move(i_refCounter),i_refDeleter)
	{
		EWAS_ASSERT(i_data!=NULL, "Trying to construct non null reference from void pointer");
	}
    using unique_pointer_wrapper<T>::operator bool;
    using unique_pointer_wrapper<T>::clear;

public:
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
    using unique_pointer_wrapper<T>::operator*;
    using unique_pointer_wrapper<T>::operator->;
    using unique_pointer_wrapper<T>::get;

	unique_reference_wrapper(unique_reference_wrapper<T>&& other)
	: unique_pointer_wrapper<T>(std::move(other))
	{
	}
    template<typename TT>
	unique_reference_wrapper(unique_reference_wrapper<TT>&& other)
	: unique_pointer_wrapper<T>(std::move(other))
	{
	}
    unique_reference_wrapper& operator=(unique_reference_wrapper<T>&& other)
	{
		unique_pointer_wrapper<T>::operator=(std::move(other));

		return *this;
	}
};

namespace mpl
{

template<typename T>
struct is_copy_constructible<unique_reference_wrapper<T>>
{
    static const bool value = false;
};

}
}
