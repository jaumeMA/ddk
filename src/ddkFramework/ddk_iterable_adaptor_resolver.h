//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_iterable_action_tag_resolver.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
using iterable_adaptor_resolver = detail::iterable_adaptor_actions<Iterable,detail::iterable_action_tag_resolver<Iterable>>;

template<typename Iterable>
typename iterable_adaptor<Iterable>::traits deduce_iterable_traits(const iterable_adaptor<Iterable>&);

}

template<typename Iterable>
using deduced_adaptor = mpl::remove_qualifiers<decltype(deduce_adaptor(std::declval<Iterable&>()))>;

template<typename Iterable>
struct iterable_adaptor : detail::iterable_adaptor_resolver<Iterable>
{
private:
    typedef detail::iterable_adaptor_resolver<Iterable> base_t;

public:
    using typename base_t::traits;
    using typename base_t::const_traits;
    using typename base_t::tags_t;
    using typename base_t::const_tags_t;
    using base_t::base_t;
};

template<typename Iterable>
struct iterable_adaptor<detail::iterable<Iterable>> : deduced_adaptor<Iterable>
{
    typedef deduced_adaptor<Iterable> base_t;

    using base_t::base_t;
};

template<typename Iterable>
struct iterable_adaptor<const detail::iterable<Iterable>> : deduced_adaptor<Iterable>
{
    typedef deduced_adaptor<Iterable> base_t;

    using base_t::base_t;
};

}