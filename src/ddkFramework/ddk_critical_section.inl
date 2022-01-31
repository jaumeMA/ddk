
namespace ddk
{
namespace detail
{

template<typename Traits>
critical_section_impl<Traits,false>::critical_section_impl(const empty_critical_section_impl_t&)
: m_callContext(reinterpret_cast<access_call_context*>(0xDEAD))
, m_id(k_invalidCriticalAccessIdentifier)
, m_context(context_unacquired)
{
}
template<typename Traits>
critical_section_impl<Traits,false>::critical_section_impl(access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
: m_callContext(&i_callContext)
, m_id(i_id)
, m_context(std::move(i_context))
{
}
template<typename Traits>
critical_section_impl<Traits,false>::critical_section_impl(critical_section_impl&& other)
: m_callContext(other.m_callContext)
, m_id(other.m_id)
, m_context(std::move(other.m_context))
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;
}
template<typename Traits>
critical_section_impl<Traits,false>::~critical_section_impl()
{
	//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
	if(*m_context)
	{
		MAKE_IT_CRASH
	}
}
template<typename Traits>
critical_section_impl<Traits,false>& critical_section_impl<Traits,false>::operator=(critical_section_impl&& other)
{
	m_callContext = other.m_callContext;
	m_id = other.m_id;

	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;

	m_context = std::move(other.m_context);

	return *this;
}
template<typename Traits>
typename critical_section_impl<Traits,false>::const_critical_call_context critical_section_impl<Traits,false>::operator->() const
{
	return m_callContext->create_call_context(ddk::lend(m_context));
}
template<typename Traits>
typename critical_section_impl<Traits,false>::critical_call_context critical_section_impl<Traits,false>::operator->()
{
	return m_callContext->create_call_context(ddk::lend(m_context));
}
template<typename Traits>
typename critical_section_impl<Traits,false>::const_critical_call_context critical_section_impl<Traits,false>::get_call_context() const
{
	return m_callContext->create_call_context(ddk::lend(m_context));
}
template<typename Traits>
typename critical_section_impl<Traits,false>::provider_interface& critical_section_impl<Traits,false>::get_provider()
{
	return m_callContext->get_provider();
}
template<typename Traits>
bool critical_section_impl<Traits,false>::empty() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_impl<Traits,false>::operator bool() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_identifier critical_section_impl<Traits,false>::get_id() const
{
	return m_id;
}
template<typename Traits>
typename critical_section_impl<Traits,false>::critical_context critical_section_impl<Traits,false>::extract_context()
{
	return std::move(*m_context);
}

template<typename Traits>
critical_section_impl<Traits,true>::critical_section_impl(const empty_critical_section_impl_t&)
: m_id(k_invalidCriticalAccessIdentifier)
, m_context(context_unacquired)
, m_provider(nullptr)
{
}
template<typename Traits>
critical_section_impl<Traits,true>::critical_section_impl(access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
: m_id(i_id)
, m_context(std::move(i_context))
, m_provider(i_callContext.get_provider())
{
}
template<typename Traits>
critical_section_impl<Traits,true>::critical_section_impl(critical_section_impl&& other)
: m_id(other.m_id)
, m_context(std::move(other.m_context))
, m_provider(nullptr)
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	std::swap(m_provider,other.m_provider);
}
template<typename Traits>
critical_section_impl<Traits,true>::~critical_section_impl()
{
	//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
	if(*m_context)
	{
		MAKE_IT_CRASH
	}
}
template<typename Traits>
critical_section_impl<Traits,true>& critical_section_impl<Traits,true>::operator=(critical_section_impl&& other)
{
	m_id = other.m_id;
	m_provider = other.m_provider;

	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_provider = nullptr;

	m_context = std::move(other.m_context);

	return *this;
}
template<typename Traits>
typename critical_section_impl<Traits,true>::provider_interface* critical_section_impl<Traits,true>::operator->() const
{
	return m_provider;
}
template<typename Traits>
typename critical_section_impl<Traits,true>::provider_interface* critical_section_impl<Traits,true>::operator->()
{
	return m_provider;
}
template<typename Traits>
bool critical_section_impl<Traits,true>::empty() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_impl<Traits,true>::operator bool() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_identifier critical_section_impl<Traits,true>::get_id() const
{
	return m_id;
}
template<typename Traits>
typename critical_section_impl<Traits,true>::critical_context critical_section_impl<Traits,true>::extract_context()
{
	return std::move(*m_context);
}

template<typename Traits>
const_critical_section_impl<Traits,false>::const_critical_section_impl(const empty_critical_section_impl_t&)
: m_callContext(reinterpret_cast<const access_call_context*>(0xDEAD))
, m_id(critical_section_identifier(0))
, m_context(context_unacquired)
{
}
template<typename Traits>
const_critical_section_impl<Traits,false>::const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
: m_callContext(&i_callContext)
, m_id(i_id)
, m_context(std::move(i_context))
{
}
template<typename Traits>
const_critical_section_impl<Traits,false>::const_critical_section_impl(const_critical_section_impl&& other)
: m_callContext(other.m_callContext)
, m_id(other.m_id)
, m_context(std::move(other.m_context))
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;
}
template<typename Traits>
const_critical_section_impl<Traits,false>::const_critical_section_impl(critical_section_impl<Traits,false>&& other)
: m_callContext(other.m_callContext)
, m_id(other.m_id)
, m_context(std::move(other.m_context))
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;
}
template<typename Traits>
const_critical_section_impl<Traits,false>::~const_critical_section_impl()
{
	//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
	if(*m_context)
	{
		MAKE_IT_CRASH
	}
}
template<typename Traits>
const_critical_section_impl<Traits,false>& const_critical_section_impl<Traits,false>::operator=(const_critical_section_impl&& other)
{
	m_callContext = other.m_callContext;
	m_id = other.m_id;
	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;

	m_context = std::move(other.m_context);

	return *this;
}
template<typename Traits>
typename const_critical_section_impl<Traits,false>::const_critical_call_context const_critical_section_impl<Traits,false>::operator->() const
{
	return m_callContext->create_call_context(ddk::lend(m_context));
}
template<typename Traits>
const typename const_critical_section_impl<Traits,false>::provider_interface& const_critical_section_impl<Traits,false>::get_provider() const
{
	return m_callContext->get_provider();
}
template<typename Traits>
bool const_critical_section_impl<Traits,false>::empty() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
const_critical_section_impl<Traits,false>::operator bool() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_identifier const_critical_section_impl<Traits,false>::get_id() const
{
	return m_id;
}
template<typename Traits>
typename const_critical_section_impl<Traits,false>::critical_context const_critical_section_impl<Traits,false>::extract_context()
{
	return std::move(*m_context);
}

