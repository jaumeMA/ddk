
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
template<typename Generator>
template<typename Sink>
iterable_action_tag_result<typename iterable_adaptor<const detail::__numbers_iterable<Generator>>::traits,sink_action_tag<Sink>> iterable_adaptor<const detail::__numbers_iterable<Generator>>::perform_action(const sink_action_tag<Sink>& i_sink) const
{
    return i_sink(m_currValue);
}
template<typename Generator>
iterable_action_tag_result<typename iterable_adaptor<const detail::__numbers_iterable<Generator>>::traits,begin_action_tag> iterable_adaptor<const detail::__numbers_iterable<Generator>>::perform_action(const begin_action_tag&) const
{
    m_currValue = 0;

    return success;
}
template<typename Generator>
iterable_action_tag_result<typename iterable_adaptor<const detail::__numbers_iterable<Generator>>::traits,forward_action_tag> iterable_adaptor<const detail::__numbers_iterable<Generator>>::perform_action(const forward_action_tag&) const
{
    m_currValue = ddk::eval(m_generator,m_currValue);

    return success;
}

}