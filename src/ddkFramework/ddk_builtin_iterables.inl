
namespace ddk
{
namespace detail
{

template<typename Generator>
__numbers_iterable<Generator>::__numbers_iterable(const Generator& i_generator)
: m_generator(i_generator)
{
}
template<typename Generator>
template<typename GGenerator>
__numbers_iterable<GGenerator> __numbers_iterable<Generator>::operator()(const GGenerator& i_generator) const
{
    return { i_generator };
}
template<typename Generator>
auto __numbers_iterable<Generator>::operator()(const value_type& i_value) const
{
    return ddk::eval(m_generator,i_value);
}

}

template<typename Generator>
iterable_adaptor<const detail::__numbers_iterable<Generator>>::iterable_adaptor(const detail::__numbers_iterable<Generator>& i_generator)
: m_generator(i_generator)
{
}
template<typename T>
bool iterable_adaptor<const detail::__numbers_iterable<T>>::valid() const
{
    return true;
}
template<typename T>
typename iterable_adaptor<const detail::__numbers_iterable<T>>::reference iterable_adaptor<const detail::__numbers_iterable<T>>::get_value()
{
    return m_currValue;
}
template<typename T>
typename iterable_adaptor<const detail::__numbers_iterable<T>>::const_reference iterable_adaptor<const detail::__numbers_iterable<T>>::get_value() const
{
    return m_currValue;
}
template<typename T>
template<typename Sink>
auto iterable_adaptor<const detail::__numbers_iterable<T>>::forward_value(Sink&& i_sink)
{
    return ddk::eval(std::forward<Sink>(i_sink),m_currValue);
}
template<typename T>
template<typename Sink>
auto iterable_adaptor<const detail::__numbers_iterable<T>>::forward_value(Sink&& i_sink) const
{
    return ddk::eval(std::forward<Sink>(i_sink),m_currValue);
}
template<typename T>
bool iterable_adaptor<const detail::__numbers_iterable<T>>::perform_action(const begin_action_tag&) const
{
    m_currValue = 0;

    return true;
}
template<typename T>
bool iterable_adaptor<const detail::__numbers_iterable<T>>::perform_action(const forward_action_tag&) const
{
    m_currValue = ddk::eval(m_generator,m_currValue);

    return true;
}

}