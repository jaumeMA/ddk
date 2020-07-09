#include "ewas_fiber_utils.h"
#include "thread_utils.h"
#include "ewas_thread_impl.h"

namespace ewas
{
namespace detail
{

fiber_id& __get_current_fiber_id()
{
	static thread_local fiber_id s_fiberId(get_current_thread_id());

	return s_fiberId;
}
fiber_id get_next_available_fiber_id()
{
	static size_t s_fiberId = 0;

	return fiber_id(++s_fiberId);
}

}

void set_current_fiber_id(const fiber_id& i_id)
{
	fiber_id& currFiberId = detail::__get_current_fiber_id();

	currFiberId = i_id;
}
fiber_id get_current_fiber_id()
{
	return detail::__get_current_fiber_id();
}
fiber_id get_thread_fiber_id()
{
	return fiber_id(get_current_thread_id());
}
void suspend()
{
	if(ewas::detail::yielder_lent_ptr currYielder = ewas::detail::thread_impl_interface::get_yielder())
	{
		currYielder->suspend(nullptr);
	}
}
void yield()
{
	if(ewas::detail::yielder_lent_ptr currYielder = ewas::detail::thread_impl_interface::get_yielder())
	{
		ewas::detail::typed_yielder_context<detail::void_t> _yielder;

		_yielder.insert_value(_void);

		currYielder->yield(&_yielder);
	}
	else
	{
		std::this_thread::yield();
	}
}

}
