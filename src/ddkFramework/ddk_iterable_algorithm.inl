
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_template_helper.h"
#include "ddk_function.h"

namespace ddk
{
namespace iter
{

template<typename Return>
template<typename Functor>
transform_iterable_transform::as<Return>::transform<Functor>::transform(const Functor& i_functor)
: m_functor(i_functor)
{
}
template<typename Return>
template<typename Functor>
template<typename ... Args>
constexpr auto transform_iterable_transform::as<Return>::transform<Functor>::operator()(Args&& ... i_args) const
{
	return ddk::terse_eval(m_functor,std::forward<Args>(i_args)...);
}

template<typename Return>
template<typename Functor>
auto transform_iterable_transform::as<Return>::operator()(Functor&& i_transform) const
{
	return detail::iterable_transform{ transform{ std::forward<Functor>(i_transform) } };
}
template<typename Functor>
auto transform_iterable_transform::operator()(Functor&& i_transform) const
{
	return detail::iterable_transform{ std::forward<Functor>(i_transform) };
}

}
}

TEMPLATE(typename Sink,typename Iterable)
REQUIRED(IS_CONTAINER(Sink))
auto operator<<=(Sink& i_sink, Iterable&& i_iterable)
{
	return ddk::trans::detail::iterable_transformation_dump(i_sink,std::forward<Iterable>(i_iterable));
}