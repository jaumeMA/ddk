
#include "ddk_await.h"
#include "ddk_iterable_interface_utils.h"
#include "ddk_callable.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,Args...))
iterable<Iterable>::iterable(Args&& ... i_args)
: m_iterableImpl(std::forward<Args>(i_args)...)
{
}
template<typename Iterable>
TEMPLATE(typename Function, typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference),ACTION_TAGS_SUPPORTED(traits,typename Action::tags_t))
action_result iterable<Iterable>::iterate_impl(Function&& i_try, const Action& i_initialAction)
{
	try
	{
		typedef typename mpl::aqcuire_callable_return_type<Function>::type action_type;

		m_iterableImpl.iterate_impl(fixed_return_terse_callable<action_type,Function>{ std::forward<Function>(i_try) },i_initialAction);
	}
	catch(const suspend_exception& i_excp)
	{
		if(i_excp)
		{
			return make_error<action_result>(ActionError::StopError,stop_error(StopError::Error,i_excp.what(),i_excp.get_code()));
		}
	}

	return success;
}
template<typename Iterable>
TEMPLATE(typename Function, typename Action)
REQUIRED(IS_CALLABLE(Function),ACTION_TAGS_SUPPORTED(traits,typename Action::tags_t))
action_result iterable<Iterable>::iterate_impl(Function&& i_try, const Action& i_initialAction) const
{
	try
	{
		typedef typename mpl::aqcuire_callable_return_type<Function>::type action_type;

		m_iterableImpl.iterate_impl(fixed_return_terse_callable<action_type,Function>{ std::forward<Function>(i_try) },i_initialAction);
	}
	catch(const suspend_exception& i_excp)
	{
		if(i_excp)
		{
			return make_error<action_result>(ActionError::StopError,stop_error(StopError::Error,i_excp.what(),i_excp.get_code()));
		}
	}

	return success;
}
template<typename Iterable>
bool iterable<Iterable>::operator==(const std::nullptr_t&) const
{
    return m_iterableImpl == nullptr;
}
template<typename Iterable>
bool iterable<Iterable>::operator!=(const std::nullptr_t&) const
{
    return m_iterableImpl != nullptr;
}
template<typename Iterable>
const Iterable& iterable<Iterable>::get() const
{
	return m_iterableImpl;
}
template<typename Iterable>
Iterable&& iterable<Iterable>::extract() &&
{
	return std::move(m_iterableImpl);
}
template<typename Iterable>
action_result iterable<Iterable>::iterate(terse_endpoint i_try,const action& i_initialAction)
{
	return iterate_impl(std::move(i_try),i_initialAction);
}
template<typename Iterable>
action_result iterable<Iterable>::iterate(const_terse_endpoint i_try,const action& i_initialAction) const
{
	return iterate_impl(std::move(i_try),i_initialAction);
}

}
}
