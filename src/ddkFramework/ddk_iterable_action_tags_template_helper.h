//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_template_helper.h"
#include "ddk_function_arguments_template_helper.h"
#include "ddk_iterator_concepts.h"
#include "ddk_static_type_list.h"

#define PUBLISH_ACTION_TAG(_ACTION,_TAG) \
    ADD_STATIC_TYPE(_ACTION,_TAG)

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
    typedef typename which_sink_action<NewTraits> type;
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
struct is_sink_action_tag_impl : public std::false_type
{};

template<typename T>
constexpr inline bool is_sink_action_tag = is_sink_action_tag_impl<mpl::remove_qualifiers<T>>::value;

struct iterable_action_tag;

template<typename>
struct iterator_action_support_impl;

template<>
struct iterator_action_support_impl<forward_action_tag>
{
    PUBLISH_ACTION_TAG(forward_action_tag,iterable_action_tag)
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(std::declval<T>()++)*);
    static std::false_type resolve(...);

public:
    template<typename Iterator>
    using type = typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<forward_action_tag>,mpl::empty_type_pack>::type;
};

template<>
struct iterator_action_support_impl<backward_action_tag>
{
    PUBLISH_ACTION_TAG(backward_action_tag,iterable_action_tag)
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(std::declval<T>()--)*);
    static std::false_type resolve(...);

public:
    template<typename Iterator>
    using type = typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<backward_action_tag>,mpl::empty_type_pack>::type;
};

template<>
struct iterator_action_support_impl<displace_action_tag>
{
    PUBLISH_ACTION_TAG(displace_action_tag,iterable_action_tag)
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(std::declval<T>() + 0)*);
    static std::false_type resolve(...);

public:
    template<typename Iterator>
    using type = typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<displace_action_tag>,mpl::empty_type_pack>::type;
};

template<>
struct iterator_action_support_impl<mpl::template_class_holder<agnostic_sink_action_tag>>
{
    PUBLISH_ACTION_TAG(mpl::template_class_holder<agnostic_sink_action_tag>,iterable_action_tag)
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(&*std::declval<T>())*);
    static std::false_type resolve(...);

public:
    template<typename Iterator>
    using type = typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<agnostic_sink_action_tag<typename Iterator::reference>>,mpl::empty_type_pack>::type;
};

template<typename Iterator,typename Action>
using iterator_action_support = typename iterator_action_support_impl<Action>::template type<Iterator>;

template<typename>
struct iterable_action_support_impl;

template<>
struct iterable_action_support_impl<begin_action_tag>
{
    PUBLISH_ACTION_TAG(begin_action_tag,iterable_action_tag)
private:
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static mpl::type_pack<begin_action_tag> resolve(T&,const decltype(std::declval<T>().begin())*);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>(),nullptr));
};

template<>
struct iterable_action_support_impl<end_action_tag>
{
    PUBLISH_ACTION_TAG(end_action_tag,iterable_action_tag)
private:
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static mpl::type_pack<end_action_tag> resolve(T&,const decltype(std::declval<T>().end())*);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>(),nullptr));
};

template<>
struct iterable_action_support_impl<remove_action_tag>
{
    PUBLISH_ACTION_TAG(remove_action_tag,iterable_action_tag)
private:
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static mpl::type_pack<remove_action_tag> resolve(T&,const decltype(std::declval<T&>().erase(std::declval<typename T::const_iterator>()))*);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>(),nullptr));
};

template<>
struct iterable_action_support_impl<mpl::template_class_holder<add_action_tag>>
{
    PUBLISH_ACTION_TAG(mpl::template_class_holder<add_action_tag>,iterable_action_tag)
private:
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static mpl::type_pack<add_action_tag<typename T::value_type>> resolve(T&,const decltype(std::declval<T&>().insert(std::declval<typename T::const_iterator>(),std::declval<typename T::value_type>()))*);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>(),nullptr));
};

template<>
struct iterable_action_support_impl<size_action_tag>
{
    PUBLISH_ACTION_TAG(size_action_tag,iterable_action_tag)
private:
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static mpl::type_pack<size_action_tag> resolve(T&,const decltype(std::declval<T>().size())*);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>(),nullptr));
};

template<typename Action>
struct iterable_action_support_impl
{
    template<typename T,typename = typename std::enable_if<concepts::has_iterator_defined_v<T>>::type>
    static iterator_action_support<typename mpl::which_type<mpl::is_const<T>,typename T::const_iterator,typename T::iterator>::type,Action> resolve(T&);
    static mpl::empty_type_pack resolve(...);

public:
    template<typename Iterable>
    using type = decltype(resolve(std::declval<Iterable&>()));
};

template<typename Iterable,typename ... Actions>
using iterable_action_support = typename mpl::merge_type_packs<typename iterable_action_support_impl<Actions>::template type<Iterable> ...>::type;

}
}