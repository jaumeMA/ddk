//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

namespace ddk
{

template<typename T>
struct constant_callable
{
public:
    struct callable_tag;

    constant_callable() = default;
    template<typename ... Args>
    constant_callable(Args&& ... i_args);

    template<typename ... Args>
    T operator()(Args&& ... i_args) const;

private:
    T m_value;
};

}

#include "ddk_constant_callable.inl"