#pragma once

#define FIXED_SIZE_CONTAINER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_container_fixed_size<_TYPE>::value>::type

#define FIXED_SIZE_CONTAINER_COND(_TYPE) \
	ddk::concepts::is_container_fixed_size<_TYPE>::value

#define DYNAMIC_SIZE_CONTAINER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_container_dynamic_size<_TYPE>::value>::type

#define DYNAMIC_SIZE_CONTAINER_COND(_TYPE) \
	ddk::concepts::is_container_dynamic_size<_TYPE>::value

#define IS_CONTAINER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_container<_TYPE>::value>::type

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

namespace ddk
{
namespace concepts
{

template<typename Container>
struct is_container
{
private:
	template<typename T>
	static std::true_type _check(T&,decltype(std::declval<T>().size())* = nullptr);
	template<typename T>
	static std::false_type _check(const T&,...);

public:
	static const bool value = decltype(_check(std::declval<Container&>(),nullptr))::value;
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

}
}
