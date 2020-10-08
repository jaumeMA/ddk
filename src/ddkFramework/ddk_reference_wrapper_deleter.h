#pragma once

#include <utility>

#ifdef _WIN32

#pragma warning ( push )
#pragma warning ( disable: 4793 )

#endif

#define EWAS_ADDRESSABLE_TYPE(_TYPE) \
	friend inline const void* allocator_address_reference_wrapper(const _TYPE* i_ptr) \
	{ \
		return i_ptr; \
	}

namespace ddk
{

class IReferenceWrapperDeleter
{
public:
	virtual ~IReferenceWrapperDeleter() = default;
	virtual void Deallocate(const void* i_object) const = 0;
};

//null deleter
class null_deleter : public IReferenceWrapperDeleter
{
public:
	void Deallocate(const void* i_object) const final
	{
		//do notihng
	}
};
static const ddk::null_deleter s_nullDeleter = null_deleter();

template<typename Allocator>
const IReferenceWrapperDeleter& get_reference_wrapper_deleter(const Allocator&)
{
    static_assert(sizeof(Allocator)==0, "You shall specialize this reference wrapper allocator for this type");

    return std::declval<IReferenceWrapperDeleter>();
}

inline const void* allocator_address_reference_wrapper(const void* i_ptr, ...)
{
	//default impl is the trivial one
	return i_ptr;
}

}

#ifdef _WIN32

#pragma warning ( pop )

#endif
