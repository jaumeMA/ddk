
namespace ddk
{

template<typename Sink>
sink_action_tag<Sink>::sink_action_tag(const Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
sink_action_tag<Sink>::sink_action_tag(Sink&& i_sink)
: m_sink(std::move(i_sink))
{
}
template<typename Sink>
template<typename T>
sink_action_tag<Sink>::sink_action_tag(const sink_action_tag<function<void(T)>>& other)
: m_sink(other.m_sink.template callable<Sink>())
{
}
template<typename Sink>
template<typename T>
T&& sink_action_tag<Sink>::operator()(T&& i_value) const
{
	ddk::terse_eval(m_sink,std::forward<T>(i_value));

	return std::forward<T>(i_value);
}

template<typename T>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,T))
sink_action_tag<function<void(T)>>::sink_action_tag(Sink&& i_sink)
: m_sink(std::forward<Sink>(i_sink))
{
}
template<typename T>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,T))
sink_action_tag<function<void(T)>>::sink_action_tag(const sink_action_tag<Sink>& i_sink)
: m_sink(i_sink.m_sink)
{
}
template<typename T>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,T))
sink_action_tag<function<void(T)>>::sink_action_tag(sink_action_tag<Sink>&& i_sink)
: m_sink(std::move(i_sink.m_sink))
{
}
template<typename T>
template<typename TT>
TT&& sink_action_tag<function<void(T)>>::operator()(TT&& i_value) const
{
	ddk::eval(m_sink,std::forward<TT>(i_value));

	return std::forward<TT>(i_value);
}

template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
add_action_tag<T>::add_action_tag(Args&& ... i_args)
: m_payload(std::forward<Args>(i_args)...)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_CONVERTIBLE(T,TT))
add_action_tag<T>::operator add_action_tag<TT>() const
{
	return m_payload;
}
template<typename T>
const T& add_action_tag<T>::get() const
{
	return std::move(m_payload);
}
template<typename T>
T add_action_tag<T>::extract()&&
{
	return std::move(m_payload);
}

}