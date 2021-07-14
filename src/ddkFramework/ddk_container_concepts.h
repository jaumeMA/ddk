#pragma once

#define IS_SIZEABLE_COND(_TYPE) \
	ddk::concepts::is_sizable<_TYPE>::value

#define IS_SIZEABLE(_TYPE) \
	typename std::enable_if<IS_SIZEABLE_COND(_TYPE)>::type

#define IS_DIMENSIONABLE_COND(_TYPE) \
	ddk::concepts::is_dimensionable<_TYPE>::value

#define IS_DIMENSIONABLE(_TYPE) \
	typename std::enable_if<IS_DIMENSIONABLE_COND(_TYPE)>::type

#define FIXED_SIZE_CONTAINER_COND(_TYPE) \
	ddk::concepts::is_container_fixed_size<_TYPE>::value

#define FIXED_SIZE_CONTAINER(_TYPE) \
	typename std::enable_if<FIXED_SIZE_CONTAINER_COND(_TYPE)>::type

#define DYNAMIC_SIZE_CONTAINER_COND(_TYPE) \
	ddk::concepts::is_container_dynamic_size<_TYPE>::value

#define DYNAMIC_SIZE_CONTAINER(_TYPE) \
	typename std::enable_if<DYNAMIC_SIZE_CONTAINER_COND(_TYPE)>::type

#define IS_CONTAINER_COND(_TYPE) \
	ddk::concepts::is_container<typename std::remove_reference<_TYPE>::type>::value

#define IS_CONTAINER(_TYPE) \
	typename std::enable_if<IS_CONTAINER_COND(_TYPE)>::type

#define ASSIGNABLE_BY_INDEX_ACCESS(_TYPE,_VALUE) \
	decltype(std::declval<_TYPE>().at(0) = std::declval<_VALUE>())

#define ASSIGNABLE_BY_TYPE_ACCESS(_TYPE,_VALUE) \
	decltype(std::declval<_TYPE>().template set<0>(std::declval<_VALUE>()))

#define ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_INDEX_ACCESS(_TYPE,_VALUE) \
	FIXED_SIZE_CONTAINER(_TYPE),ASSIGNABLE_BY_INDEX_ACCESS(_TYPE,_VALUE)

#define ASSIGNABLE_DYNAMIC_CONTAINER_BY_INDEX_ACCESS(_TYPE,_VALUE) \
	DYNAMIC_SIZE_CONTAINER(_TYPE),ASSIGNABLE_BY_INDEX_ACCESS(_TYPE,_VALUE)

#define ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_TYPE_ACCESS(_TYPE,_VALUE) \
	FIXED_SIZE_CONTAINER(_TYPE),ASSIGNABLE_BY_TYPE_ACCESS(_TYPE,_VALUE)

#define IS_INDEXED_COND(_TYPE) \
    ddk::concepts::is_indexed_container<_TYPE>

#define IS_INDEXED(_TYPE) \
    typename std::enable_if<IS_INDEXED_COND(_TYPE)>::type


namespace ddk
{
namespace concepts
{

template<typename Container>
struct is_sizable
{
private:
	template<typename T>
	static std::true_type sized_type(T&,decltype(std::declval<T>().size())* = nullptr,decltype(std::declval<T>().empty())* = nullptr);
	template<typename T>
	static std::false_type sized_type(const T&,...);

public:
	static const bool value = decltype(sized_type(std::declval<Container&>(),nullptr))::value;
};

template<typename Container>
struct is_dimensionable
{
private:
	template<typename T>
	static std::true_type dimensioned_type(T&,decltype(std::declval<T>().dimension())* = nullptr);
	template<template<typename,size_t...> typename T,typename R, size_t ... Dims>
	static std::true_type dimensioned_type(const T<R,Dims...>&);
	template<typename T>
	static std::false_type dimensioned_type(const T&,...);

public:
	static const bool value = decltype(dimensioned_type(std::declval<Container&>()))::value;
};

template<typename Container>
struct is_container
{
private:
	template<typename T>
	static std::true_type random_access_type(T&,decltype(std::declval<T>()[0])* = nullptr);
	template<typename T>
	static std::false_type random_access_type(const T&,...);
	template<typename T>
	static std::true_type relative_access_type(T&,decltype(std::declval<T>().front())* = nullptr);
	template<typename T>
	static std::false_type relative_access_type(const T&,...);
	template<typename T>
	static std::true_type iterable_access_type(T&,decltype(std::declval<T>().begin())* = nullptr,decltype(std::declval<T>().end())* = nullptr);
	template<typename T>
	static std::false_type iterable_access_type(const T&,...);

public:
	static const bool value = is_sizable<Container>::value &&
							  (decltype(random_access_type(std::declval<Container&>(),nullptr))::value ||
							   decltype(relative_access_type(std::declval<Container&>(),nullptr))::value ||
							   decltype(iterable_access_type(std::declval<Container&>(),nullptr))::value) &&
							  std::is_default_constructible<Container>::value;
};

template<typename Container>
struct is_container_dynamic_size
{
private:
	template<typename T>
	static std::true_type _check(const T&,decltype(std::declval<T>().insert(std::declval<typename T::iterator>(),std::declval<typename T::value_type>()))*);
	template<typename T>
	static std::false_type _check(const T&, ...);

public:
	static const bool value = is_container<Container>::value && decltype(_check(std::declval<Container>(),nullptr))::value;
};

template<typename Container>
struct is_container_fixed_size
{
	static const bool value = is_container<Container>::value && (is_container_dynamic_size<Container>::value == false);
};

template<typename T>
std::false_type is_indexed_container_impl(const T&,...);
template<typename T,typename = typename T::place_type,size_t = T::num_places>
std::true_type is_indexed_container_impl(T&);

template<typename T>
inline constexpr bool is_indexed_container = decltype(is_indexed_container_impl(std::declval<T&>()))::value;

}
}
