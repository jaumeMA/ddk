#include "ddk_iterable_order_resolver.h"

namespace ddk
{
namespace detail
{

transpose_multi_dimensional_order_resolver::action::action(difference_type i_targetShift)
: m_shift(i_targetShift)
{
}
transpose_multi_dimensional_order_resolver::action::action(const stop_action&)
: action_base(false)
{
}
transpose_multi_dimensional_order_resolver::action::difference_type transpose_multi_dimensional_order_resolver::action::shift() const
{
	return m_shift;
}

}

const detail::forward_order_resolver forward_order = detail::forward_order_resolver();
const detail::backward_order_resolver reverse_order = detail::backward_order_resolver();
const detail::transpose_multi_dimensional_order_resolver transponse_dimension_order = detail::transpose_multi_dimensional_order_resolver();

}
