#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Iterable,typename ActionTag>
inline iterable_action_tag_result<Iterable,ActionTag> operator&&(const iterable_action_tag_result<Iterable,ActionTag>& i_lhs,const iterable_action_tag_result<Iterable,ActionTag>& i_rhs);

template<typename Result, typename Traits,typename ActionTag>
inline auto forward_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error);

}

#include "ddk_iterable_action_tag_ops.inl"