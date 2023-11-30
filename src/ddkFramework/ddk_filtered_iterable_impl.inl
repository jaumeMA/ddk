
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_interface_utils.h"
#include "ddk_iterable_action_concepts.h"

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_filter<Function>::iterable_filter(const Function& i_filter)
: m_filter(i_filter)
{
}
template<typename Function>
Function iterable_filter<Function>::get_filter() const
{
	return m_filter;
}

template<typename Iterable, typename Filter>
TEMPLATE(typename IIterable,typename FFilter)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Filter,FFilter))
filtered_iterable_impl<Iterable,Filter>::filtered_iterable_impl(IIterable&& i_iterable,FFilter&& i_filter)
: m_iterable(std::forward<IIterable>(i_iterable))
, m_filter(std::forward<FFilter>(i_filter))
{
}
template<typename Iterable,typename Filter>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
iterable_result filtered_iterable_impl<Iterable,Filter>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
    typedef typename mpl::aqcuire_callable_return_type<Function>::type return_type;

    step_by_step_action currAction(i_initialAction);

    return m_iterable.iterate_impl([&](reference i_value) mutable -> step_by_step_action<return_type>
    {
        if (ddk::terse_eval(m_filter,std::forward<reference>(i_value)))
        {
            if constexpr (IS_FILTERED_ACTION_COND(Action))
            {
                //allow later on filters to be applied
                return ddk::eval(std::forward<Function>(i_try),i_value);
            }
            else
            {
                //otherwise, send whenever ready
                if (currAction.ready())
                {
                    return ddk::eval(std::forward<Function>(i_try),i_value);
                }
                else
                {
                    return --currAction;
                }
            }
        }
        else
        {
            return currAction;
        }
    },currAction);
}
template<typename Iterable,typename Filter>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,const_reference))
iterable_result filtered_iterable_impl<Iterable,Filter>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
    typedef typename mpl::aqcuire_callable_return_type<Function>::type return_type;

    step_by_step_action currAction(i_initialAction);

    return m_iterable.iterate_impl([&](const_reference i_value) mutable -> step_by_step_action<return_type>
    {
        if (ddk::terse_eval(m_filter,std::forward<const_reference>(i_value)))
        {
            if constexpr (IS_FILTERED_ACTION_COND(Action))
            {
                //allow later on filters to be applied
                return ddk::eval(std::forward<Function>(i_try),i_value);
            }
            else
            {
                //otherwise, send whenever ready
                if (currAction.ready())
                {
                    return ddk::eval(std::forward<Function>(i_try),i_value);
                }
                else
                {
                    return --currAction;
                }
            }
        }
        else
        {
            return currAction;
        }
    },currAction);
}

}
}
