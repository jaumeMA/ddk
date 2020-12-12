#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_reference_exception.h"

namespace ddk
{

template<typename T, typename TT = T>
class share_from_this
{
	template<typename TTT, typename TTTT>
	friend inline shared_reference_wrapper<TTTT> share(share_from_this<TTT,TTTT>&);
	template<typename TTT,typename TTTT>
	friend inline shared_reference_wrapper<const TTTT> share(const share_from_this<TTT,TTTT>&);

public:
	struct __shared_type_tag;

	share_from_this() = default;
	share_from_this(const share_from_this&) = delete;
	share_from_this(share_from_this&&);

	share_from_this& operator=(const share_from_this&) = delete;
	share_from_this& operator=(share_from_this&&) = delete;

	inline const IReferenceWrapperDeleter* set_deleter(const IReferenceWrapperDeleter*);
	inline shared_reference_counter* get_reference_counter();
	inline const shared_reference_counter* get_reference_counter() const;

protected:
	inline ddk::shared_reference_wrapper<TT> ref_from_this();
	inline ddk::shared_reference_wrapper<const TT> ref_from_this() const;
	inline ddk::shared_reference_wrapper<TT> unsafe_ref_from_this();
	inline ddk::shared_reference_wrapper<const TT> unsafe_ref_from_this() const;

private:
	mutable shared_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter = nullptr;
};

}

#include "ddk_shared_from_this.inl"