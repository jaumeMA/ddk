#pragma once

#include "ddk_iterable_state.h"

namespace ddk
{
namespace detail
{

class iterable_interface
{
public:
    virtual iter::iterable_state& get_state() = 0;
    virtual const iter::iterable_state& get_state() const = 0;

protected:
    virtual ~iterable_interface() = default;
};

}
}
