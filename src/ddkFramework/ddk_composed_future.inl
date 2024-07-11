
namespace ddk
{

template<typename T, size_t Dim>
composed_future<std::array<T,Dim>>::future_data::future_data(ddk::promise<std::array<T, Dim>> i_promise)
: m_promise(std::move(i_promise))
{}
template<typename T,size_t Dim>
composed_future<std::array<T,Dim>>::future_data::~future_data()
{
	for (size_t index = 0;index < Dim;index++)
	{
		m_values[index].template destroy<T>();
	}
}
template<typename T, size_t Dim>
TEMPLATE(typename ... Futures)
REQUIRED(IS_BASE_OF(future<T>, Futures)...)
composed_future<std::array<T,Dim>>::composed_future(Futures&& ... i_futures)
: composed_future(index_seq{}, ddk::promise<std::array<T, Dim>>{}, std::forward<Futures>(i_futures)...)
{
}
template<typename T, size_t Dim>
template<size_t ... Indexs, typename ... Futures>
composed_future<std::array<T,Dim>>::composed_future(const mpl::sequence<Indexs...>& i_seq, ddk::promise<std::array<T, Dim>> i_promise, Futures&& ... i_futures)
: future<std::array<T,Dim>>(i_promise.get_future())
{
	ddk::distributed_reference_wrapper<future_data> futureData = ddk::make_distributed_reference<future_data>(std::move(i_promise));

	const bool _ = (place_future<Indexs,Indexs...>(std::forward<Futures>(i_futures),futureData) && ...);
}
template<typename T, size_t Dim>
template<size_t Index, size_t ... Indexs, typename Future>
bool composed_future<std::array<T,Dim>>::place_future(Future&& i_future, ddk::distributed_reference_wrapper<future_data> i_futureData)
{
	std::forward<Future>(i_future).then(ddk::make_function([i_futureData](T i_value) mutable
	{
		i_futureData->m_values[Index].template construct<T>(std::move(i_value));

		if (++i_futureData->m_resolvedValues == Dim)
		{
			i_futureData->m_promise.set_value({ { std::move(i_futureData->m_values[Indexs]).template extract<T>() ...} });
		}
	}));

	return true;
}

template<typename ... T>
composed_future<std::tuple<T...>>::future_data::future_data(ddk::promise<std::tuple<T...>> i_promise)
: m_promise(std::move(i_promise))
{}
template<typename ... T>
composed_future<std::tuple<T...>>::future_data::~future_data()
{
	destroy_values(index_seq{});
}
template<typename ... T>
template<size_t ... Indexs>
void composed_future<std::tuple<T...>>::future_data::destroy_values(const mpl::sequence<Indexs...>&)
{
	const bool _[] = { m_values.template get<Indexs>().template destroy<typename mpl::nth_type_of<Indexs,T...>::type>() ... };
}
template<typename ... T>
TEMPLATE(typename ... Futures)
REQUIRED(IS_BASE_OF(future<T>, Futures)...)
composed_future<std::tuple<T...>>::composed_future(Futures&& ... i_futures)
: composed_future(index_seq{},ddk::promise<std::tuple<T...>>{},std::forward<Futures>(i_futures)...)
{
}
template<typename ... T>
template<size_t ... Indexs, typename ... Futures>
composed_future<std::tuple<T...>>::composed_future(const mpl::sequence<Indexs...>& i_seq, ddk::promise<std::tuple<T...>> i_promise, Futures&& ... i_futures)
: future<std::tuple<T...>>(i_promise.get_future())
{
	ddk::distributed_reference_wrapper<future_data> futureData = ddk::make_distributed_reference<future_data>(std::move(i_promise));

	const bool _ = (place_future<Indexs, Indexs...>(std::forward<Futures>(i_futures),futureData) && ...);
}
template<typename ... T>
template<size_t Index, size_t ... Indexs, typename Future>
bool composed_future<std::tuple<T...>>::place_future(Future&& i_future, ddk::distributed_reference_wrapper<future_data> i_futureData)
{
	std::forward<Future>(i_future).then(ddk::make_function([i_futureData](typename Future::value_type i_value) mutable
	{
		i_futureData->m_values.template get<Index>().template construct<typename Future::value_type>(std::move(i_value));

		if (++i_futureData->m_resolvedValues == Dim)
		{
			i_futureData->m_promise.set_value({ std::move(i_futureData->m_values).template extract<Indexs>().template extract<T>() ...});
		}
	}));

	return true;
}

}