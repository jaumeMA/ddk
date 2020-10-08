#pragma once

namespace ddk
{
namespace detail
{

template<typename,typename>
class composed_function;

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
class composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)>
{
public:
    composed_function(const function<ReturnDst(TypesDst...)>& i_lhs, const function<ReturnSrc(TypesSrc...)>& i_rhs);

    ReturnDst operator()(TypesSrc ... i_args) const;

private:
    const function<ReturnDst(TypesDst...)> m_lhs;
    const function<ReturnSrc(TypesSrc...)> m_rhs;
};

}
}

#include "ddk_composed_function.inl"
