#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"

#define IS_SHARED_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_shared_reference_v<_TYPE>

#define IS_SHARED_REFERENCE(_TYPE) \
	typename std::enable_if<IS_SHARED_REFERENCE_COND(_TYPE)>::type*

#define IS_NOT_SHARED_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_shared_reference_v<_TYPE> == false

#define IS_NOT_SHARED_REFERENCE(_TYPE) \
	typename std::enable_if<IS_NOT_SHARED_REFERENCE_COND(_TYPE)>::type*

#define IS_SHARED_POINTER_COND(_TYPE) \
	ddk::concepts::is_shared_pointer_v<_TYPE>

#define IS_SHARED_POINTER(_TYPE) \
	typename std::enable_if<IS_SHARED_POINTER_COND(_TYPE)>::type*

#define IS_NOT_SHARED_POINTER_COND(_TYPE) \
		ddk::concepts::is_shared_pointer_v<_TYPE> == false

#define IS_NOT_SHARED_POINTER(_TYPE) \
		typename std::enable_if<IS_NOT_SHARED_POINTER_COND(_TYPE)>::type*

#define IS_DISTRIBUTED_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_distributed_reference_v<_TYPE>

#define IS_DISTRIBUTED_REFERENCE(_TYPE) \
	typename std::enable_if<IS_DISTRIBUTED_REFERENCE_COND(_TYPE)>::type*

#define IS_NOT_DISTRIBUTED_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_distributed_reference_v<_TYPE> == false

#define IS_NOT_DISTRIBUTED_REFERENCE(_TYPE) \
	typename std::enable_if<IS_NOT_DISTRIBUTED_REFERENCE_COND(_TYPE)>::type*

#define IS_DISTRIBUTED_POINTER_COND(_TYPE) \
	ddk::concepts::is_distributed_pointer_v<_TYPE>

#define IS_DISTRIBUTED_POINTER(_TYPE) \
	typename std::enable_if<IS_DISTRIBUTED_POINTER_COND(_TYPE)>::type*

#define IS_NOT_DISTRIBUTED_POINTER_COND(_TYPE) \
		ddk::concepts::is_distributed_pointer_v<_TYPE> == false

#define IS_NOT_DISTRIBUTED_POINTER(_TYPE) \
		typename std::enable_if<IS_NOT_DISTRIBUTED_POINTER_COND(_TYPE)>::type*

#define IS_UNIQUE_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_unique_reference_v<_TYPE>

#define IS_UNIQUE_REFERENCE(_TYPE) \
	typename std::enable_if<IS_UNIQUE_REFERENCE_COND(_TYPE)>::type*

#define IS_NOT_UNIQUE_REFERENCE_COND(_TYPE) \
	ddk::concepts::is_unique_reference_v<_TYPE> == false

#define IS_NOT_UNIQUE_REFERENCE(_TYPE) \
	typename std::enable_if<IS_NOT_UNIQUE_REFERENCE_COND(_TYPE)>::type*

#define IS_UNIQUE_POINTER_COND(_TYPE) \
	ddk::concepts::is_unique_pointer_v<_TYPE>

#define IS_UNIQUE_POINTER(_TYPE) \
	typename std::enable_if<IS_UNIQUE_POINTER_COND(_TYPE)>::type*

#define IS_NOT_UNIQUE_POINTER_COND(_TYPE) \
	ddk::concepts::is_unique_pointer_v<_TYPE> == false

#define IS_NOT_UNIQUE_POINTER(_TYPE) \
	typename std::enable_if<IS_NOT_UNIQUE_POINTER_COND(_TYPE)>::type*

#define IS_LENDABLE_COND(_TYPE) \
	ddk::concepts::is_lendable_v<_TYPE>

#define IS_LENDABLE(_TYPE) \
	typename std::enable_if<IS_LENDABLE_COND(_TYPE)>::type*

namespace ddk
{
namespace concepts
{

template<typename>
struct is_shared_reference;

template<typename T>
struct is_shared_reference<shared_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_shared_reference
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_shared_reference_v = is_shared_reference<T>::value;

template<typename>
struct is_shared_pointer;

template<typename T>
struct is_shared_pointer<shared_pointer_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_shared_pointer
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_shared_pointer_v = is_shared_pointer<T>::value;

template<typename>
struct is_distributed_reference;

template<typename T>
struct is_distributed_reference<distributed_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_distributed_reference
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_distributed_reference_v = is_distributed_reference<T>::value;

template<typename>
struct is_distributed_pointer;

template<typename T>
struct is_distributed_pointer<distributed_pointer_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_distributed_pointer
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_distributed_pointer_v = is_distributed_pointer<T>::value;

template<typename>
struct is_unique_reference;

template<typename T>
struct is_unique_reference<shared_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_unique_reference
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_unique_reference_v = is_unique_reference<T>::value;

template<typename>
struct is_unique_pointer;

template<typename T>
struct is_unique_pointer<unique_pointer_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_unique_pointer
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_unique_pointer_v = is_unique_pointer<T>::value;


template<typename T>
struct is_lendable
{
private:
	template<typename TT>
	static constexpr std::true_type test(TT&&, decltype(lend(std::declval<TT>()))* = nullptr);
	static constexpr std::false_type test(...);

public:
	static const bool value = decltype(test(std::declval<T>()))::value;
};

template<typename T>
constexpr bool is_lendable_v = is_lendable<T>::value;

template<typename T>
struct _is_lendable_pointer;

template<typename T>
struct _is_lendable_pointer<lent_pointer_wrapper<T>>
{
	static const bool value = true;
};

#ifdef DDK_DEBUG

template<typename T>
struct _is_lendable_pointer<lent_reference_wrapper<T>>
{
	static const bool value = true;
};

#endif

template<typename T>
struct _is_lendable_pointer
{
	static const bool value = false;
};


template<typename T>
struct is_lendable_pointer
{
private:
	template<typename TT>
	static constexpr decltype(lend(std::declval<TT>())) test(TT&&);
	static constexpr std::false_type test(...);

public:
	static const bool value = is_lendable_v<T> && _is_lendable_pointer<decltype(lend(std::declval<T>()))>::value;
};

template<typename T>
constexpr bool is_lendable_pointer_v = is_lendable_pointer<T>::value;

template<typename T>
struct _is_lendable_reference;

template<typename T>
struct _is_lendable_reference<lent_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename T>
struct _is_lendable_reference
{
	static const bool value = false;
};


template<typename T>
struct is_lendable_reference
{
private:
	template<typename TT>
	static constexpr decltype(lend(std::declval<TT>())) test(TT&&);
	static constexpr std::false_type test(...);

public:
	static const bool value = is_lendable_v<T> && _is_lendable_reference<decltype(lend(std::declval<T>()))>::value;
};

template<typename T>
constexpr bool is_lendable_reference_v = is_lendable_reference<T>::value;

}
}
