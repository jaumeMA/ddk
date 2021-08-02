#pragma once

#include "ddk_lent_value.h"
#include "ddk_lent_object.h"
#include "ddk_distributed_value.h"
#include "ddk_distributed_object.h"
#include "ddk_unique_value.h"
#include "ddk_unique_object.h"

namespace ddk
{

template<typename T, typename ... Args>
inline distributed_value<T> make_distributed_value(Args&& ... i_args);

template<typename T,typename ... Args>
inline unique_value<T> make_unique_value(Args&& ... i_args);

template<typename T>
inline distributed_value<T> promote_to_value(const distributed_object<T>& i_obj);
template<typename T>
inline unique_value<T> promote_to_value(unique_object<T> i_obj);

template<typename T>
inline lent_object<T> lend(const distributed_object<T>& i_obj);
template<typename T>
inline lent_value<T> lend(const distributed_value<T>& i_obj);
template<typename T>
inline lent_object<T> lend(const unique_object<T>& i_obj);
template<typename T>
inline lent_value<T> lend(const unique_value<T>& i_obj);

}

#include "ddk_inherited_value.inl"
#include "ddk_inherited_value_concepts.h"
#include "ddk_dynamic_multivisitor.h"
