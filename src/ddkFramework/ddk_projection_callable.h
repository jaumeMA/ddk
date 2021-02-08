#pragma once

#include "ddk_template_helper.h"

namespace ddk
{

template<size_t Component>
struct projection_callable
{
public:
    projection_callable() = default;

    template<typename ... Args>
    inline mpl::nth_type_of_t<Component,Args...> operator()(Args&& ... i_args) const;
};

template<size_t Component>
const projection_callable<Component> projection = projection_callable<Component>{};

}

#include "ddk_projection_callable.inl"