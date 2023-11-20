
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

TEMPLATE(typename Functor)
REQUIRED(IS_CALLABLE(Functor))
ddk::detail::iterable_filter<Functor> filter(Functor&& i_functor)
{
	return { std::forward<Functor>(i_functor) };
}
template<typename T>
ddk::detail::iterable_order<T> order(const T& i_order)
{
	return { i_order };
}
TEMPLATE(typename Iterable,typename ... Predicates)
REQUIRED(IS_BASE_OF_ITERABLE(Iterable),IS_CALLABLE(Predicates)...)
auto group_by(Iterable&& i_lhs, Predicates&& ... i_predicates)
{
	return std::array<decltype(deduce_iterable(std::declval<mpl::remove_qualifiers<Iterable>>())),mpl::num_types<Predicates...>+1>{ filter(deduce_function(i_predicates)) <<= deduce_iterable(i_lhs) ..., filter((!deduce_function(i_predicates) && ...)) <<= deduce_iterable(i_lhs)};
}
TEMPLATE(typename Functor)
REQUIRED(IS_CALLABLE(Functor))
ddk::detail::iterable_constrain<Functor> constrain(Functor&& i_constrain)
{
	return { std::forward<Functor>(i_constrain) };
}
template<typename Iterable>
auto take_n(Iterable&& i_iterable,size_t i_numItems)
{
	return constrain(std::forward<Iterable>(i_iterable),detail::num_items_constrain(i_numItems));
}

}
}
