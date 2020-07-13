#pragma once

#include "ddk_fiber_defs.h"
#include "ddk_thread_yielder.h"
#include "shared_reference_wrapper.h"
#include "lent_reference_wrapper.h"
#include <exception>

namespace ddk
{
namespace detail
{

class fiber_yielder_interface
{
protected:
	virtual ~fiber_yielder_interface() = default;

public:
	virtual void yield(yielder_context*) = 0;
	virtual void suspend(yielder_context*) = 0;
};

class fiber_scheduler_interface
{
public:
	virtual ~fiber_scheduler_interface() = default;
	virtual bool activate(fiber_id, const std::function<void()>&) = 0;
	virtual bool deactivate(fiber_id) = 0;
	virtual void unregister(fiber_id) = 0;
};

struct suspend_exception : public std::exception
{
public:
	suspend_exception(fiber_id i_id);

	const char* what() const noexcept;
	fiber_id get_id() const;

private:
	fiber_id m_id;
};

}

typedef shared_reference_wrapper<detail::fiber_scheduler_interface> fiber_scheduler_interface_shared_ref;
typedef shared_reference_wrapper<const detail::fiber_scheduler_interface> fiber_scheduler_interface_const_shared_ref;
typedef shared_pointer_wrapper<detail::fiber_scheduler_interface> fiber_scheduler_interface_shared_ptr;
typedef shared_pointer_wrapper<const detail::fiber_scheduler_interface> fiber_scheduler_interface_const_shared_ptr;

typedef lent_reference_wrapper<detail::fiber_scheduler_interface> fiber_scheduler_interface_lent_ref;
typedef lent_reference_wrapper<const detail::fiber_scheduler_interface> fiber_scheduler_interface_const_lent_ref;
typedef lent_pointer_wrapper<detail::fiber_scheduler_interface> fiber_scheduler_interface_lent_ptr;
typedef lent_pointer_wrapper<const detail::fiber_scheduler_interface> fiber_scheduler_interface_const_lent_ptr;

}
