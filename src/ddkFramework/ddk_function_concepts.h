//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_function_template_helper.h"

#define IS_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::mpl::is_function<_TYPE>>::type

#define IS_NOT_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::mpl::is_function<_TYPE> == false>::type

#define IS_CALLABLE(_TYPE, ...) \
	typename std::enable_if<ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value>::type

#define IS_CALLABLE_COND(_TYPE, ...) \
	ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value

#define IS_NOT_CALLABLE(_TYPE, ...) \
	typename std::enable_if<ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value==false>::type

#define IS_NOT_CALLABLE_COND(_TYPE, ...) \
	ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value==false

#define IS_CALLABLE_BY_COND(_TYPE, ...) \
	ddk::concepts::is_callable_by<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value

#define IS_CALLABLE_BY(_TYPE, ...) \
	typename std::enable_if<IS_CALLABLE_BY_COND(_TYPE,##__VA_ARGS__)>::type

#define IS_RETURN_TYPE_CONVERTIBLE_TO(_TYPE,_RETURN) \
	typename std::enable_if<std::is_convertible<typename ddk::mpl::aqcuire_callable_return_type<_TYPE>::type,_RETURN>::value>::type

#define IS_CALLABLE_NOT_FUNCTION(_TYPE,...) \
	IS_CALLABLE(_TYPE,##__VA_ARGS__),IS_NOT_FUNCTION(_TYPE)

namespace ddk
{
namespace concepts
{

template<typename T,typename ... Args>
struct is_callable_by
{
private:
	template<typename TT, typename = decltype(std::declval<TT>()(std::declval<Args>()...))>
	static std::true_type resolve(const TT&);
	static std::false_type resolve(...);

public:
	static const bool value = decltype(resolve(std::declval<T>()))::value;
};

}
}