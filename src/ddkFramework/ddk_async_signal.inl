
namespace ddk
{

template<typename ... Types>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(async_attachable_message_queue<builtn_message_type>,Args...))
async_signal<void(Types...)>::async_signal(Args&& ... i_args)
: m_id(reinterpret_cast<size_t>(this))
, m_msgLoop(m_id)
, m_msgQueue(std::forward<Args>(i_args)...)
{
}
template<typename ... Types>
NO_DISCARD_RETURN connection async_signal<void(Types...)>::connect(const ddk::function<void(Types...)>& i_function) const
{
	return m_msgLoop.connect(make_intrusive(m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this))),ddk::lend(m_msgQueue));
}
template<typename ... Types>
template<typename MessageType>
NO_DISCARD_RETURN connection async_signal<void(Types...)>::connect(const ddk::function<void(Types...)>& i_function,lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
{
	return m_msgLoop.connect(make_intrusive(m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this))),i_messageQueue);
}
template<typename ... Types>
NO_DISCARD_RETURN connection async_signal<void(Types...)>::connect(async_signal<void(Types...)>& other) const
{
	return m_msgLoop.connect(make_intrusive(m_callers.push([&other,this](Types ... i_args){ other.m_msgQueue.dispatch_message(builtn_message_type(other.m_id,std::forward<Types>(i_args) ...)); },static_cast<const detail::signal_connector&>(*this))),ddk::lend(other.m_msgQueue));
}
template<typename ... Types>
void async_signal<void(Types...)>::disconnect()
{
	m_msgLoop.clear();
	m_callers.clear();
}
template<typename ... Types>
template<typename ... Args>
void async_signal<void(Types...)>::execute(Args&& ... i_args) const
{
	m_msgLoop.push_message(builtn_message_type(m_id,std::forward<Args>(i_args) ...));
}
template<typename ... Types>
bool async_signal<void(Types...)>::set_affinity(const cpu_set_t& i_set)
{
	return m_msgQueue.set_affinity(i_set);
}
template<typename ... Types>
bool async_signal<void(Types...)>::disconnect(const detail::connection_base& i_base) const
{
	typedef typename linked_list<intrusive_node<signal_functor_t>>::iterator iterator;
	iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_base](const intrusive_node<signal_functor_t>& i_functor) { return i_functor->get_id() == i_base.get_id(); });

	if(itCaller != m_callers.end())
	{
		m_msgLoop.disconnect((*itCaller).get());

		m_callers.erase(std::move(itCaller));

		return true;
	}
	else
	{
		return false;
	}
}

}