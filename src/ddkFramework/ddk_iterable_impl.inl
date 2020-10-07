
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
void iterable_impl<Traits,Iterable>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    visit_iterator(m_iterable,i_try,i_initState);
}
template<typename Traits, typename Iterable>
void iterable_impl<Traits,Iterable>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    visit_iterator(m_iterable,i_try,i_initState);
}
template<typename Traits, typename Iterable>
size_t iterable_impl<Traits,Iterable>::size() const
{
    return m_iterable.size();
}
template<typename Traits, typename Iterable>
bool iterable_impl<Traits,Iterable>::empty() const
{
    return m_iterable.empty();
}

}
}
