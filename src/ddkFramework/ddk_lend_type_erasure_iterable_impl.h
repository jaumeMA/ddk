//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_defs.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class lend_type_erasure_iterable_impl : public iterable_impl_interface<Traits>
{
public:
    typedef Traits traits;

    lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_interface<Traits>>& i_iterable);

    template<typename Action>
    void iterate_impl(const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const Action& i_initialAction) const;

private:
    lent_reference_wrapper<iterable_interface<Traits>> m_iterable;
};
template<typename Traits>
lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_interface<Traits>>&) -> lend_type_erasure_iterable_impl<Traits>;

}
}

#include "ddk_lend_type_erasure_iterable_impl.inl"