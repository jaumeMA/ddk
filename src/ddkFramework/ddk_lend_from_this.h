#pragma once

#include "ddk_reference_counter.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_macros.h"
#include "ddk_reference_tag.h"
#include "ddk_point_from_this.h"
#include <type_traits>

namespace ddk
{

template<typename T, typename TT = T>
class lend_from_this
{
	typedef TT interface_type;

	template<typename TTT, typename TTTT>
	friend inline lent_reference_wrapper<TTTT> lend(lend_from_this<TTT,TTTT>&);
	template<typename TTT,typename TTTT>
	friend inline lent_reference_wrapper<const TTTT> lend(const lend_from_this<TTT,TTTT>&);

public:
	lend_from_this()
	{
	}
	lend_from_this(const lend_from_this& other)
	{
	}
	~lend_from_this()
	{
		DDK_ASSERT(m_counter.hasLentReferences() == false, "Still lent references alive while destroying lend_from_this");
	}

protected:
#ifdef DDK_DEBUG
	inline ddk::lent_reference_wrapper<TT> lent_from_this()
	{
		return ddk::lent_reference_wrapper<TT>(static_cast<T*>(this),&m_counter);
	}
	inline ddk::lent_reference_wrapper<const TT> lent_from_this() const
	{
		return ddk::lent_reference_wrapper<const TT>(static_cast<typename std::add_const<T>::type*>(this),&m_counter);
	}
#else
	inline ddk::lent_reference_wrapper<TT> lent_from_this()
	{
		return ddk::lent_reference_wrapper<TT>(static_cast<T*>(this));
	}
	inline ddk::lent_reference_wrapper<const TT> lent_from_this() const
	{
		return ddk::lent_reference_wrapper<const TT>(static_cast<typename std::add_const<T>::type*>(this));
	}
#endif

private:
#ifdef DDK_DEBUG
	mutable ddk::lent_reference_counter m_counter;
#endif
};

template<typename T>
class point_from_this<lent_pointer_wrapper<T>> : public lend_from_this<T,T>
{
public:
	using lend_from_this<T,T>::lend_from_this;
};

}