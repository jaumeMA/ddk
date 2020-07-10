#pragma once

#include "ewas_awaitable.h"

namespace ewas
{

template<typename T>
struct co_iterator
{
public:
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_iterator(async_execute_shared_ptr<T> i_executor);
	co_iterator(const detail::none_t&);

	reference operator*();
	const_reference operator*() const;
	co_iterator<T>& operator++();
	bool operator!=(const co_iterator<T>& other) const;
	bool operator==(const co_iterator<T>& other) const;

private:
	async_execute_shared_ptr<T> m_executor;
};

template<typename Iterable>
using co_iterable = awaitable<typename Iterable::reference,co_iterator<typename Iterable::reference>>;

template<typename Iterable>
typename Iterable::reference forward_iterator_awaitable(Iterable& i_iterable);
template<typename Iterable>
typename Iterable::reference backward_iterator_awaitable(Iterable& i_iterable);

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable);
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(Iterable& i_iterable);

}

#include "ewas_co_iterable.inl"
