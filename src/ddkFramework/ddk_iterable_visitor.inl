
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_fiber_utils.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(adaptor_t,Args...))
iterable_visitor<Iterable>::iterable_visitor(Args&& ... i_args)
: m_adaptor(std::forward<Args>(i_args)...)
{
}
template<typename Iterable>
template<typename Function,typename Action>
void iterable_visitor<Iterable>::loop(Function&& i_sink, const Action& i_initialAction) const
{
	typedef decltype(std::declval<Action>().apply(std::declval<adaptor_t>(),std::declval<Function>())) return_type;

	Action currAction = i_initialAction;

	if constexpr (mpl::is_same_type<Action,return_type>::value)
	{
		while (currAction = currAction.apply(m_adaptor,std::forward<Function>(i_sink)));
	}
	else
	{
		if (auto newAction = currAction.apply(m_adaptor,std::forward<Function>(i_sink)))
		{
			loop(std::forward<Function>(i_sink),newAction);
		}
	}
}

}
}
