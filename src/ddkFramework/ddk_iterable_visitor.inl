
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
void iterable_visitor<Iterable>::loop(const Action& i_initialAction)
{
	_loop<mpl::empty_type_pack>(i_initialAction);
}
template<typename Iterable>
template<typename Action>
void iterable_visitor<Iterable>::loop(const Action& i_initialAction) const
{
	_loop<mpl::empty_type_pack>(i_initialAction);
}
template<typename Iterable>
template<typename TypePack, typename Action>
constexpr auto iterable_visitor<Iterable>::_loop(const Action& i_action)
{
	typedef decltype(std::declval<Action>().apply(std::declval<adaptor_t>())) return_action;
	Action currAction = i_action;

apply_action:
	if constexpr (mpl::is_same_type<Action,return_action>::value)
	{
		if (currAction = Action(currAction.apply(m_adaptor)))
		{
			goto apply_action;
		}
		else
		{
			return currAction;
		}
	}
	else
	{
		if constexpr (TypePack::template contains<return_action>())
		{
			return currAction.apply(m_adaptor);
		}
		else
		{
			typedef typename TypePack::template add_unique<Action>::type new_type_pack;
			typedef decltype(_loop<new_type_pack>(std::declval<return_action>())) action_descent;

			if constexpr (mpl::is_same_type<Action,action_descent>::value)
			{
				if (auto nextAction = currAction.apply(m_adaptor))
				{
					if (currAction = _loop<new_type_pack>(std::move(nextAction)))
					{
						goto apply_action;
					}
					else
					{
						return nextAction;
					}
				}
				else
				{
					return stop_action(currAction);
				}
			}
			else
			{
				if (auto nextAction = currAction.apply(m_adaptor))
				{
					return _loop<new_type_pack>(std::move(nextAction));
				}
				else
				{
					return nextAction;
				}
			}
		}
	}
}
template<typename Iterable>
template<typename TypePack,typename Action>
constexpr auto iterable_visitor<Iterable>::_loop(const Action& i_action) const
{
	typedef decltype(std::declval<Action>().apply(std::declval<adaptor_t>())) return_action;
	Action currAction = i_action;

apply_action:
	if constexpr (mpl::is_same_type<Action,return_action>::value)
	{
		if (currAction = currAction.apply(m_adaptor))
		{
			goto apply_action;
		}
		else
		{
			return currAction;
		}
	}
	else
	{
		if constexpr (TypePack::template contains<return_action>())
		{
			return currAction.apply(m_adaptor);
		}
		else
		{
			typedef typename TypePack::template add_unique<Action>::type new_type_pack;
			typedef decltype(_loop<new_type_pack>(std::declval<return_action>())) action_descent;

			if constexpr (mpl::is_same_type<Action,action_descent>::value)
			{
				if (auto nextAction = currAction.apply(m_adaptor))
				{
					if (currAction = _loop<new_type_pack>(std::move(nextAction)))
					{
						goto apply_action;
					}
					else
					{
						return currAction;
					}
				}
				else
				{
					return stop_action(currAction);
				}
			}
			else
			{
				if (auto nextAction = currAction.apply(m_adaptor))
				{
					return _loop<new_type_pack>(std::move(nextAction));
				}
				else
				{
					return nextAction;
				}
			}
		}
	}
}

}
}

#pragma warning(default: 4102)