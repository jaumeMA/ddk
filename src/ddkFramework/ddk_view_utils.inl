
#include "ddk_function_utils.h"
#include "ddk_iterable_utils.h"

namespace ddk
{
namespace view
{
namespace detail
{

struct num_items_constrain
{
public:
	struct callable_tag;

	num_items_constrain(size_t i_maxItems)
	: m_maxItems(i_maxItems)
	{
	}
	template<typename T>
	bool operator()(T&& i_value) const
	{
		return m_currItem++ < m_maxItems;
	}

private:
	size_t m_maxItems;
	mutable size_t m_currItem = 0;
};

}

template<typename Type, typename Allocator>
ddk::detail::iterable_filter<function<bool(Type),Allocator>> filter(const function<bool(Type),Allocator>& i_functor)
{
	return ddk::detail::iterable_filter<function<bool(Type),Allocator>>(i_functor);
}
TEMPLATE(typename Functor)
REQUIRED(IS_CALLABLE(Functor))
ddk::detail::iterable_filter<ddk::detail::resolved_callable<Functor>> filter(Functor&& i_functor)
{
	return ddk::detail::iterable_filter<ddk::detail::resolved_callable<Functor>>(make_function(std::forward<Functor>(i_functor)));
}
template<typename T>
ddk::detail::iterable_order<T> order(const T& i_order)
{
	return ddk::detail::iterable_order<T>(i_order);
}
TEMPLATE(typename Iterable,typename ... Predicates)
REQUIRED(IS_CALLABLE(Predicates)...)
std::array<decltype(deduce_iterable(std::declval<Iterable>())),mpl::num_types<Predicates...>+1> group_by(Iterable&& i_lhs, Predicates&& ... i_predicates)
{
	return { filter(deduce_function(i_predicates)) <<= deduce_iterable(i_lhs) ..., filter((!deduce_function(i_predicates) && ...)) <<= deduce_iterable(i_lhs)};
}
TEMPLATE(typename Iterable,typename Functor)
REQUIRED(IS_CALLABLE(Functor))
auto constrain(Iterable&& i_iterable, Functor&& i_constrain)
{
	return ddk::detail::iterable_constrain(std::forward<Functor>(i_constrain)) <<= std::forward<Iterable>(i_iterable);
}
template<typename Iterable>
auto take_n(Iterable&& i_iterable,size_t i_numItems)
{
	return constrain(std::forward<Iterable>(i_iterable),detail::num_items_constrain(i_numItems));
}
TEMPLATE(typename Functor)
REQUIRED(IS_CALLABLE(Functor))
auto constrain(Functor&& i_constrain)
{
	return detail::iterable_constrain{ std::forward<Functor>(i_constrain) };
}

}
}
