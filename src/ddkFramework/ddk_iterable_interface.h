#pragma once

#include "ddk_iterable_action_defs.h"
#include "ddk_iterable_action_state.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_base
{
public:
    typedef Traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;
    typedef supported_action<traits> action;
    typedef mpl::terse_callable<function<action(reference)>> terse_endpoint;
    typedef mpl::terse_callable<function<action(const_reference)>> const_terse_endpoint;

    virtual ~iterable_base() = default;
    virtual action_result iterate(terse_endpoint i_try,const action& i_initialAction) = 0;
    virtual action_result iterate(const_terse_endpoint i_try,const action& i_initialAction) const = 0;
};

}
}