//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_unique_pointer_wrapper.h"

namespace ddk
{

template<typename>
class unique_reference_wrapper;

namespace detail
{

template<typename TT>
unique_reference_wrapper<TT> __make_unique_reference(TT* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter);
template<typename TT>
unique_reference_wrapper<TT> __make_unique_reference(TT* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter);

}


template<typename T>
class unique_reference_wrapper : public unique_pointer_wrapper<T>
{
	template<typename TT>
	friend unique_reference_wrapper<TT> detail::__make_unique_reference(TT* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter);
	template<typename TT>
	friend unique_reference_wrapper<TT> detail::__make_unique_reference(TT* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter);

    using unique_pointer_wrapper<T>::operator bool;
    using unique_pointer_wrapper<T>::clear;

	unique_reference_wrapper(T* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter);
	unique_reference_wrapper(T* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter);

public:
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
	using typename unique_pointer_wrapper<T>::value_type;
	using typename unique_pointer_wrapper<T>::const_value_type;
	using typename unique_pointer_wrapper<T>::reference;
	using typename unique_pointer_wrapper<T>::const_reference;
	using typename unique_pointer_wrapper<T>::rreference;
	using typename unique_pointer_wrapper<T>::pointer;
	using typename unique_pointer_wrapper<T>::const_pointer;
	typedef unique_reference_wrapper<const_value_type> const_type;
	using unique_pointer_wrapper<T>::operator*;
    using unique_pointer_wrapper<T>::operator->;
    using unique_pointer_wrapper<T>::get;

	unique_reference_wrapper(const unique_reference_wrapper&) = delete;
	template<typename TT>
	unique_reference_wrapper(const unique_reference_wrapper<TT>&) = delete;
	unique_reference_wrapper(unique_reference_wrapper<T>&& other);
    template<typename TT>
	unique_reference_wrapper(unique_reference_wrapper<TT>&& other);
    unique_reference_wrapper& operator=(unique_reference_wrapper<T>&& other);
};

}

#include "ddk_unique_reference_wrapper.inl"
