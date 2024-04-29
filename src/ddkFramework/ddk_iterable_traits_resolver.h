//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename>
struct iterable_adaptor;

namespace detail
{

template<typename Iterable>
typename iterable_adaptor<Iterable>::traits deduce_iterable_traits(const Iterable&, ...);

}

template<typename Iterable>
using deduced_iterable_traits = decltype(detail::deduce_iterable_traits(std::declval<mpl::remove_qualifiers<Iterable>>()));

}