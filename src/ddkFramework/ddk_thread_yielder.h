#pragma once

#include "ddk_arena.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_embedded_type.h"
#include "ddk_fiber_defs.h"
#include "ddk_async_exceptions.h"
#include "ddk_variant.h"

namespace ddk
{
namespace detail
{

struct yielder_context
{
	enum State
	{
		Running,
		Paused,
		Stopped
	};

	yielder_context(fiber_id i_id, State i_state = Running);
	virtual ~yielder_context() = default;

	inline void stop(fiber_id i_id)
	{
		m_state = std::make_pair(i_id,Stopped);
	}
	inline bool is_stopped(fiber_id i_id) const
	{
		return m_state.first == i_id && m_state.second == Stopped;
	}
	inline void pause(fiber_id i_id)
	{
		m_state = std::make_pair(i_id,Paused);
	}
	inline bool is_paused(fiber_id i_id) const
	{
		return m_state.first == i_id && m_state.second == Paused;
	}
	inline bool is_running(fiber_id i_id) const
	{
		return m_state.first == i_id && m_state.second == Running;
	}

private:
	std::pair<fiber_id,State> m_state;
};

template<typename T>
struct typed_yielder_context : yielder_context
{
public:
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rreference;
	typedef typename embedded_type<T>::raw_type value_type;
	typedef variant<T,async_exception> result_type;

	typed_yielder_context();
    template<typename TT>
	inline typed_yielder_context(TT&& i_value);
	inline typed_yielder_context(const typed_yielder_context& other);
	inline typed_yielder_context(typed_yielder_context&& other);

	inline typed_yielder_context& operator=(const typed_yielder_context&) = delete;
	inline typed_yielder_context& operator=(typed_yielder_context&&) = delete;
	TEMPLATE(typename Arg)
	REQUIRES(IS_CONSTRUCTIBLE(result_type,Arg))
	inline void insert_value(Arg&& i_value);
	template<typename Sink>
	inline void get_value(Sink&& i_sink) const;
	template<typename Sink>
	inline void get_value(Sink&& i_sink);
	template<typename Sink>
	inline void extract_value(Sink&& i_sink) &&;

private:
	mutable result_type m_value;
};

template<>
struct typed_yielder_context<void> : yielder_context
{
public:

	typed_yielder_context() = default;
	inline ~typed_yielder_context() = default;
};

}
}

#include "ddk_thread_yielder.inl"