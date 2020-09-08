#pragma once

#include "ddk_template_helper.h"
#include "ddk_tuple.h"

namespace ddk
{
namespace mpl
{

#define define_place_arg(_INDEX) \
static const ddk::mpl::place_holder<_INDEX> arg_##_INDEX;

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
template<typename Functor>
struct aqcuire_callable_return_type
{
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type return_type;
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::args_type args_type;
};

template<typename T>
struct is_valid_functor
{
private:
    template<typename TT>
    char static func(const TT&, decltype(&TT::operator()) = nullptr);
    template<typename ... TT>
    int static func(const TT& ...);

public:
    static const bool value = (sizeof(func(std::declval<T>())) == sizeof(char));
};

}

define_place_arg(0)
define_place_arg(1)
define_place_arg(2)
define_place_arg(3)
define_place_arg(4)
define_place_arg(5)

}
