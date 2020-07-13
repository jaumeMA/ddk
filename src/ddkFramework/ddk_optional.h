#pragma once

#include "ddk_arena.h"
#include "ddk_none.h"

namespace ddk
{

template<typename T>
class optional
{
	template<typename TT>
	friend class optional;

public:
    //publish relevant inner types
    typedef const typename embedded_type<T>::raw_type& reference_const_type;
    typedef typename embedded_type<T>::raw_type& reference_type;
    typedef typename embedded_type<T>::raw_type&& rreference_type;
    typedef typename embedded_type<T>::raw_type* pointer_type;
    typedef const typename embedded_type<T>::raw_type* pointer_const_type;

    optional();
    optional(detail::none_t);
    optional(const optional<T>& other);
    optional(optional<T>&& other);
    optional(reference_type val);
    optional(reference_const_type val);
    optional(rreference_type val);
    template<typename TT>
    optional(const optional<TT>& other);
    template<typename TT>
    optional(optional<TT>&& other);
    template<typename Arg>
    explicit optional(Arg&& i_arg, typename std::enable_if<std::is_convertible<Arg,optional<T>>::value == false>::type* = NULL);
    ~optional();
    optional<T>& operator=(const optional<T>& other);
    optional<T>& operator=(optional<T>&& other);
    template<typename TT>
    optional<T>& operator=(const optional<TT>& other);
    template<typename TT>
    optional<T>& operator=(optional<TT>&& other);
    optional<T>& operator=(detail::none_t);
    optional<T>& operator=(reference_type val);
    optional<T>& operator=(reference_const_type val);
    optional<T>& operator=(rreference_type val);
	inline bool empty() const;
    inline explicit operator bool() const;
    inline bool operator!() const;
    typename embedded_type<T>::cref_type get() const;
    typename embedded_type<T>::ref_type get();
    typename embedded_type<T>::cref_type get_value_or(typename embedded_type<T>::cref_type default_value) const;
    typename embedded_type<T>::ref_type get_value_or(typename embedded_type<T>::ref_type default_value);
    typename embedded_type<T>::cpointer_type get_ptr() const;
    typename embedded_type<T>::pointer_type get_ptr();
    T extract();
    typename embedded_type<T>::cref_type operator*() const;
    typename embedded_type<T>::ref_type operator*();
    typename embedded_type<T>::cpointer_type operator->() const;
    typename embedded_type<T>::pointer_type operator->();
    void swap(const optional<T>& other);

private:
	arena<sizeof(embedded_type<T>),std::alignment_of<embedded_type<T>>::value> m_storage;
};

}

#include "ddk_optional.inl"
