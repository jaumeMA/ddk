#pragma once

namespace ddk
{

template<size_t Component>
struct projection_callable
{
public:
    projection_callable() = default;

    template<typename ... Args>
    inline typename mpl::nth_type_of<Component,Args...>::type operator(Args&& ... i_args) const;
};

}
