
#include "ddk_type_erasured_iterable_adaptor.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
void view_iterable<Iterable>::iterate(const action& i_initialAction)
{
	this->iterate_impl(i_initialAction);
}
template<typename Iterable>
void view_iterable<Iterable>::iterate(const const_action& i_initialAction) const
{
	this->iterate_impl(i_initialAction);
}
template<typename Iterable>
iterable_adaptor<type_erasure_iterable_impl<typename view_iterable<Iterable>::traits>> view_iterable<Iterable>::deduce_owned_adaptor()
{
	return deduce_adaptor(this->get());
}

}
}