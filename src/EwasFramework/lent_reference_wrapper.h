#pragma once

#include "lent_pointer_wrapper.h"

namespace ewas
{

template<typename>
class unique_reference_wrapper;
template<typename>
class shared_reference_wrapper;
template<typename>
class lendable;

#ifdef EWAS_DEBUG

template<typename T>
class lent_reference_wrapper : public lent_pointer_wrapper<T>
{
	template<typename TT,typename TTT>
	friend class lend_from_this;
	template<typename>
	friend class lendable;
	template<typename TT>
	friend lent_reference_wrapper<TT> promote_to_ref(const lent_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const unique_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const shared_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> __make_lent_reference(TT* i_data, const tagged_pointer<lent_reference_counter>& i_refCounter);

	lent_reference_wrapper(T* i_data, const tagged_pointer<lent_reference_counter>& i_refCounter)
	: lent_pointer_wrapper<T>(i_data,i_refCounter)
	{
		EWAS_ASSERT(i_data!=NULL, "Trying to construct non null reference from void pointer");
	}

    using lent_pointer_wrapper<T>::operator bool;
    using lent_pointer_wrapper<T>::operator=;

public:
    template<typename TT>
	lent_reference_wrapper(const lent_reference_wrapper<TT>& other, typename std::enable_if<std::is_base_of<T,TT>::value>::type* = NULL)
	: lent_pointer_wrapper<T>(other)
	{
	}
    template<typename TT>
	lent_reference_wrapper(lent_reference_wrapper<TT>&& other, typename std::enable_if<std::is_base_of<T,TT>::value>::type* = NULL)
	: lent_pointer_wrapper<T>(std::move(other))
	{
	}
    template<typename TT>
	lent_reference_wrapper(const unique_reference_wrapper<TT>& other, typename std::enable_if<std::is_base_of<T,TT>::value>::type* = NULL)
	: lent_pointer_wrapper<T>(other)
	{
	}
};

#else

template<typename T>
using lent_reference_wrapper = T*;

#endif

}
