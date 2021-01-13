#pragma once

#include "ddk_template_helper.h"
#include "ddk_tuple.h"
#include "ddk_system_allocator.h"

#define define_place_arg(_INDEX) \
static const ddk::mpl::place_holder<_INDEX> arg_##_INDEX;

namespace ddk
{

template<typename,typename = system_allocator>
class function;

namespace mpl
{

template<size_t Index>
struct place_holder
{
};

template<typename>
struct is_place_holder;

template<typename T>
struct is_place_holder
{
    static const bool value = false;
};
template<size_t Index>
struct is_place_holder<place_holder<Index>>
{
    static const bool value = true;
};

template<size_t currIndex,typename>
struct pos_place_holder;

template<size_t currIndex>
struct pos_place_holder<currIndex,tuple<>>
{
    typedef sequence<> type;
};

template<size_t currIndex,typename Arg, typename ... Args>
struct pos_place_holder<currIndex,tuple<Arg,Args...>>
{
private:
    typedef typename pos_place_holder<currIndex+1,tuple<Args...>>::type next_seq;
    typedef typename std::remove_const<typename std::remove_reference<Arg>::type>::type raw_arg;

public:
    typedef typename static_if<is_place_holder<raw_arg>::value,typename merge_sequence<sequence<currIndex>,next_seq>::type,next_seq>::type type;
};

template<typename>
struct sequence_place_holder;

template<>
struct sequence_place_holder<tuple<>>
{
    typedef sequence<> type;
};

template<typename Arg, typename ... Args>
struct sequence_place_holder<tuple<Arg,Args...>>
{
private:
    template<typename T>
    struct arg_sequence
    {
        typedef sequence<> type;
    };
    template<size_t Index>
    struct arg_sequence<place_holder<Index>>
    {
        typedef sequence<Index> type;
    };

    typedef typename std::remove_const<typename std::remove_reference<Arg>::type>::type raw_arg;
    typedef typename arg_sequence<raw_arg>::type curr_sequence;
    typedef typename sequence_place_holder<tuple<Args...>>::type next_sequence;

public:
    typedef typename merge_sequence<curr_sequence,next_sequence>::type type;
};

template<typename>
struct aqcuire_callable_return_type;

template<typename Return, typename ... Args>
struct aqcuire_callable_return_type<Return(*)(Args...)>
{
	typedef Return return_type;
	typedef tuple<Args...> args_type;
};
template<typename Return, typename T, typename ... Args>
struct aqcuire_callable_return_type<Return(T::*)(Args...) const>
{
	typedef Return return_type;
	typedef tuple<Args...> args_type;
};
template<typename Return,typename T,typename ... Args>
struct aqcuire_callable_return_type<Return(T::*)(Args...)>
{
	typedef Return return_type;
	typedef tuple<Args...> args_type;
};
template<typename Functor>
struct aqcuire_callable_return_type
{
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type return_type;
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::args_type args_type;
};

template<typename>
struct _is_function;

template<typename T>
struct _is_function: public std::false_type
{
};
template<typename Return,typename ... Types,typename Allocator>
struct _is_function<function<Return(Types...),Allocator>> : public std::true_type
{
};

template<typename T>
struct is_function
{
private:
	typedef typename std::remove_const<typename std::remove_reference<T>::type>::type raw_type;

public:
	static const bool value = _is_function<raw_type>::value;
};

template<typename T, typename ... Args>
struct is_valid_functor
{
private:
    template<typename TT>
	static std::true_type _resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().operator()(std::declval<Args>() ...))>::type);
    template<typename TT>
	static std::false_type _resolve(TT&, ...);

    template<typename TT>
	static std::true_type resolve(TT&, typename TT::callable_tag*);
    template<typename TT>
	static std::true_type resolve(TT&, decltype(&TT::operator()));
    template<typename TT>
	static decltype(_resolve(std::declval<T&>(),nullptr)) resolve(TT&, ...);

public:
    typedef typename static_if<is_function<T>::value,std::true_type,decltype(resolve(std::declval<T&>(),nullptr))>::type type;
    static const bool value = type::value;
};

}

define_place_arg(0)
define_place_arg(1)
define_place_arg(2)
define_place_arg(3)
define_place_arg(4)
define_place_arg(5)
define_place_arg(6)
define_place_arg(7)
define_place_arg(8)
define_place_arg(9)

}
