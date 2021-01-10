#pragma once

#include "ddk_iterable_action.h"
#include "ddk_tuple_values.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

template<typename>
struct base_iterable_traits;

template<typename T>
struct base_iterable_traits
{
    typedef T base_type;
    typedef typename std::remove_reference<T>::type value_type;
    typedef T& reference;
    typedef typename std::add_const<T>::type& const_reference;
    typedef T* pointer;
    typedef typename std::add_const<T>::type* const_pointer;

    static inline pointer get_address(reference i_ref)
    {
        return &i_ref;
    }
    static inline reference get_value(pointer i_ptr)
    {
        return *i_ptr;
    }
};

template<typename ... Types>
struct base_iterable_traits<values_tuple<Types...>>
{
    typedef values_tuple<Types...> base_type;
    typedef tuple<typename std::remove_reference<Types>::type ...> value_type;
    typedef tuple<Types& ...> reference;
    typedef tuple<typename std::add_const<Types>::type& ...> const_reference;
    typedef tuple<Types* ...> pointer;
    typedef tuple<typename std::add_const<Types>::type* ...> const_pointer;

    static inline pointer get_address(reference i_ref)
    {
        return get_address(i_ref,typename mpl::make_sequence<0,values_tuple<Types...>::size()>::type{});
    }
    template<size_t ... Indexs>
    static inline pointer get_address(reference i_ref, const mpl::sequence<Indexs...>&)
    {
        return make_tuple(&i_ref.template get<Indexs>() ...);
    }
    static inline reference get_value(pointer i_ptr)
    {
        return get_value(i_ptr,typename mpl::make_sequence<0,values_tuple<Types...>::size()>::type{});
    }
    template<size_t ... Indexs>
    static inline reference get_value(pointer i_ptr, const mpl::sequence<Indexs...>&)
    {
        return make_tuple(*i_ptr.template get<Indexs>() ...);
    }
};

template<typename ... Types>
struct base_iterable_traits<const values_tuple<Types...>>
{
    typedef const values_tuple<Types...> base_type;
    typedef tuple<typename std::remove_reference<Types>::type ...> value_type;
    typedef tuple<typename std::add_const<Types>::type& ...> reference;
    typedef tuple<typename std::add_const<Types>::type& ...> const_reference;
    typedef tuple<typename std::add_const<Types>::type* ...> pointer;
    typedef tuple<typename std::add_const<Types>::type* ...> const_pointer;

    static inline pointer get_address(reference i_ref)
    {
        return get_address(i_ref,typename mpl::make_sequence<0,values_tuple<Types...>::size()>::type{});
    }
    template<size_t ... Indexs>
    static inline pointer get_address(reference i_ref, const mpl::sequence<Indexs...>&)
    {
        return make_tuple(&i_ref.template get<Indexs>() ...);
    }
    static inline reference get_value(pointer i_ptr)
    {
        return get_value(i_ptr,typename mpl::make_sequence<0,values_tuple<Types...>::size()>::type{});
    }
    template<size_t ... Indexs>
    static inline reference get_value(pointer i_ptr, const mpl::sequence<Indexs...>&)
    {
        return make_tuple(*i_ptr.template get<Indexs>() ...);
    }
};

template<typename T>
struct const_input_iterable_traits : base_iterable_traits<T>
{
    typedef iter::const_forward_action action;
	template<typename TT>
	using as = const_input_iterable_traits<TT>;

    static constexpr iter::const_forward_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct input_iterable_traits: base_iterable_traits<T>
{
	typedef iter::forward_action action;
	template<typename TT>
	using as = input_iterable_traits<TT>;

	static constexpr iter::forward_action default_action()
	{
		return iter::go_next_place;
	}
};

template<typename T>
struct const_forward_iterable_traits : base_iterable_traits<T>
{
    typedef iter::const_forward_action action;
	typedef std::forward_iterator_tag iterable_tag;
	template<typename TT>
	using as = const_forward_iterable_traits<TT>;

    static constexpr iter::const_forward_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct forward_iterable_traits: base_iterable_traits<T>
{
	typedef iter::forward_action action;
	typedef std::forward_iterator_tag iterable_tag;
	template<typename TT>
	using as = forward_iterable_traits<TT>;

	static constexpr iter::forward_action default_action()
	{
		return iter::go_next_place;
	}
};

template<typename T>
struct const_bidirectional_iterable_traits : base_iterable_traits<T>
{
    typedef iter::const_bidirectional_action action;
	typedef std::bidirectional_iterator_tag iterable_tag;
	template<typename TT>
	using as = const_bidirectional_iterable_traits<TT>;

    static constexpr iter::const_bidirectional_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct bidirectional_iterable_traits: base_iterable_traits<T>
{
	typedef iter::bidirectional_action action;
	typedef std::bidirectional_iterator_tag iterable_tag;
	template<typename TT>
	using as = bidirectional_iterable_traits<TT>;

	static constexpr iter::bidirectional_action default_action()
	{
		return iter::go_next_place;
	}
};

template<typename T>
struct const_random_access_iterable_traits : base_iterable_traits<T>
{
    typedef iter::const_random_access_action action;
	typedef std::random_access_iterator_tag iterable_tag;
	template<typename TT>
	using as = const_random_access_iterable_traits<TT>;

    static constexpr iter::const_random_access_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct random_access_iterable_traits: base_iterable_traits<T>
{
	typedef iter::random_access_action action;
	typedef std::random_access_iterator_tag iterable_tag;
	template<typename TT>
	using as = random_access_iterable_traits<TT>;

	static constexpr iter::random_access_action default_action()
	{
		return iter::go_next_place;
	}
};

}
}
