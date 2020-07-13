#pragma once

#include "reference_counter.h"
#include "lent_reference_wrapper.h"
#include "ddk_macros.h"
#include "reference_tag.h"
#include <type_traits>

namespace ddk
{

template<typename T, typename TT>
class lend_from_this
{
	typedef TT interface_type;

	template<typename TTT>
	friend inline lent_reference_wrapper<typename TTT::interface_type> lend(TTT& i_lendable);
	template<typename TTT>
	friend inline lent_reference_wrapper<const typename TTT::interface_type> lend(const TTT& i_lendable);

public:
	lend_from_this()
	{
		m_counter.addStrongReference();
	}
	lend_from_this(const lend_from_this& other)
	{
		m_counter.addStrongReference();
	}
	~lend_from_this()
	{
		m_counter.removeStrongReference();

		EWAS_ASSERT(m_counter.hasWeakReferences() == false, "Still lent references alive while destroying lend_from_this");
	}
	inline lent_reference_wrapper<TT> lend()
	{
		return ref_from_this<TT>(*static_cast<T*>(this));
	}
	inline lent_reference_wrapper<const TT> lend() const
	{
		return ref_from_this<TT>(*static_cast<const T*>(this));
	}

protected:
#ifdef EWAS_DEBUG
	template<typename TTT, typename TTTT>
	inline ewas::lent_reference_wrapper<TTT> ref_from_this(TTTT& i_lendable)
	{
		return ewas::lent_reference_wrapper<TTT>(static_cast<TTT*>(&i_lendable),&m_counter);
	}
	template<typename TTT, typename TTTT>
	inline ewas::lent_reference_wrapper<const TTT> ref_from_this(const TTTT& i_lendable) const
	{
		return ewas::lent_reference_wrapper<const TTT>(static_cast<typename std::add_const<TTT>::type*>(&i_lendable),&m_counter);
	}
#else
	template<typename TTT, typename TTTT>
	inline ewas::lent_reference_wrapper<TTT> ref_from_this(TTTT& i_lendable)
	{
		return ewas::lent_reference_wrapper<TTT>(static_cast<TTT*>(&i_lendable));
	}
	template<typename TTT, typename TTTT>
	inline ewas::lent_reference_wrapper<const TTT> ref_from_this(const TTTT& i_lendable) const
	{
		return ewas::lent_reference_wrapper<const TTT>(static_cast<typename std::add_const<TTT>::type*>(&i_lendable));
	}
#endif

private:
	mutable ewas::unique_reference_counter m_counter;	
};

}
