
namespace ddk
{

template<typename ... Types>
NO_DISCARD_RETURN connection signal<void(Types...)>::connect(const ddk::function<void(Types...)>& i_function) const
{
	return m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this));
}
template<typename ... Types>
void signal<void(Types...)>::disconnect()
{
	m_callers.clear();
}
template<typename ... Types>
template<typename ... Args>
void signal<void(Types...)>::execute(Args&& ... i_args) const
{
	typename linked_list<signal_functor_t>::const_iterator itCaller = m_callers.cbegin();
	for(; itCaller != m_callers.cend(); ++itCaller)
	{
		if(itCaller->is_enabled())
		{
			itCaller->execute(std::forward<Args>(i_args) ...);
		}
	}
}
template<typename ... Types>
bool signal<void(Types...)>::disconnect(const detail::connection_base& i_base) const
{
	typedef typename linked_list<signal_functor_t>::iterator iterator;
	iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_base](const signal_functor_t& i_functor) { return i_functor.get_id() == i_base.get_id(); });
	if(itCaller != m_callers.end())
	{
		m_callers.erase(std::move(itCaller));

		return true;
	}
	else
	{
		return false;
	}
}

}