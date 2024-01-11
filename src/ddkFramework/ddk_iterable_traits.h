#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_function_arguments_template_helper.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

template<typename Value,typename Reference,typename ConstReference>
struct type_traits
{
    typedef Value value_type;
    typedef Reference reference;
    typedef ConstReference const_reference;
    typedef long long difference_type;
};

template<typename Traits>
using const_type_traits = type_traits<typename Traits::value_type,typename Traits::const_reference,typename Traits::const_reference>;

template<typename T>
using by_type_traits = type_traits<T,T&,const T&>;

template<typename T>
using by_value_traits = type_traits<T,T,T>;

template<typename Adaptor>
using by_adaptor_traits = type_traits<typename Adaptor::value_type,typename Adaptor::reference,typename Adaptor::const_reference>;

template<typename Traits,typename TTraits>
using map_type_traits = type_traits<typename TTraits::value_type,typename TTraits::reference,typename TTraits::const_reference>;

template<typename Traits,typename TTraits>
using reduce_type_traits = type_traits<typename Traits::value_type,typename Traits::reference,typename Traits::const_reference>;

template<typename Value, typename Reference, typename ConstReference, typename Tags, typename ConstTags>
struct iterable_by_type_traits : type_traits<Value,Reference,ConstReference>
{
    using typename type_traits<Value,Reference,ConstReference>::value_type;
    using typename type_traits<Value,Reference,ConstReference>::reference;
    using typename type_traits<Value,Reference,ConstReference>::const_reference;
    typedef Tags tags_t;
    typedef ConstTags const_tags_t;
};

template<typename T, typename Tags, typename ConstTags>
using iterable_by_type_adaptor = iterable_by_type_traits<T,T&,const T&,Tags,ConstTags>;

template<typename T,typename Tags, typename ConstTags>
using iterable_by_value_adaptor = iterable_by_type_traits<T,T,T,Tags,ConstTags>;

template<typename Traits>
using iterable_traits = iterable_by_type_traits<typename Traits::value_type,typename Traits::reference,typename Traits::const_reference,typename Traits::tags_t,typename Traits::const_tags_t>;

template<typename Traits>
using const_iterable_traits = iterable_by_type_traits<typename Traits::value_type,typename Traits::const_reference,typename Traits::const_reference,typename Traits::const_tags_t,typename Traits::const_tags_t>;

template<typename Adaptor, typename Tags = typename Adaptor::tags_t, typename ConstTags = typename Adaptor::const_tags_t>
using iterable_adaptor_traits = iterable_by_type_traits<typename Adaptor::value_type,typename Adaptor::reference,typename Adaptor::const_reference,Tags,ConstTags>;

template<typename Adaptor>
using adaptor_traits = typename mpl::which_type<mpl::is_const<Adaptor>,typename mpl::remove_qualifiers<Adaptor>::const_traits,typename mpl::remove_qualifiers<Adaptor>::traits>::type;

template<typename Adaptor>
using adaptor_tags = typename mpl::which_type<mpl::is_const<Adaptor>,typename mpl::remove_qualifiers<Adaptor>::const_tags_t,typename mpl::remove_qualifiers<Adaptor>::tags_t>::type;

template<typename Trait, typename ... Traits>
struct union_iterable_traits_resolver
{
private:
    using union_type_traits = type_traits<typename std::common_type<typename Trait::value_type,typename Traits::value_type...>::type,
                                            typename std::common_type<typename Trait::reference,typename Traits::reference...>::type,
                                            typename std::common_type<typename Trait::const_reference,typename Traits::const_reference...>::type>;
    using union_iterable_traits = iterable_adaptor_traits<union_type_traits,
                                                            mpl::type_pack_intersection<typename mpl::action_tags_retrait<Trait,union_type_traits,map_type_traits,typename Trait::tags_t>::type,typename mpl::action_tags_retrait<Traits,union_type_traits,map_type_traits,typename Traits::tags_t>::type...>,
                                                            mpl::type_pack_intersection<typename mpl::action_tags_retrait<Trait,union_type_traits,map_type_traits,typename Trait::const_tags_t>::type,typename mpl::action_tags_retrait<Traits,union_type_traits,map_type_traits,typename Traits::const_tags_t>::type...>>;
public:
    typedef iterable_traits<union_iterable_traits> traits;
};

template<typename ... Traits>
using union_iterable_traits = typename union_iterable_traits_resolver<Traits ...>::traits;

template<typename Trait, typename ... Traits>
struct intersection_iterable_traits_resolver
{
private:
    using intersection_type_traits = type_traits<function_arguments<typename Trait::value_type,typename Traits::value_type...>,
                                                    function_arguments<typename Trait::reference,typename Traits::reference...>,
                                                    function_arguments<typename Trait::const_reference,typename Traits::const_reference...>>;
    using intersection_iterable_traits = iterable_adaptor_traits<intersection_type_traits,
                                                                mpl::type_pack_intersection<typename mpl::action_tags_retrait<Trait,intersection_type_traits,reduce_type_traits,typename Trait::tags_t>::type,typename mpl::action_tags_retrait<Traits,intersection_type_traits,reduce_type_traits,typename Traits::tags_t>::type...>,
                                                                mpl::type_pack_intersection<typename mpl::action_tags_retrait<Trait,intersection_type_traits,reduce_type_traits,typename Trait::const_tags_t>::type,typename mpl::action_tags_retrait<Traits,intersection_type_traits,reduce_type_traits,typename Traits::const_tags_t>::type...>>;

public:
    typedef iterable_traits<intersection_iterable_traits> traits;
};

template<typename ... Traits>
using intersection_iterable_traits = typename intersection_iterable_traits_resolver<Traits...>::traits;

}
}
