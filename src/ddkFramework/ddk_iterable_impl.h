//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Traits, typename Iterable>
class iterable_impl : public iterable_impl_interface<Traits>, public iterable_visitor<Iterable>
{
public:
    typedef Traits traits;
    typedef const_iterable_traits<Traits> const_traits;
    typedef typename traits::tags_t tags_t;
    typedef typename traits::const_tags_t const_tags_t;

    iterable_impl(Iterable& i_iterable);

    template<typename Action>
    void iterate_impl(const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const Action& i_initialAction) const;
};

}
}

#include "ddk_iterable_impl.inl"