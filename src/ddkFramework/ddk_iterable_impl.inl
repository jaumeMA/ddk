
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Traits, typename Iterable>
iterable_impl<Traits,Iterable>::iterable_impl(Iterable& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Traits, typename Iterable>
void iterable_impl<Traits,Iterable>::iterate_impl(const function<action(reference)>& i_try)
{
    visit_iterator(m_iterable,i_try);
}
template<typename Traits, typename Iterable>
void iterable_impl<Traits,Iterable>::iterate_impl(const function<action(const_reference)>& i_try) const
{
    visit_iterator(m_iterable,i_try);
}

}
}
