#pragma once

#include "ddk_embedded_type.h"
#include "ddk_template_helper.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<typename ... Types>
class variant_impl
{
    template<typename ...>
    friend class variant_impl;
    template<typename,typename ...>
    friend class variant_visitor_invoker;
    static const int _numTypes = mpl::get_num_types<Types...>::value;

public:
	static const size_t npos = _numTypes;

    variant_impl();
    void construct(const variant_impl<Types...>& other);
    void construct(variant_impl<Types...>&& other);
    template<typename ... TTypes>
    void construct(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    void construct(variant_impl<TTypes...>&& other);
    template<size_t Index, typename TType>
    void construct(TType&& other);
    void destroy();
    ~variant_impl();
    variant_impl<Types...>& operator=(const variant_impl<Types...>& other);
    variant_impl<Types...>& operator=(variant_impl<Types...>&& other);
    template<typename ... TTypes>
    variant_impl<Types...>& operator=(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    variant_impl<Types...>& operator=(variant_impl<TTypes...>&& other);
    template<size_t Index, typename TType>
    variant_impl<Types...>& assign(TType&& val);
    bool operator==(const variant_impl<Types...>& other) const;
    bool operator==(variant_impl<Types...>&& other) const;
    bool operator!=(const variant_impl<Types...>& other) const;
    bool operator!=(variant_impl<Types...>&& other) const;
    template<size_t Index, typename TType>
    bool compare(TType&& other) const;
    template<typename TType>
    typename embedded_type<TType>::cref_type get() const;
    template<typename TType>
    typename embedded_type<TType>::ref_type get();
    template<typename TType>
    TType extract();
    template<typename TType>
    bool is() const;
	template<typename TType>
	typename embedded_type<TType>::cref_type get_as() const;
	template<typename TType>
	typename embedded_type<TType>::ref_type get_as();
	template<typename TType>
	bool is_base_of() const;
	template<size_t Pos>
    typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cref_type get() const;
    template<size_t Pos>
    typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::ref_type get();
    template<size_t Pos>
    embedded_type<typename mpl::nth_type_of<Pos,Types...>::type> extract();
    template<size_t Pos>
    bool is() const;
    bool empty() const;
    char which() const;
    void reset();
    void swap(variant_impl<Types...>& other);
    template<typename Visitor>
    typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor);
    template<typename Visitor>
    typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor) const;

private:
    typedef typename mpl::max_type<embedded_type<Types>...>::type dominantType;
	typedef arena<sizeof(dominantType), std::alignment_of<dominantType>::value> data_type;

    data_type m_storage;
    unsigned char m_currentType;
};

}
}

#include "ddk_variant_impl.inl"
