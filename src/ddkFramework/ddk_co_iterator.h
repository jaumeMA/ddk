#pragma once

#include "ddk_none.h"
#include "ddk_async_executor_interface.h"

namespace ddk
{

template<typename T>
struct co_iterator
{
	template<typename>
	friend class co_iterable;
	template<typename>
	friend struct co_iterator;

public:
	static const size_t npos = -1;
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_iterator(const detail::none_t&);
	co_iterator(const co_iterator& other);

	reference operator*();
	const_reference operator*() const;
	co_iterator<T>& operator++();
	co_iterator<T>& operator++(int);
	co_iterator<T>& operator=(const co_iterator& other);
	bool operator!=(const co_iterator<T>& other) const;
	bool operator==(const co_iterator<T>& other) const;

private:
	co_iterator(const std::function<reference(size_t)>& i_function);

	async_execute_shared_ptr<T> m_executor;
	std::function<reference(size_t)> m_function;
	size_t m_currIndex;
};

}

#include "ddk_co_iterator.inl"