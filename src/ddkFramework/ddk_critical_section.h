#pragma once

#include "ddk_macros.h"
#include <cstdlib>
#include "ddk_critical_section_context.h"
#include "ddk_critical_section_defs.h"
#include "ddk_iaccess_critical_call_context.h"
#include "ddk_intrusive_node.h"
#include "ddk_lendable.h"
#include "ddk_reference_wrapper.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

struct empty_critical_section_impl_t
{};

template<typename,bool>
class critical_section_impl;

template<typename Traits>
class critical_section_impl<Traits,false>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename access_call_context::critical_call_context critical_call_context;
	typedef typename access_call_context::const_critical_call_context const_critical_call_context;
	template<typename TTraits,bool>
	friend class const_critical_section_impl;

public:
	critical_section_impl(const empty_critical_section_impl_t&);
	explicit critical_section_impl(access_call_context& i_callContext, critical_section_identifier i_id,critical_context i_context);
	critical_section_impl(const critical_section_impl&) = delete;
	critical_section_impl(critical_section_impl&& other);
	~critical_section_impl();
	critical_section_impl& operator=(const critical_section_impl& other) = delete;
	critical_section_impl& operator=(critical_section_impl&& other);
	inline const_critical_call_context operator->() const;
	inline critical_call_context operator->();
	const_critical_call_context get_call_context() const;
	provider_interface& get_provider();
	bool empty() const;
	operator bool() const;
	critical_section_identifier get_id() const;
	critical_context extract_context();

private:
	access_call_context* m_callContext;
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
};

template<typename Traits>
class critical_section_impl<Traits,true>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	template<typename TTraits,bool>
	friend class const_critical_section_impl;

public:
	critical_section_impl(const empty_critical_section_impl_t&);
	explicit critical_section_impl(access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context);
	critical_section_impl(const critical_section_impl&) = delete;
	critical_section_impl(critical_section_impl&& other);
	~critical_section_impl();
	critical_section_impl& operator=(const critical_section_impl& other) = delete;
	critical_section_impl& operator=(critical_section_impl&& other);
	inline provider_interface* operator->() const;
	inline provider_interface* operator->();
	bool empty() const;
	operator bool() const;
	critical_section_identifier get_id() const;
	critical_context extract_context();

private:
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
	provider_interface* m_provider;
};

template<typename, bool>
class const_critical_section_impl;

template<typename Traits>
class const_critical_section_impl<Traits,false>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;
	typedef typename access_call_context::const_critical_call_context const_critical_call_context;

public:
	const_critical_section_impl(const empty_critical_section_impl_t&);
	explicit const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context);
	const_critical_section_impl(const const_critical_section_impl& other) = delete;
	const_critical_section_impl(const_critical_section_impl&& other);
	const_critical_section_impl(critical_section_impl<Traits,false>&& other);
	~const_critical_section_impl();
	const_critical_section_impl& operator=(const const_critical_section_impl& other) = delete;
	const_critical_section_impl& operator=(const_critical_section_impl&& other);
	inline const_critical_call_context operator->() const;
	inline const provider_interface& get_provider() const;
	bool empty() const;
	operator bool() const;
	critical_section_identifier get_id() const;
	critical_context extract_context();

private:
	const access_call_context* m_callContext;
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
};

template<typename Traits>
class const_critical_section_impl<Traits,true>
{
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef IAccessCriticalCallContext<Traits> access_call_context;

public:
	const_critical_section_impl(const empty_critical_section_impl_t&);
	explicit const_critical_section_impl(const access_call_context& i_callContext,critical_section_identifier i_id,critical_context i_context);
	const_critical_section_impl(const const_critical_section_impl& other) = delete;
	const_critical_section_impl(const_critical_section_impl&& other);
	const_critical_section_impl(critical_section_impl<Traits,true>&& other);
	~const_critical_section_impl();
	const_critical_section_impl& operator=(const const_critical_section_impl& other) = delete;
	const_critical_section_impl& operator=(const_critical_section_impl&& other);
	inline const provider_interface* operator->() const;
	bool empty() const;
	operator bool() const;
	critical_section_identifier get_id() const;
	critical_context extract_context();

private:
	critical_section_identifier m_id;
	mutable ddk::lendable<critical_context> m_context;
	const provider_interface* m_provider;
};

}

template<typename Traits>
using const_critical_section = detail::const_critical_section_impl<Traits,std::is_same<typename Traits::critical_context,ddk::critical_section_context>::value>;
template<typename Traits>
using critical_section = detail::critical_section_impl<Traits,std::is_same<typename Traits::critical_context,ddk::critical_section_context>::value>;

const detail::empty_critical_section_impl_t empty_critical_section_impl = detail::empty_critical_section_impl_t();

}

#include "ddk_critical_section.inl"