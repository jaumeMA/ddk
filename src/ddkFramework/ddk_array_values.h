//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <array>

namespace ddk
{

template<typename T, size_t Dim>
class values_array : public std::array<T,Dim>
{
    typedef std::array<T,Dim> base_t;
    template<typename,size_t>
    friend class values_array;

public:
    using base_t::base_t;
    values_array(const base_t& other);
    values_array(base_t&& other);

    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(T,Args)...)
    auto append(Args&& ... i_args) const;
    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(T,Args)...)
    auto prepend(Args&& ... i_args) const;

private:
    template<size_t ... Indexs, typename ... Args>
    inline values_array<T,Dim + mpl::num_types<Args...>> construct(bool i_order, const mpl::sequence<Indexs...>&, Args&& ... i_args) const;
};

TEMPLATE(typename T,typename ... Args)
REQUIRES(IS_CONSTRUCTIBLE(T,Args)...)
inline values_array<T,mpl::num_types<Args...>> make_values_array(Args&& ... i_args);



}

#include "ddk_array_values.inl"