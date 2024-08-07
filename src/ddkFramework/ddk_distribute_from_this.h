//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lend_from_this.h"

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
inline shared_reference_wrapper_impl<T,ReferenceCounter> __make_shared_reference(T*,const tagged_pointer<ReferenceCounter>&, const tagged_pointer_deleter&);

}

template<typename T, typename TT = T>
class distribute_from_this : public lend_from_this<T,TT>
{
	template<typename TTT,typename TTTT>
	friend inline distributed_reference_wrapper<TTTT> distribute(distribute_from_this<TTT,TTTT>& i_distFromThis);
	template<typename TTT,typename TTTT>
	friend inline distributed_reference_wrapper<const TTTT> distribute(const distribute_from_this<TTT,TTTT>& i_distFromThis);
	template<typename TTT>
	friend inline distributed_reference_wrapper<TTT> as_distributed_reference(TTT* i_ptr);
	template<typename TTT, typename Deleter>
	friend inline distributed_reference_wrapper<TTT> as_distributed_reference(TTT* i_ptr, const Deleter& i_refDeleter);
	template<typename TTT, typename ReferenceCounter>
	friend inline detail::shared_reference_wrapper_impl<TTT,ReferenceCounter> detail::__make_shared_reference(TTT*,const tagged_pointer<ReferenceCounter>&);

public:
	struct __distributed_type_tag;
	typedef tagged_pointer<distributed_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	distribute_from_this() = default;
	distribute_from_this(tagged_reference_counter i_refCounter);
	distribute_from_this(const distribute_from_this&) = default;
	distribute_from_this(distribute_from_this&&) = delete;
	~distribute_from_this() = default;

	distribute_from_this& operator=(const distribute_from_this&) = delete;
	distribute_from_this& operator=(distribute_from_this&&) = delete;

protected:
	inline ddk::distributed_reference_wrapper<TT> distributed_from_this();
	inline ddk::distributed_reference_wrapper<const TT> distributed_from_this() const;
	inline tagged_reference_counter get_reference_counter() const;
	inline void set_reference_counter(tagged_reference_counter i_refCounter) const;

private:
	mutable tagged_reference_counter m_refCounter;
};

}

#include "ddk_distribute_from_this.inl"
