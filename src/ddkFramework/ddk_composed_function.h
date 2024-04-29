//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_function_impl.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
class composed_function;

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
class composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> : public inherited_functor_impl<ReturnDst,TypesSrc...>
{
public:
    composed_function(const function<ReturnDst(TypesDst...)>& i_lhs, const function<ReturnSrc(TypesSrc...)>& i_rhs);

    ReturnDst operator()(forwarded_arg<TypesSrc> ... i_args) const final override;

private:
    const function<ReturnDst(TypesDst...)> m_lhs;
    const function<ReturnSrc(TypesSrc...)> m_rhs;
};

}
}

#include "ddk_composed_function.inl"
