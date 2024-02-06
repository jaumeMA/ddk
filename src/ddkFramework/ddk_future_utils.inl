
namespace ddk
{

TEMPLATE(typename ... Futures)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(0,Futures...),TYPE_CONTAINS_SYMBOL(Futures,future_tag)...)
auto compose(Futures&& ... i_components)
{
	if constexpr (mpl::are_same_type<typename Futures::value_type ...>())
	{
		return composed_future<std::array<mpl::nth_type_of_t<0, typename Futures::value_type...>,static_cast<unsigned char>(mpl::num_types<Futures...>)>>(std::forward<Futures>(i_components)...);
	}
	else
	{
		return composed_future<std::tuple<typename Futures::value_type...>>(std::forward<Futures>(i_components) ...);
	}
}

}
