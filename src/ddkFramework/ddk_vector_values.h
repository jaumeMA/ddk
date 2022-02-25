#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <vector>

namespace ddk
{

template<typename T>
class values_vector : public std::vector<T>
{
public:
    using std::vector<T>::vector;
};

TEMPLATE(typename T, typename ... Args)
REQUIRES(IS_CONSTRUCTIBLE(T,Args)...)
inline values_vector<T> make_values_vector(Args&& ... i_args);

}

#include "ddk_vector_values.inl"