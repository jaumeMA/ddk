
namespace ddk
{
namespace detail
{

template<typename Function>
iterable_constrain<Function>::iterable_constrain(const Function& i_constrain)
: m_constrain(i_constrain)
{
}
template<typename Function>
Function iterable_constrain<Function>::get_constrain() const
{
	return m_constrain;
}

template<typename Traits,typename Function>
constrained_iterable_impl<Traits,Function>::constrained_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef,const Function& i_actionResolver)
: m_iterableRef(i_iterableRef)
, m_constrain(i_actionResolver)
{
}
template<typename Traits,typename Function>
void constrained_iterable_impl<Traits,Function>::iterate_impl(const function<action(reference)>& i_try,const shift_action& i_initialAction,action_state_lent_ptr i_actionStatePtr)
{
	m_iterableRef->iterate_impl(make_function([i_try,this](reference i_value) -> action { if(ddk::eval(m_constrain,i_value) == false) { stop_iteration(); } return ddk::eval(i_try,i_value); }),i_initialAction,i_actionStatePtr);
}
template<typename Traits,typename Function>
void constrained_iterable_impl<Traits,Function>::iterate_impl(const function<action(const_reference)>& i_try,const shift_action& i_initialAction,action_state_lent_ptr i_actionStatePtr) const
{
	m_iterableRef->iterate_impl(make_function([i_try,this](const_reference i_value) -> action { if(ddk::eval(m_constrain,i_value) == false) { stop_iteration(); } return ddk::eval(i_try,i_value); }),i_initialAction,i_actionStatePtr);
}

}
}