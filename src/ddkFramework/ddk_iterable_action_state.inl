
#include "ddk_iterable_exceptions.h"

namespace ddk
{

template<typename T>
void action_state::forward_result(T&& i_result)
{
	m_actionResult.dismiss();

	m_actionResult = std::forward<T>(i_result);
}
template<typename T>
T&& action_state::forward_result()
{
    return std::move(m_actionResult).extract().template get_as<T>();
}
template<typename Visitor>
typename Visitor::return_type action_state::visit(Visitor& i_visitor) const
{
	if constexpr(std::is_same<void,typename Visitor::return_type>::value)
	{
		if(m_actionResult != success)
		{
			m_actionResult.error().visit(i_visitor);
		}
		else
		{
			throw iteration_exception{"Trying to visit error result which is ok"};
		}
	}
	else
	{
		if(m_actionResult != success)
		{
			return m_actionResult.error().visit(i_visitor);
		}
		else
		{
			throw iteration_exception{ "Trying to visit error result which is ok" };
		}
	}
}

}
