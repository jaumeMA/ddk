#pragma once

#include "ddk_sync_executor.h"
#include "ddk_none.h"

namespace ddk
{

template<typename T>
struct awaited_result
{
	template<typename TT>
	friend inline bool operator|=(TT& i_lhs, awaited_result<T> i_rhs)
	{
		if(i_rhs)
		{
			i_lhs = i_rhs.get();
		}

		return static_cast<bool>(i_rhs);
	}

public:
	typedef typename std::add_lvalue_reference<T>::type reference;
	typedef typename std::add_const<reference>::type const_reference;
	typedef typename std::add_pointer<T>::type pointer;
	typedef typename std::add_const<pointer>::type const_pointer;

	awaited_result(const detail::none_t&);
	awaited_result(T i_content);
	awaited_result(const awaited_result&) = delete;
	awaited_result(awaited_result&& other);
	~awaited_result();
	awaited_result<T>& operator=(const awaited_result<T>&) = delete;
	awaited_result<T>& operator=(awaited_result<T>&&);
	const_reference get() const;
	const_pointer get_ptr() const;
	reference get();
	pointer get_ptr();
	explicit operator bool() const;
	explicit operator const_reference() const;
	explicit operator reference();
	void set(T i_content);

private:
	arena<sizeof(T),alignof(T)> m_content;
};

template<typename T, typename Result = awaited_result<T>>
class awaitable
{
	friend Result;
	friend inline Result resume(awaitable<T,Result>& i_awaitable)
	{
		typename awaitable<T,Result>::continue_result res = i_awaitable.resume();

		return (res.hasError() == false) ? i_awaitable.m_executor->extract_value() : Result(none);
	}

public:
	typedef typename async_executor<T>::start_result continue_result;
	typedef typename Result::reference reference;
	typedef typename Result::const_reference const_reference;

	template<typename ... Types, typename ... Args>
	awaitable(const std::function<T(Types...)>& i_function, Args&& ... i_args);
	awaitable(const awaitable& other);
	awaitable(awaitable&& other);
	continue_result resume();

private:
	async_execute_shared_ptr<T> m_executor;
};

}

#include "ddk_awaitable.inl"
