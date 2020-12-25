#pragma once

#include "ddk_function_template_helper.h"
#include "ddk_system_allocator.h"

#define IS_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::concepts::is_function<_TYPE>::value>::type*

#define IS_NOT_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::concepts::is_function<_TYPE>::value==false>::type*

#define IS_CALLABLE(_TYPE) \
	typename ddk::concepts::require_callables<_TYPE>::type

#define IS_CALLABLE_NOT_FUNCTION(_TYPE) \
	IS_CALLABLE(_TYPE),IS_NOT_FUNCTION(_TYPE)

#define ARE_CALLABLES(...) \
	typename ddk::concepts::require_callables<__VA_ARGS__>::type

namespace ddk
{

template<typename,typename = system_allocator>
class function;

namespace concepts
{

template<typename ... Callables>
struct require_callables
{
	typedef mpl::type_pack<typename std::enable_if<mpl::is_valid_functor<Callables>::value>::type ...> type;
};

template<typename>
struct _is_function;

template<typename T>
struct _is_function: public std::false_type
{
};
template<typename Return,typename ... Types,typename Allocator>
struct _is_function<function<Return(Types...),Allocator>> : public std::true_type
{
};

template<typename T>
struct is_function
{
private:
	typedef typename std::remove_const<typename std::remove_reference<T>::type>::type raw_type;

public:
	static const bool value = _is_function<raw_type>::value;
};

}
}