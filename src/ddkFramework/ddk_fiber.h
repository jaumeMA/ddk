#pragma once

#include "lent_reference_wrapper.h"
#include "ewas_fiber_impl.h"
#include "ewas_fiber_defs.h"

namespace ewas
{

class fiber
{
	template<typename>
	friend class fiber_scheduler;

public:
	fiber(detail::fiber_impl_unique_ref i_fiber);
	fiber(const fiber&) = delete;
	fiber(fiber&& other);
	~fiber() = default;
	fiber& operator=(const fiber&) = delete;
	fiber& operator=(fiber&& other);

	fiber_id get_id() const;
	void start(const std::function<void()>& i_function);
	bool ready() const;
	void stop();

private:
	detail::fiber_impl_unique_ref m_impl;
};

extern const detail::this_fiber_t this_fiber;

template<typename Allocator, typename ... Args>
fiber make_fiber(Args&& ... i_args);

}

#include "ewas_fiber.inl"