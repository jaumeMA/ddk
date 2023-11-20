#pragma once

#include "ddk_iterable_action.h"
#include "ddk_function_arguments.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

template<typename Value, typename Reference, typename ConstReference, typename ActionTags>
struct iterable_by_type_traits
{
    typedef Value value_type;
    typedef Reference reference;
    typedef ConstReference const_reference;
    typedef ActionTags tags_t;
    typedef long long difference_type;
};

template<typename T, typename ActionTags>
using iterable_by_type_adaptor = iterable_by_type_traits<T,T&,const T&,ActionTags>;

template<typename T,typename ActionTags>
using iterable_by_value_adaptor = iterable_by_type_traits<T,T,T,ActionTags>;

template<typename Adaptor>
using iterable_adaptor_traits = iterable_by_type_traits<typename Adaptor::value_type,typename Adaptor::reference,typename Adaptor::const_reference,typename Adaptor::tags_t>;

template<typename Traits>
struct iterable_traits
{
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::tags_t tags_t;

    template<typename TTraits>
	using as = iterable_traits<TTraits>;

    static go_to_begin_action default_action()
    {
        return {};
    }
};

template<typename Trait, typename ... Traits>
struct union_iterable_traits_resolver
{
private:
    struct union_iterable_adaptor
    {
        typedef typename std::common_type<typename Trait::value_type,typename Traits::value_type...>::type value_type;
        typedef typename std::common_type<typename Trait::reference,typename Traits::reference...>::type reference;
        typedef typename std::common_type<typename Trait::const_reference,typename Traits::const_reference...>::type const_reference;
        typedef mpl::type_pack_intersection<typename Trait::tags_t,typename Traits::tags_t...> tags_t;
    };

public:
    typedef iterable_traits<iterable_adaptor_traits<union_iterable_adaptor>> traits;
};

template<typename ... Traits>
using union_iterable_traits = typename union_iterable_traits_resolver<Traits ...>::traits;

template<typename Trait, typename ... Traits>
struct intersection_iterable_traits_resolver
{
private:
    struct intersection_iterable_adaptor
    {
        typedef function_arguments<typename Trait::value_type,typename Traits::value_type...> value_type;
        typedef function_arguments<typename Trait::reference,typename Traits::reference...> reference;
        typedef function_arguments<typename Trait::const_reference,typename Traits::const_reference... > const_reference;
        typedef mpl::type_pack_intersection<typename Trait::tags_t,typename Traits::tags_t...> tags_t;
    };

public:
    typedef iterable_traits<iterable_adaptor_traits<intersection_iterable_adaptor>> traits;
};

template<typename ... Traits>
using intersection_iterable_traits = typename intersection_iterable_traits_resolver<Traits...>::traits;

}
}
