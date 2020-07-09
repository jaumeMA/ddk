#pragma once

#include "ewas_signal_functor.h"
#include "ewas_connection.h"
#include "ewas_macros.h"
#include "ewas_linked_list.h"
#include "ewas_signal_connector.h"

namespace ewas
{

template<typename>
class callback;

template<typename Return, typename ... Types>
class callback<Return(Types...)> : protected detail::signal_connector
{
private:
	typedef signal_functor<Return(Types...)> signal_functor_t;

    callback(const callback<Return(Types...)>&);
    callback<Return(Types...)>& operator=(const callback<Return(Types...)>&);

public:
	callback()
	{
	}
	callback(callback<Return(Types...)>&& other)
	: m_recipient(std::move(other.m_recipient))
	{
	}
    ~callback()
	{
		disconnect();
	}
    //non static member functions
    template<typename T, typename baseT, typename ... Args>
    detail::connection_base& connect(T *object, Return (baseT::*hook)(Args ...)) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<Return(Types...)> call = [object,hook](Args&& ... i_args) mutable -> Return { return (object->*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	template<typename T, typename baseT, typename ... Args>
	detail::connection_base& connect(const T *object, Return(baseT::*hook)(Args...)const) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<Return(Types...)> call = [object,hook](Args&& ... i_args) mutable -> Return { return (object->*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	//functors
	template<typename Functor>
	detail::connection_base& connect(const Functor& i_functor) const
	{
		std::function<Return(Types...)> call(i_functor);

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	//static member/free functions
	template<typename ... Args>
    detail::connection_base& connect(Return(*hook)(Args...)) const
	{
		const std::function<Return(Types...)> call = [hook](Args&& ... i_args) mutable -> Return { return (*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
    void disconnect()
	{
		m_recipient.clear();
	}
    template<typename ... Args>
	Return execute(Args&& ... i_args) const
	{
		EWAS_ASSERT(m_recipient.empty() == false, "Trying to call null function");

        return (*m_recipient.begin()).execute(std::forward<Args>(i_args) ...);
	}
	bool is_connected() const
	{
		return m_recipient.empty() == false;
	}
	operator bool() const
	{
		return m_recipient.empty() == false;
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		m_recipient.clear();
	}

	mutable linked_list<signal_functor_t> m_recipient;
};

//void return case
template<typename ... Types>
class callback<void(Types...)> : protected detail::signal_connector
{
private:
	typedef signal_functor<void(Types...)> signal_functor_t;

    callback(const callback<void(Types...)>&);
    callback<void(Types...)>& operator=(const callback<void(Types...)>&);

public:
	callback()
	{
	}
	callback(callback<void(Types...)>&& other)
	: m_recipient(std::move(other.m_recipient))
	{
	}
    ~callback()
	{
		disconnect();
	}
    //non static member functions
    template<typename T, typename baseT, typename ... Args>
    detail::connection_base& connect(T *object, void(baseT::*hook)(Args...)) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args) { (object->*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	template<typename T, typename baseT, typename ... Args>
	detail::connection_base& connect(const T *object, void(baseT::*hook)(Types...)const) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args){ (object->*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	//functors
	template<typename Functor>
	detail::connection_base& connect(const Functor& i_functor) const
	{
		std::function<void(Types...)> call(i_functor);

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
	//static member/free functions
	template<typename ... Args>
    detail::connection_base& connect(void(*hook)(Args ...)) const
	{
		const std::function<void(Types...)> call = [hook](Args&& ... i_args) { (*hook)(std::forward<Args>(i_args)...); };

		EWAS_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(call,static_cast<const detail::signal_connector&>(*this));
	}
    void disconnect()
	{
		m_recipient.clear();
	}
    template<typename ... Args>
	void execute(Args&& ... i_args) const
	{
		EWAS_ASSERT(m_recipient.empty() == false, "Trying to call null function");

        (*m_recipient.begin()).execute(std::forward<Args>(i_args) ...);
	}
	bool is_connected() const
	{
		return m_recipient.empty() == false;
	}
	operator bool() const
	{
		return m_recipient.empty() == false;
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		m_recipient.clear();
	}

	mutable linked_list<signal_functor_t> m_recipient;
};

}
