
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
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
void iterable<Iterable>::iterate_impl(Action&& i_initialAction)
{
	m_iterableImpl.iterate_impl(std::forward<Action>(i_initialAction));
}
template<typename Iterable>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
void iterable<Iterable>::iterate_impl(Action&& i_initialAction) const
{
	m_iterableImpl.iterate_impl(std::forward<Action>(i_initialAction));
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
void iterable<Iterable>::iterate(const action& i_initialAction)
{
	iterate_impl(i_initialAction);
}
template<typename Iterable>
void iterable<Iterable>::iterate(const const_action& i_initialAction) const
{
	iterate_impl(i_initialAction);
}
template<typename Iterable>
iterable_adaptor<type_erasure_iterable_impl<typename iterable<Iterable>::traits>> iterable<Iterable>::deduce_owned_adaptor()
{
	return deduce_adaptor(m_iterableImpl);
}

}
}
