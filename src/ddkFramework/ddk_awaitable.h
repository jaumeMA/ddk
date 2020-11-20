#pragma once

#include "ddk_sync_executor.h"
#include "ddk_none.h"

namespace ddk
{
template<typename>
struct awaited_result;

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

	awaited_result() = default;
	awaited_result(const detail::none_t&);
    template<typename Arg>
	awaited_result(Arg&& i_arg);
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
	typed_arena<T> m_content;
};

template<>
struct awaited_result<void> : awaited_result<detail::void_t>
{
public:
    using awaited_result<detail::void_t>::awaited_result;
};

template<typename T,typename = awaited_result<T>>
class awaitable;

template<typename T, typename Result>
class awaitable
{
	friend Result;
	friend inline Result resume(awaitable<T,Result>& i_awaitable)
	{
		typename awaitable<T,Result>::continue_result res = i_awaitable.resume();

		return (res == success) ? Result{ i_awaitable.m_executor->extract_value() } : Result{ none };
	}

public:
	typedef typename async_executor<T>::start_result continue_result;
	typedef typename Result::reference reference;
	typedef typename Result::const_reference const_reference;

    awaitable() = default;
	awaitable(const ddk::function<T()>& i_function, const detail::this_thread_t& i_thread);
	awaitable(const awaitable& other);
	awaitable(awaitable&& other);
	continue_result resume();
	operator bool() const;

    awaitable& operator=(const awaitable&) = default;
    awaitable& operator=(awaitable&&) = default;

protected:
	async_execute_shared_ptr<T> m_executor;
};

template<typename Result>
class awaitable<void,Result> : public awaitable<detail::void_t,Result>
{
public:
    awaitable() = default;
	awaitable(const ddk::function<void()>& i_function, const detail::this_thread_t& i_thread);
	awaitable(const awaitable& other) = default;
	awaitable(awaitable&& other) = default;

	awaitable& operator=(const awaitable&) = default;
	awaitable& operator=(awaitable&&) = default;
};

}

#include "ddk_awaitable.inl"
