
namespace ddk
{
namespace detail
{

template<typename T,typename TT>
embedded_private_async_state<T,TT>::embedded_private_async_state()
	: m_refCounter(nullptr,*this)
{
}
template<typename T,typename TT>
embedded_private_async_state<T,TT>::~embedded_private_async_state()
{
	this->detach();

	DDK_ASSERT(m_refCounter.hasSharedReferences() == false,"Pending shared references while destroying embedded async state");
}
template<typename T,typename TT>
template<typename ... Args>
TT& embedded_private_async_state<T,TT>::attach(Args&& ... i_args)
{
	typedef ddk::tagged_pointer<distributed_reference_counter> tagged_reference_counter;

	m_arena.construct<TT>(std::forward<Args>(i_args)...);

	m_refCounter = distributed_async_control_block(m_arena.get_ptr<TT>(),*this);

	private_async_state<T>::attach(ddk::as_distributed_reference(m_arena.get_ptr<TT>(),tagged_reference_counter{ &m_refCounter,ddk::ReferenceAllocationType::Embedded }));

	return m_arena.get<TT>();
}
template<typename T,typename TT>
void embedded_private_async_state<T,TT>::deallocate(TT* i_ptr) const
{
	if (m_arena.get_ptr<TT>() == i_ptr)
	{
		m_arena.destroy<TT>();
	}
	else
	{
		throw async_exception("Mismatch between embedded async and to delete async addresses");
	}
}

}
}