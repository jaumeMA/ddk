#pragma once

namespace ddk
{

template<typename T>
struct constant_function
{
public:
    constant_function(T i_value);

    template<typename ... Args>
    T operator()(Args&& ... i_args) const;

private:
    T m_value;
};

}
