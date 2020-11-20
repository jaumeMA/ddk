#pragma once

#include "ddk_arena.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_embedded_type.h"
#include "ddk_fiber_defs.h"

namespace ddk
{
namespace detail
{

struct yielder_context
{
	yielder_context();
	virtual ~yielder_context() = default;

	inline void stop(fiber_id i_id)
	{
		m_stop = std::make_pair(i_id,true);
	}
	inline bool is_stopped(fiber_id i_id) const
	{
		return m_stop.first == i_id && m_stop.second;
	}

private:
	std::pair<fiber_id,bool> m_stop;
};

template<typename T>
struct typed_yielder_context : yielder_context
{
public:
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::raw_type value_type;

	typed_yielder_context() = default;
    template<typename TT>
	inline typed_yielder_context(TT&& i_value);
	inline typed_yielder_context(const typed_yielder_context& other);
	inline typed_yielder_context(typed_yielder_context&& other);
	inline ~typed_yielder_context();

	inline typed_yielder_context& operator=(const typed_yielder_context& other);
	inline typed_yielder_context& operator=(typed_yielder_context&& other);
	inline void insert_value(T i_value);
	inline const_reference get_value() const;
	inline reference get_value();

private:
	mutable typed_arena<T> m_value;
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
