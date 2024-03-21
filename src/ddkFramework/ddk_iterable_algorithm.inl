
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_template_helper.h"
#include "ddk_function.h"

namespace ddk
{
namespace iter
{

template<typename Functor>
auto transform_iterable_transform::operator()(Functor&& i_transform) const
{
	return detail::iterable_transform<Functor>{i_transform};
}

}
}

TEMPLATE(typename Sink,typename Iterable)
REQUIRED(IS_CONTAINER(Sink))
auto operator<<=(Sink& i_sink, Iterable&& i_iterable)
{
	return ddk::trans::detail::iterable_transformation_dump(i_sink,std::forward<Iterable>(i_iterable));
}