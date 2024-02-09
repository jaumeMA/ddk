
#include "ddk_union_iterable_action_conversion_sequence.h"

namespace ddk
{

template<typename Traits,typename ActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error)
: base_t(union_iterable_action_cs<typename recovery_tag::action_tag>(i_error.recovery()))
{
}
template<typename Traits,typename ActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(iterable_action_tag_error<Traits,ActionTag>&& i_error)
: base_t(union_iterable_action_cs<typename recovery_tag::action_tag>(std::move(i_error).recovery()))
{
}
template<typename Traits,typename ActionTag>
template<typename TTraits,typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(const iterable_action_tag_error<TTraits,AActionTag>& i_error)
: base_t({ union_iterable_action_cs<typename recovery_tag::action_tag>(i_error.recovery()) })
{
}
template<typename Traits,typename ActionTag>
template<typename TTraits,typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(iterable_action_tag_error<TTraits,AActionTag>&& i_error)
: base_t({ union_iterable_action_cs<typename recovery_tag::action_tag>(std::move(i_error).recovery()) })
{
}
template<typename Traits,typename ActionTag>
template<typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(const union_iterable_action_error<Traits,AActionTag>& i_error)
: base_t({ union_iterable_action_cs<typename recovery_tag::action_tag>(i_error.recovery().action()) })
{
}
template<typename Traits,typename ActionTag>
template<typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::union_iterable_action_error(union_iterable_action_error<Traits,AActionTag>&& i_error)
: base_t({ union_iterable_action_cs<typename recovery_tag::action_tag>(std::move(i_error).recovery().action()) })
{
}
template<typename Traits,typename ActionTag>
template<typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::operator iterable_action_tag_error<Traits,AActionTag>() const &
{
    typedef iterable_action_recovery_tag<Traits,AActionTag> action_recovery_tag;

    return union_iterable_action_cs<action_recovery_tag>(recovery().action());
}
template<typename Traits,typename ActionTag>
template<typename AActionTag>
union_iterable_action_error<Traits,ActionTag>::operator iterable_action_tag_error<Traits,AActionTag>() &&
{
    typedef iterable_action_recovery_tag<Traits,AActionTag> action_recovery_tag;

    return union_iterable_action_cs<action_recovery_tag>(std::move(*this).recovery().action());
}

}