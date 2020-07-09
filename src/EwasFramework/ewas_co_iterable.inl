

namespace ewas
{

template<typename T>
co_iterator<T>::co_iterator(async_execute_shared_ptr<T> i_executor)
: m_executor(i_executor)
{
}
template<typename T>
co_iterator<T>::co_iterator(const detail::none_t&)
{
}
template<typename T>
typename co_iterator<T>::reference co_iterator<T>::operator*()
{
	return m_executor->get_value();
}
template<typename T>
typename co_iterator<T>::const_reference co_iterator<T>::operator*() const
{
	return m_executor->get_value();
}
template<typename T>
co_iterator<T>& co_iterator<T>::operator++()
{
	typedef typename async_executor<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	start_result execRes = m_executor->execute();

	if(execRes.hasError())
	{
		 start_error execError = execRes.getError();

		if(execError == async_executor<T>::AlreadyDone)
		{
			m_executor.clear();
		}
	}

	return *this;
}
template<typename T>
bool co_iterator<T>::operator!=(const co_iterator<T>& other) const
{
	if(m_executor && other.m_executor)
	{
		return m_executor->get_value() != other.m_executor->get_value();
	}
	else
	{
		return m_executor.get() != other.m_executor.get();
	}
}
template<typename T>
bool co_iterator<T>::operator==(const co_iterator<T>& other) const
{
	if(m_executor && other.m_executor)
	{
		return m_executor->get_value() == other.m_executor->get_value();
	}
	else
	{
		return m_executor.get() == other.m_executor.get();
	}
}

template<typename Iterable>
typename Iterable::reference forward_iterator_awaitable(Iterable& i_iterable)
{
	auto&& itNext = std::begin(i_iterable);

	while(itNext != std::end(i_iterable))
	{
		yield(*itNext);

		itNext = std::next(itNext);
	}

	suspend();

	return ewas::crash_on_return<typename Iterable::reference>::value();
}
template<typename Iterable>
typename Iterable::reference backward_iterator_awaitable(Iterable& i_iterable)
{
	//JAUME: current gcc does not support std::rebgin/rend. Pending to add ewas::begin/end/rbegin/rend wich fulfills this lack of api
	static_assert(sizeof(Iterable)==0,"add ewas::rbegin, ewas::rend");
	auto&& itNext = std::begin(i_iterable);

	while(itNext != std::end(i_iterable))
	{
		yield(*itNext);

		std::next(itNext);
	}

	suspend();

	return ewas::crash_on_return<typename Iterable::reference>::value();
}

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable)
{
	return { std::function<typename Iterable::reference(Iterable&)>([](Iterable& i_iterable) -> typename Iterable::reference { return forward_iterator_awaitable(i_iterable); }),i_iterable };
}
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(const Iterable& i_iterable)
{
	return { std::function<typename Iterable::reference(Iterable&)>([](Iterable& i_iterable) -> typename  Iterable::reference { return backward_iterator_awaitable(i_iterable); }),i_iterable };
}

}
