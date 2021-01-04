#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_reference_exception.h"

namespace ddk
{
namespace detail
{

template<typename T>
inline shared_reference_wrapper_impl<T,true> __make_shared_reference(T*,const tagged_pointer<shared_reference_counter>&,const IReferenceWrapperDeleter*);

}

template<typename T, typename TT = T>
class share_from_this
{
	template<typename TTT, typename TTTT>
	friend inline shared_reference_wrapper<TTTT> share(share_from_this<TTT,TTTT>&);
	template<typename TTT,typename TTTT>
	friend inline shared_reference_wrapper<const TTTT> share(const share_from_this<TTT,TTTT>&);
	template<typename TTT>
	friend inline detail::shared_reference_wrapper_impl<TTT,true> detail::__make_shared_reference(TTT*,const tagged_pointer<shared_reference_counter>&,const IReferenceWrapperDeleter*);
	friend inline weak_pointer_wrapper<TT> weak(share_from_this& i_sharedFromThis)
	{
		return as_shared_reference(static_cast<TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
	}
	friend inline weak_pointer_wrapper<const TT> weak(const share_from_this& i_sharedFromThis)
	{
		return as_shared_reference(static_cast<const TT*>(&i_sharedFromThis),i_sharedFromThis.m_refCounter,i_sharedFromThis.m_deleter);
	}

public:
	struct __shared_type_tag;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	share_from_this() = default;
	share_from_this(const share_from_this&) = default;
	share_from_this(share_from_this&&) = delete;

	share_from_this& operator=(const share_from_this&) = delete;
	share_from_this& operator=(share_from_this&&) = delete;


protected:
	inline shared_reference_wrapper<TT> ref_from_this();
	inline shared_reference_wrapper<const TT> ref_from_this() const;

private:		
	inline const IReferenceWrapperDeleter* set_deleter(const IReferenceWrapperDeleter* i_refDeleter) const;
	inline void set_reference_counter(const tagged_reference_counter& i_refCounter) const;

	mutable tagged_reference_counter m_refCounter;
	mutable const IReferenceWrapperDeleter* m_deleter = nullptr;
};

}

#include "ddk_shared_from_this.inl"