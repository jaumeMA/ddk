
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_fiber_utils.h"
#include "ddk_iterable_exceptions.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Iterable>
action_visitor<Iterable,input_action>::action_visitor(Iterable& i_iterable, iterable_iterator<Iterable> i_currIterator)
: m_iterable(i_iterable)
, m_currIterator(i_currIterator)
{
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,input_action>::visit(const stop_action&)
{
    return std::end(m_iterable);
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,input_action>::visit(const erase_action&)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
        return m_iterable.erase(m_currIterator);
    }

    throw iterable_operation_forbidden{"You cannot erase elements from const iterable"};
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,input_action>::visit(const add_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
        typedef typename Iterable::value_type value_type;

        return m_iterable.insert(m_currIterator,i_action.template get<value_type>());
    }

    throw iterable_operation_forbidden{"You cannot add elements into const iterable"};
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,input_action>::visit(const go_forward_action&)
{
    return ++m_currIterator;
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,bidirectional_action>::visit(const go_backward_action&)
{
    return --(this->m_currIterator);
}
template<typename Iterable>
iterable_iterator<Iterable> action_visitor<Iterable,random_access_action>::visit(const shift_action& i_action)
{
    return (this->m_currIterator += i_action.shifted());
}

}

template<typename Iterable, typename Action, typename Reference>
Reference visit_iterator(Iterable& i_iterable, const function<Action(Reference)>& i_sink, const iter::iterable_state& i_initState)
{
	const size_t currPos = i_initState.position();
	auto&& itNext = std::begin(i_iterable);

    if(currPos != iter::iterable_state::npos)
    {
        itNext = std::next(itNext,currPos);
    }

	if(itNext != std::end(i_iterable))
	{
        detail::action_visitor<Iterable,Action> actionVisitor(i_iterable,itNext);

		do
		{
            const Action currAction = i_sink(*itNext);

            itNext = currAction.visit(actionVisitor);
		}
		while (itNext != std::end(i_iterable));
	}

    suspend();
}

}
}
