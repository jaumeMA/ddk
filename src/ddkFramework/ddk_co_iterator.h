#pragma once

#include "ddk_none.h"
#include "ddk_await_executor.h"
#include "ddk_iterable_action.h"
#include "ddk_iterable_state.h"
#include "ddk_iterable_action_state.h"
#include "ddk_lendable.h"

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
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;

	co_forward_iterator(const detail::none_t&);
	co_forward_iterator(const co_forward_iterator& other);
	~co_forward_iterator();

	reference operator*();
	const_reference operator*() const;
	co_forward_iterator<T>& operator++();
	co_forward_iterator<T> operator++(int);
	bool operator!=(const co_forward_iterator<T>& other) const;
	bool operator==(const co_forward_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_forward_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value==false>::type* = nullptr);
	iter::go_forward_action&& acquire_iterable_value(reference i_value);

	ddk::function<reference(const iter::shift_action&,const function<iter::const_forward_action(reference)>&)> m_function;
	iter::iterable_state m_currState;
	detail::this_fiber_t m_caller;
	iter::go_forward_action m_currAction = iter::go_no_place;
	mutable typed_arena<reference> m_iteratorValueContainer;
	detail::await_executor<T> m_executor;
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
	typedef typename embedded_type<T>::reference reference;
	typedef typename embedded_type<T>::const_reference const_reference;

	co_bidirectional_iterator(const detail::none_t&);
	co_bidirectional_iterator(const co_bidirectional_iterator& other);
	~co_bidirectional_iterator();

	reference operator*();
	const_reference operator*() const;
	co_bidirectional_iterator<T>& operator++();
	co_bidirectional_iterator<T> operator++(int);
	co_bidirectional_iterator<T>& operator--();
	co_bidirectional_iterator<T> operator--(int);
	bool operator!=(const co_bidirectional_iterator<T>& other) const;
	bool operator==(const co_bidirectional_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_bidirectional_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type* = nullptr);
    iter::bidirectional_action&& acquire_iterable_value(reference i_value);

	ddk::function<reference(const iter::shift_action&,const detail::relative_function_impl<co_bidirectional_iterator<T>,iter::bidirectional_action&&,reference>&)> m_function;
	iter::iterable_state m_currState;
	detail::this_fiber_t m_caller;
	iter::shift_action m_currAction;
	detail::await_executor<T> m_executor;
	mutable typed_arena<reference> m_iteratorValueContainer;
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
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;

	co_random_access_iterator(const detail::none_t&);
	co_random_access_iterator(const co_random_access_iterator& other);
	~co_random_access_iterator();

	reference operator*();
	const_reference operator*() const;
	co_random_access_iterator<T>& operator++();
	co_random_access_iterator<T> operator++(int);
	co_random_access_iterator<T>& operator--();
	co_random_access_iterator<T> operator--(int);
	co_random_access_iterator<T> operator+(int i_shift) const;
	co_random_access_iterator<T> operator[](size_t i_absPos) const;
	bool operator!=(const co_random_access_iterator<T>& other) const;
	bool operator==(const co_random_access_iterator<T>& other) const;

private:
	template<typename Iterable>
	co_random_access_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type* = nullptr);
	iter::shift_action&& acquire_iterable_value(reference i_value);

	ddk::function<reference(const iter::shift_action&, const detail::relative_function_impl<co_random_access_iterator<T>,iter::shift_action&&,reference>&)> m_function;
	iter::iterable_state m_currState;
	detail::this_fiber_t m_caller;
	iter::shift_action m_currAction;
	detail::await_executor<T> m_executor;
	mutable typed_arena<reference> m_iteratorValueContainer;
};

namespace detail
{

template<typename,typename>
struct co_iterator_type_correspondence;

template<typename T>
struct co_iterator_type_correspondence<T,std::forward_iterator_tag>
{
	typedef co_forward_iterator<T> type;
	typedef iter::const_input_action action;
};
template<typename T>
struct co_iterator_type_correspondence<T,std::bidirectional_iterator_tag>
{
	typedef co_bidirectional_iterator<T> type;
	typedef iter::const_bidirectional_action action;
};
template<typename T>
struct co_iterator_type_correspondence<T, std::random_access_iterator_tag>
{
	typedef co_random_access_iterator<T> type;
	typedef iter::const_random_access_action action;
};

}

template<typename Iterable>
typename std::iterator_traits<typename Iterable::iterator>::iterator_category iterable_tag_resolver(const Iterable&);

}

#include "ddk_co_iterator.inl"
