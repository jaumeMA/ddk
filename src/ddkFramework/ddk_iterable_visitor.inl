
#pragma warning(disable: 4102)

namespace ddk
{
namespace detail
{

template<typename Iterable>
template<typename ... Args>
iterable_visitor<Iterable>::iterable_visitor(Args&& ... i_args)
: m_adaptor(std::forward<Args>(i_args)...)
{
}
template<typename Iterable>
template<typename Action>
iterable_result iterable_visitor<Iterable>::loop(const Action& i_initialAction)
{
	typedef decltype(std::declval<Action>().apply(std::declval<adaptor_t&>())) return_result;

	Action currAction = i_initialAction;

apply_action:
	if (auto actionRes = currAction.apply(m_adaptor))
	{
		if constexpr (mpl::is_same_type<Action,typename return_result::payload_t>::value)
		{
			if (currAction = std::move(actionRes).extract())
			{
				goto apply_action;
			}
			else
			{
				return success;
			}
		}
		else
		{
			if (auto newAction = std::move(actionRes).extract())
			{
				return loop(std::move(newAction));
			}
			else
			{
				return success;
			}
		}
	}
	else
	{
		return actionRes.error();
	}
}
template<typename Iterable>
template<typename Action>
iterable_result iterable_visitor<Iterable>::loop(const Action& i_initialAction) const
{
	typedef decltype(std::declval<Action>().apply(std::declval<const adaptor_t&>())) return_result;

	Action currAction = i_initialAction;

apply_action:
	if (auto actionRes = currAction.apply(m_adaptor))
	{
		if constexpr (mpl::is_same_type<Action,typename return_result::payload_t>::value)
		{
			if (currAction = std::move(actionRes).extract())
			{
				goto apply_action;
			}
			else
			{
				return success;
			}
		}
		else
		{
			if (auto newAction = std::move(actionRes).extract())
			{
				return loop(std::move(newAction));
			}
			else
			{
				return success;
			}
		}
	}
	else
	{
		return actionRes.error();
	}
}

}
}

#pragma warning(default: 4102)