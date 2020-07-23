#pragma once

#include "ddk_none.h"
#include "ddk_async_executor_interface.h"
#include "ddk_co_iterator_context.h"

#define IS_CO_ITERATOR(_CO_ITERATOR) \
template<typename TT> \
struct is_co_iterator; \
 \
template<typename TT> \
struct is_co_iterator<_CO_ITERATOR<TT>> \
{ \
	static const bool value = true; \
}; \
template<typename TT> \
struct is_co_iterator \
{ \
	static const bool value = false; \
};

#define ITERATOR_CATEGORY(_ITERABLE,_CATEGORY) \
namespace std \
{ \
 \
template<> \
struct iterator_traits<_ITERABLE> \
{ \
	typedef _CATEGORY iterator_category; \
}; \
 \
}

namespace ddk
{

template<typename T>
struct co_forward_iterator
{
	IS_CO_ITERATOR(co_forward_iterator)

	template<typename>
	friend class co_iterable;
	template<typename>
	friend struct co_forward_iterator;

public:
	static const size_t npos = -1;
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_forward_iterator(const detail::none_t&);
	co_forward_iterator(const co_forward_iterator& other);

	reference operator*();
	const_reference operator*() const;
	co_forward_iterator<T>& operator++();
	co_forward_iterator<T> operator++(int);
	co_forward_iterator<T>& operator=(const co_forward_iterator& other);
	bool operator!=(const co_forward_iterator<T>& other) const;
	bool operator==(const co_forward_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_forward_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value==false>::type* = nullptr);

	async_execute_shared_ptr<T> m_executor;
	std::function<reference(const co_forward_iterator_context&)> m_function;
	detail::co_forward_iterator_context_impl m_context;
	detail::this_fiber_t m_caller;
};

template<typename T>
struct co_bidirectional_iterator
{
	IS_CO_ITERATOR(co_bidirectional_iterator)

	template<typename>
	friend class co_iterable;
	template<typename>
	friend struct co_bidirectional_iterator;

public:
	static const size_t npos = -1;
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_bidirectional_iterator(const detail::none_t&);
	co_bidirectional_iterator(const co_bidirectional_iterator& other);

	reference operator*();
	const_reference operator*() const;
	co_bidirectional_iterator<T>& operator++();
	co_bidirectional_iterator<T> operator++(int);
	co_bidirectional_iterator<T>& operator--();
	co_bidirectional_iterator<T> operator--(int);
	co_bidirectional_iterator<T>& operator=(const co_bidirectional_iterator& other);
	bool operator!=(const co_bidirectional_iterator<T>& other) const;
	bool operator==(const co_bidirectional_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_bidirectional_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type* = nullptr);

	async_execute_shared_ptr<T> m_executor;
	std::function<reference(const co_forward_iterator_context&)> m_function;
	detail::co_bidirectional_iterator_context_impl m_context;
	detail::this_fiber_t m_caller;
};

template<typename T>
struct co_random_access_iterator
{
	IS_CO_ITERATOR(co_random_access_iterator)

	template<typename>
	friend class co_iterable;
	template<typename>
	friend struct co_random_access_iterator;

public:
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_random_access_iterator(const detail::none_t&);
	co_random_access_iterator(const co_random_access_iterator& other);

	reference operator*();
	const_reference operator*() const;
	co_random_access_iterator<T>& operator++();
	co_random_access_iterator<T> operator++(int);
	co_random_access_iterator<T>& operator--();
	co_random_access_iterator<T> operator--(int);
	co_random_access_iterator<T> operator+(int i_shift);
	co_random_access_iterator<T>& operator[](size_t i_absPos);
	co_random_access_iterator<T>& operator=(const co_random_access_iterator& other);
	bool operator!=(const co_random_access_iterator<T>& other) const;
	bool operator==(const co_random_access_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_random_access_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type* = nullptr);

	async_execute_shared_ptr<T> m_executor;
	std::function<reference(const co_random_access_iterator_context&)> m_function;
	detail::co_random_access_iterator_context_impl m_context;
	detail::this_fiber_t m_caller;
};

namespace detail
{

template<typename,typename>
struct iterator_type_correspondence;

template<typename T>
struct iterator_type_correspondence<T,std::forward_iterator_tag>
{
	typedef co_forward_iterator<T> type;
};
template<typename T>
struct iterator_type_correspondence<T, std::random_access_iterator_tag>
{
	typedef co_random_access_iterator<T> type;
};

}
}

#include "ddk_co_iterator.inl"
