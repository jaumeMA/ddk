
namespace ddk
{
namespace detail
{

template<typename T>
__numbers_iterable<T>::__numbers_iterable(const function<T(const T&)>& i_generator)
: m_generator(i_generator)
{
}
template<typename T>
__numbers_iterable<T> __numbers_iterable<T>::operator()(const function<T(const T&)>& i_generator) const
{
    return { i_generator };
}

}

template<typename T>
iterable_adaptor<const detail::__numbers_iterable<T>>::iterable_adaptor(const detail::__numbers_iterable<T>& i_iterable)
: m_generator(lend(i_iterable))
{
}
template<typename T>
bool iterable_adaptor<const detail::__numbers_iterable<T>>::valid() const
{
    return true;
}
template<typename T>
template<typename Sink>
bool iterable_adaptor<const detail::__numbers_iterable<T>>::init(Sink&& i_sink, const ddk::shift_action& i_initialAction)
{
    m_currValue = static_cast<T>(i_initialAction.shifting());

    i_sink.apply((m_generator != nullptr) ? eval(m_generator,m_currValue) : m_currValue);

    return true;
}
template<typename T>
template<typename Sink>
typename iterable_adaptor<const detail::__numbers_iterable<T>>::difference_type iterable_adaptor<const detail::__numbers_iterable<T>>::forward_next_value_in(Sink&& i_sink)
{
    i_sink.apply((m_generator != nullptr) ? eval(m_generator,++m_currValue) : ++m_currValue);

    return 0;
}
template<typename T>
template<typename Sink>
typename iterable_adaptor<const detail::__numbers_iterable<T>>::difference_type iterable_adaptor<const detail::__numbers_iterable<T>>::forward_prev_value_in(Sink&& i_sink) const
{
    i_sink.apply((m_generator != nullptr) ? eval(m_generator,--m_currValue) : --m_currValue);

    return 0;
}
template<typename T>
template<typename Sink>
typename iterable_adaptor<const detail::__numbers_iterable<T>>::difference_type iterable_adaptor<const detail::__numbers_iterable<T>>::forward_shift_value_in(difference_type i_shift,Sink&& i_sink)
{
    m_currValue += static_cast<T>(i_shift);

    i_sink.apply((m_generator != nullptr) ? eval(m_generator,m_currValue) : m_currValue);

    return 0;
}

}