template<typename Traits>
const_critical_section_impl<Traits,true>::const_critical_section_impl(const empty_critical_section_impl_t&)
: m_id(critical_section_identifier(0))
, m_context(context_unacquired)
, m_provider(nullptr)
{
}
template<typename Traits>
const_critical_section_impl<Traits,true>::const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context)
: m_id(i_id)
, m_context(std::move(i_context))
, m_provider(i_callContext.get_provider())
{
}
template<typename Traits>
const_critical_section_impl<Traits,true>::const_critical_section_impl(const_critical_section_impl&& other)
: m_id(other.m_id)
, m_context(std::move(other.m_context))
, m_provider(nullptr)
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	std::swap(m_provider,other.m_provider);
}
template<typename Traits>
const_critical_section_impl<Traits,true>::const_critical_section_impl(critical_section_impl<Traits,true>&& other)
: m_id(other.m_id)
, m_context(std::move(other.m_context))
, m_provider(nullptr)
{
	other.m_id = k_invalidCriticalAccessIdentifier;
	std::swap(m_provider,other.m_provider);
}
template<typename Traits>
const_critical_section_impl<Traits,true>::~const_critical_section_impl()
{
	//IF YOU RAISED THIS CRASH IS BECAUSE YOU FORGOT TO LEAVE THIS CRITICAL SECTION, I.E. YOU FORGOT TO ABANDON THIS CRITICAL SECTION FOR OTHER PEOPLER USAGE, GO AND PUT IT!!
	if(*m_context)
	{
		MAKE_IT_CRASH
	}
}
template<typename Traits>
const_critical_section_impl<Traits,true>& const_critical_section_impl<Traits,true>::operator=(const_critical_section_impl&& other)
{
	m_id = other.m_id;
	m_provider = other.m_provider;

	other.m_id = k_invalidCriticalAccessIdentifier;
	other.m_callContext = nullptr;

	m_context = std::move(other.m_context);

	return *this;
}
template<typename Traits>
const typename const_critical_section_impl<Traits,true>::provider_interface* const_critical_section_impl<Traits,true>::operator->() const
{
	return m_provider;
}
template<typename Traits>
bool const_critical_section_impl<Traits,true>::empty() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
const_critical_section_impl<Traits,true>::operator bool() const
{
	return static_cast<bool>(*m_context);
}
template<typename Traits>
critical_section_identifier const_critical_section_impl<Traits,true>::get_id() const
{
	return m_id;
}
template<typename Traits>
typename const_critical_section_impl<Traits,true>::critical_context const_critical_section_impl<Traits,true>::extract_context()
{
	return std::move(*m_context);
}

}
}