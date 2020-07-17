#pragma once

#include "ddk_awaitable.h"
#include "ddk_template_helper.h"

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
	typedef typename async_state_interface<T>::reference reference;
	typedef typename async_state_interface<T>::const_reference const_reference;
	typedef typename async_state_interface<T>::value_type value_type;

	co_iterator(const detail::none_t&);

	reference operator*();
	const_reference operator*() const;
	co_iterator<T>& operator++();
	bool operator!=(const co_iterator<T>& other) const;
	bool operator==(const co_iterator<T>& other) const;

private:
	template<typename TT>
	co_iterator(const co_iterator<TT>& other);
	co_iterator(const awaitable<T,co_iterator<T>>& i_awaitable);

	async_execute_shared_ptr<T> m_executor;
};

template<typename Iterable>
class co_iterable
{
	typedef typename mpl::which_type<std::is_const<Iterable>::value, typename Iterable::const_reference, typename Iterable::reference>::type reference;
	typedef typename Iterable::const_reference const_reference;
	typedef awaitable<reference, co_iterator<reference>> awaitable_type;

	friend inline co_iterator<reference> begin(co_iterable& i_co_iterable)
	{
		typename awaitable_type::continue_result res = i_co_iterable.m_impl.resume();

		return (res.hasError() == false) ? i_co_iterable.m_impl : co_iterator<reference>(none);
	}
	friend inline co_iterator<reference> end(co_iterable&)
	{
		return none;
	}
	friend inline co_iterator<const_reference> begin(const co_iterable& i_co_iterable)
	{
		typename awaitable_type::continue_result res = i_co_iterable.m_impl.resume();

		return (res.hasError() == false) ? i_co_iterable.m_impl : co_iterator<const_reference>(none);
	}
	friend inline co_iterator<const_reference> end(const co_iterable&)
	{
		return none;
	}
public:
	typedef co_iterator<reference> iterator;
	typedef co_iterator<const_reference> const_iterator;

	template<typename ... Types, typename ... Args>
	co_iterable(const std::function<reference(Types...)>& i_function, Args&& ... i_args);

	inline iterator begin();
	inline iterator end();
	inline const_iterator begin() const;
	inline const_iterator end() const;

private:
	mutable awaitable_type m_impl;
};

template<typename Iterable>
typename Iterable::reference forward_iterator_awaitable(Iterable& i_iterable);
template<typename Iterable>
typename Iterable::reference backward_iterator_awaitable(Iterable& i_iterable);

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable);
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(Iterable& i_iterable);
template<typename Iterable>
co_iterable<const Iterable> co_iterate(const Iterable& i_iterable);
template<typename Iterable>
co_iterable<const Iterable> co_reverse_iterate(const Iterable& i_iterable);

}

#include "ddk_co_iterable.inl"
