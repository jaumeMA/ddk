//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable.h"
#include "ddk_iterable_interface.h"
#include "ddk_type_erasure_iterable_impl.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
class view_iterable : public iterable<Iterable>, public iterable_interface<typename Iterable::traits>
{
    typedef iterable_interface<typename Iterable::traits> base_t;

public:
    using typename base_t::traits;
    using typename base_t::const_traits;
    using iterable<Iterable>::iterable;

private:
    using typename base_t::action;
    using typename base_t::const_action;

    void iterate(const action& i_initialAction) override;
    void iterate(const const_action& i_initialAction) const override;
    iterable_adaptor<type_erasure_iterable_impl<traits>> deduce_owned_adaptor() override;
};
template<typename Iterable>
view_iterable(const Iterable&) -> view_iterable<Iterable>;
template<typename Iterable>
view_iterable(Iterable&&) -> view_iterable<Iterable>;

}
}

#include "ddk_view_iterable.inl"