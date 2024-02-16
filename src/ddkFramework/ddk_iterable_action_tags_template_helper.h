#pragma once

#include "ddk_template_helper.h"
#include "ddk_function_arguments_template_helper.h"

#define PUBLISH_ACTION_TAG(_ACTION,_TAG) \
    typename decltype(__get_action_tag_list(std::declval<ddk::rtti::detail::static_typed_number<_TAG,ddk::static_counter<_TAG>::get_curr_count()>>()))::add<decltype(ddk::mpl::resolve_holder<_ACTION>())>::type __get_action_tag_list(const ddk::rtti::detail::static_typed_number<_TAG,ddk::static_counter<decltype(ddk::mpl::resolve_holder<_TAG>())>::get_next_count()>&);


namespace ddk
{
namespace mpl
{

template<typename,typename,template<typename,typename>typename,typename>
struct iterable_action_tag_retraits;

template<typename OldTraits, typename NewTraits,template<typename,typename> typename Projection, typename Tag>
struct iterable_action_tag_retraits
{
    typedef Tag type;
};

template<typename OldTraits,typename NewTraits,template<typename,typename> typename Projection,typename T>
struct iterable_action_tag_retraits<OldTraits,NewTraits,Projection,sink_action_tag<function<void(T)>>>
{
private:
    template<typename Traits>
    using which_sink_action = sink_action_tag<function<void(typename mpl::which_type<mpl::is_const<T>,typename Traits::const_reference,typename Traits::reference>::type)>>;
    template<typename Traits>
    static constexpr bool is_convertible = mpl::which_type<mpl::is_const<T>,std::is_convertible<T,typename Traits::const_reference>,std::is_convertible<T,typename Traits::reference>>::type::value;

public:
    typedef typename std::enable_if<is_convertible<Projection<OldTraits,NewTraits>>,which_sink_action<NewTraits>>::type type;
};

template<typename OldTraits,typename NewTraits,template<typename,typename> typename Projection,typename T>
struct iterable_action_tag_retraits<OldTraits,NewTraits,Projection,add_action_tag<T>>
{
private:
    template<typename Traits>
    static constexpr bool is_constructible = std::is_constructible<T,typename Traits::value_type>::value;

public:
    typedef typename static_if<is_constructible<Projection<OldTraits,NewTraits>>,add_action_tag<typename NewTraits::value_type>,void_action_tag>::type type;
};

template<typename,typename,template<typename,typename>typename,typename>
struct action_tags_retrait;

template<typename OldTraits,typename NewTraits,template<typename,typename> typename Projection,typename...Tags>
struct action_tags_retrait<OldTraits,NewTraits,Projection,type_pack<Tags...>>
{
    typedef typename type_pack<typename iterable_action_tag_retraits<OldTraits,NewTraits,Projection,Tags>::type ...>::template drop<void_action_tag>::type type;
};

template<typename>
struct is_sink_action_tag_impl;

template<typename Sink>
struct is_sink_action_tag_impl<sink_action_tag<Sink>> : public std::true_type
{};
template<typename T>
struct is_sink_action_tag_impl : public std::true_type
{};

template<typename T>
constexpr inline bool is_sink_action_tag = is_sink_action_tag_impl<T>::value;

}
}