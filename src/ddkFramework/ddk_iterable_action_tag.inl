
namespace ddk
{

template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
add_action_tag<T>::add_action_tag(Args&& ... i_args)
: m_payload(std::forward<Args>(i_args)...)
{
}
template<typename T>
T add_action_tag<T>::extract()&&
{
	return std::move(m_payload);
}

}