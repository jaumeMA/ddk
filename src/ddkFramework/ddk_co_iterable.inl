
namespace ddk
{

template<typename Iterable>
typename Iterable::reference forward_iterator_awaitable(Iterable& i_iterable, size_t i_initPos)
{
	auto&& itNext = std::begin(i_iterable) + i_initPos;

	while(itNext != std::end(i_iterable))
	{
		yield(*itNext);

		itNext = std::next(itNext);
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}
template<typename Iterable>
typename Iterable::reference backward_iterator_awaitable(Iterable& i_iterable, size_t i_initPos)
{
	//JAUME: current gcc does not support std::rebgin/rend. Pending to add ddk::begin/end/rbegin/rend wich fulfills this lack of api
	static_assert(sizeof(Iterable)==0,"add ddk::rbegin, ddk::rend");
	auto&& itNext = std::begin(i_iterable) + i_initPos;

	while(itNext != std::end(i_iterable))
	{
		yield(*itNext);

		std::next(itNext);
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}

template<typename Iterable>
co_iterable<Iterable>::co_iterable(const std::function<reference(size_t)>& i_function)
: m_function(i_function)
{
}
template<typename Iterable>
typename co_iterable<Iterable>::iterator co_iterable<Iterable>::begin()
{
	return iterator(m_function);
}
template<typename Iterable>
typename co_iterable<Iterable>::iterator co_iterable<Iterable>::end()
{
	return none;
}
template<typename Iterable>
typename co_iterable<Iterable>::const_iterator co_iterable<Iterable>::begin() const
{
	return const_iterator(m_function);
}
template<typename Iterable>
typename co_iterable<Iterable>::const_iterator co_iterable<Iterable>::end() const
{
	return none;
}

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable)
{
	typedef typename co_iterable<Iterable>::reference reference;

	return [&i_iterable](size_t i_index) -> reference { return forward_iterator_awaitable(i_iterable, i_index);  };
}
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(Iterable& i_iterable)
{
	typedef typename co_iterable<Iterable>::reference reference;

	return [&i_iterable](size_t i_index) -> reference { return backward_iterator_awaitable(i_iterable, i_index);  };
}
template<typename Iterable>
co_iterable<const Iterable> co_iterate(const Iterable& i_iterable)
{
	typedef typename co_iterable<Iterable>::const_reference const_reference;

	return [&i_iterable](size_t i_index) -> const_reference { return forward_iterator_awaitable(i_iterable, i_index);  };
}
template<typename Iterable>
co_iterable<const Iterable> co_reverse_iterate(const Iterable& i_iterable)
{
	typedef typename co_iterable<Iterable>::const_reference const_reference;

	return [&i_iterable](size_t i_index) -> const_reference { return backward_iterator_awaitable(i_iterable, i_index);  };
}

}