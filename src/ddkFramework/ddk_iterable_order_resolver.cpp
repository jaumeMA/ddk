#include "ddk_iterable_order_resolver.h"

namespace ddk
{

const detail::forward_order_resolver forward_order = detail::forward_order_resolver();
const detail::backward_order_resolver reverse_order = detail::backward_order_resolver();
const detail::transpose_multi_dimensional_order_resolver transponse_dimension_order = detail::transpose_multi_dimensional_order_resolver();

}
