
#include "ewas_thread_yielder.h"
#include "ewas_thread_impl.h"

namespace ewas
{

template<typename T>
void yield(T&& i_value)
{
	if(detail::yielder_lent_ptr currYielder = detail::thread_impl_interface::get_yielder())
	{
		detail::typed_yielder_context<T> _yielder(std::forward<T>(i_value));

		currYielder->yield(&_yielder);
	}
	else
	{
		std::this_thread::yield();
	}
}

}
