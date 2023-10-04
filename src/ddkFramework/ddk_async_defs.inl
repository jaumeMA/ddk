
namespace ddk
{

template<typename Payload>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Payload,Args...))
async_event<Payload>::async_event(Args&& ... i_args)
: m_payload(std::forward<Args>(i_args)...)
{
}
template<typename Payload>
Payload async_event<Payload>::extract() &&
{
	return std::move(m_payload).extract();
}
template<typename Payload>
async_event<Payload>::operator bool() const
{
	return static_cast<bool>(m_payload);
}

}