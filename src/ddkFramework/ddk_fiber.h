//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_fiber_impl.h"
#include "ddk_fiber_defs.h"

namespace ddk
{

class fiber
{
	template<typename>
	friend class fiber_scheduler;

public:
	typedef detail::fiber_impl::start_result start_result;
	typedef detail::fiber_impl::start_error start_error;
	typedef detail::fiber_impl::stop_result stop_result;
	typedef detail::fiber_impl::stop_error stop_error;

	fiber(detail::fiber_impl_unique_ref i_fiber);
	fiber(const fiber&) = delete;
	fiber(fiber&& other);
	~fiber() = default;
	fiber& operator=(const fiber&) = delete;
	fiber& operator=(fiber&& other);

	fiber_id get_id() const;
	start_result start(const ddk::function<void()>& i_function);
	stop_result stop();
	bool ready() const;
	bool joinable() const;

private:
	detail::fiber_impl_unique_ref m_impl;
};

template<typename Allocator, typename ... Args>
fiber make_fiber(Args&& ... i_args);

}

#include "ddk_fiber.inl"
