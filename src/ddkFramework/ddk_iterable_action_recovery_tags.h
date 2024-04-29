//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_action_tags.h"

namespace ddk
{

template<typename,typename>
struct iterable_action_tag_type;

template<typename Traits, typename ActionTag>
struct iterable_action_tag_type
{
	typedef ActionTag recovery_type;
	typedef void return_type;
};
template<typename Traits>
struct iterable_action_tag_type<Traits,begin_action_tag>
{
	typedef begin_action_tag recovery_type;
	typedef typename Traits::reference return_type;
};
template<typename Traits>
struct iterable_action_tag_type<Traits,forward_action_tag>
{
	typedef forward_action_tag recovery_type;
	typedef typename Traits::reference return_type;
};
template<typename Traits>
struct iterable_action_tag_type<Traits,backward_action_tag>
{
	typedef backward_action_tag recovery_type;
	typedef typename Traits::reference return_type;
};
template<typename Traits>
struct iterable_action_tag_type<Traits,displace_action_tag>
{
	typedef displace_action_tag recovery_type;
	typedef typename Traits::reference return_type;
};
template<typename Traits, typename T>
struct iterable_action_tag_type<Traits,add_action_tag<T>>
{
	typedef add_action_tag<T> recovery_type;
	typedef typename Traits::reference return_type;
};
template<typename Traits>
struct iterable_action_tag_type<Traits,size_action_tag>
{
	typedef size_action_tag recovery_type;
	typedef size_t return_type;
};
template<typename Traits,typename Sink>
struct iterable_action_tag_type<Traits,sink_action_tag<Sink>>
{
	typedef sink_action_tag<Sink> recovery_type;
	typedef typename Traits::reference return_type;
};

template<typename Traits, typename ActionTag>
using iterable_action_recovery_tag = typename iterable_action_tag_type<mpl::remove_qualifiers<Traits>,mpl::remove_qualifiers<ActionTag>>::recovery_type;

template<typename Traits, typename ActionTag>
using iterable_action_return_type = typename iterable_action_tag_type<mpl::remove_qualifiers<Traits>,mpl::remove_qualifiers<ActionTag>>::return_type;

}