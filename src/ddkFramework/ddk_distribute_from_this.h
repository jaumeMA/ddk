#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_shared_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T>
inline shared_reference_wrapper_impl<T,false> __make_shared_reference(T*,const tagged_pointer<distributed_reference_counter>&,const IReferenceWrapperDeleter*);

}

template<typename T, typename TT = T>
class distribute_from_this
{
	template<typename TT,typename TTT>
	friend inline distributed_reference_wrapper<TTT> distribute(distribute_from_this<TT,TTT>& i_distFromThis);
	template<typename TT,typename TTT>
	friend inline distributed_reference_wrapper<const TTT> distribute(const distribute_from_this<TT,TTT>& i_distFromThis);
	template<typename TT>
	friend inline distributed_reference_wrapper<TT> as_distributed_reference(TT* i_ptr);
	template<typename TT>
	friend inline distributed_reference_wrapper<TT> as_distributed_reference(TT* i_ptr,const IReferenceWrapperDeleter* i_refDeleter);
	template<typename TTT>
	friend inline detail::shared_reference_wrapper_impl<TTT,false> detail::__make_shared_reference(TTT*,const tagged_pointer<distributed_reference_counter>&,const IReferenceWrapperDeleter*);

public:
	struct __distributed_type_tag;
	typedef tagged_pointer<distributed_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	distribute_from_this() = default;
	distribute_from_this(IReferenceWrapperDeleter* i_deleter);
	distribute_from_this(const distribute_from_this&) = default;
	distribute_from_this(distribute_from_this&&) = delete;

	distribute_from_this& operator=(const distribute_from_this&) = delete;
	distribute_from_this& operator=(distribute_from_this&&) = delete;

protected:
	inline ddk::distributed_reference_wrapper<TT> ref_from_this();
	inline ddk::distributed_reference_wrapper<const TT> ref_from_this() const;

private:
	inline tagged_reference_counter get_reference_counter() const;
	inline const IReferenceWrapperDeleter* get_deleter() const;
	inline void set_deleter(const IReferenceWrapperDeleter* i_deleter) const;

	mutable distributed_reference_counter m_refCounter;
	mutable const IReferenceWrapperDeleter* m_deleter = nullptr;
};

}

#include "ddk_distribute_from_this.inl"