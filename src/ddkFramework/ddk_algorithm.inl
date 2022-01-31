
#include "ddk_template_helper.h"

//JAUME: we have to do this due to operator scope resolution ¬¬
TEMPLATE(typename Iterable,typename Filter)
REQUIRES(IS_ITERABLE(Iterable),IS_CALLABLE(Filter))
inline auto __find(Iterable&& i_iterable, Filter&& i_filter)
{
	return ddk::view::take_n(ddk::view::filter(std::forward<Filter>(i_filter)) <<= std::forward<Iterable>(i_iterable),1);
}
TEMPLATE(typename Iterable,typename Filter)
REQUIRES(IS_ITERABLE(Iterable),IS_CALLABLE(Filter))
inline auto __find_any(Iterable&& i_iterable,Filter&& i_filter)
{
	return ddk::view::filter(std::forward<Filter>(i_filter)) <<= std::forward<Iterable>(i_iterable);
}

namespace ddk
{
namespace iter
{

TEMPLATE(typename Iterable,typename Value)
REQUIRED(IS_ITERABLE(Iterable))
auto find(Iterable&& i_iterable, Value&& i_value)
{
	typedef typename ddk::mpl::remove_qualifiers<Iterable>::const_reference const_reference;

	return __find(std::forward<Iterable>(i_iterable),[&i_value](const_reference i_currValue) { return i_value == i_currValue; });
}
TEMPLATE(typename Iterable,typename Callable)
REQUIRED(IS_ITERABLE(Iterable),IS_CALLABLE(Callable))
auto find_if(Iterable&& i_iterable,Callable&& i_callable)
{
	return __find(std::forward<Iterable>(i_iterable),std::forward<Callable>(i_callable));
}
TEMPLATE(typename Iterable,typename Value)
REQUIRED(IS_ITERABLE(Iterable))
auto find_any(Iterable&& i_iterable,Value&& i_value)
{
	typedef typename ddk::mpl::remove_qualifiers<Iterable>::const_reference const_reference;

	return __find_any(std::forward<Iterable>(i_iterable),[&i_value](const_reference i_currValue) { return i_value == i_currValue; });
}
TEMPLATE(typename Iterable,typename Callable)
REQUIRED(IS_ITERABLE(Iterable),IS_CALLABLE(Callable))
auto find_any_if(Iterable&& i_iterable,Callable&& i_callable)
{
	return __find_any(std::forward<Iterable>(i_iterable),std::forward<Callable>(i_callable));
}

}
}