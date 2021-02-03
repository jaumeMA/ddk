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
    static const size_t s_numTypes = mpl::get_num_types<Types...>();

public:
	static const size_t npos = s_numTypes;

    variant_impl();
    inline void construct(const variant_impl& other);
    inline void construct(variant_impl&& other);
    template<typename ... TTypes>
    inline void construct(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    inline void construct(variant_impl<TTypes...>&& other);
    template<size_t Index, typename TType>
    inline void construct(TType&& other);
    inline void destroy();
    ~variant_impl();
    inline variant_impl<Types...>& operator=(const variant_impl<Types...>& other);
    inline variant_impl<Types...>& operator=(variant_impl<Types...>&& other);
    template<typename ... TTypes>
    inline variant_impl<Types...>& operator=(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    inline variant_impl<Types...>& operator=(variant_impl<TTypes...>&& other);
    template<size_t Index, typename TType>
    inline variant_impl<Types...>& assign(TType&& val);
    inline bool operator==(const variant_impl<Types...>& other) const;
    inline bool operator==(variant_impl<Types...>&& other) const;
    inline bool operator!=(const variant_impl<Types...>& other) const;
    inline bool operator!=(variant_impl<Types...>&& other) const;
    template<size_t Index, typename TType>
    inline bool compare(TType&& other) const;
    template<typename TType>
	inline typename embedded_type<TType>::cref_type get() const;
    template<typename TType>
	inline typename embedded_type<TType>::ref_type get();
    template<typename TType>
	inline TType extract() &&;
    template<typename TType>
	inline bool is() const;
	template<typename TType>
	inline typename embedded_type<TType>::cref_type get_as() const;
	template<typename TType>
	inline typename embedded_type<TType>::ref_type get_as();
	template<typename TType>
	inline bool is_base_of() const;
	template<size_t Pos>
	inline typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cref_type get() const;
    template<size_t Pos>
	inline typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::ref_type get();
    template<size_t Pos>
	inline embedded_type<typename mpl::nth_type_of<Pos,Types...>::type> extract() &&;
    template<size_t Pos>
    inline bool is() const;
    inline char which() const;
    inline void swap(variant_impl<Types...>& other);
    template<typename Visitor>
	inline typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor);
    template<typename Visitor>
	inline typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor) const;

private:
    typedef typename mpl::max_type<embedded_type<Types>...>::type dominantType;
	typedef arena<sizeof(dominantType), std::alignment_of<dominantType>::value> data_type;

    data_type m_storage;
    unsigned char m_currentType;
};

}
}

#include "ddk_variant_impl.inl"
