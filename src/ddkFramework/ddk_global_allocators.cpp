#include "ddk_global_allocators.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace
{

std::map<size_t,fixed_size_allocator>& __get_global_allocator_map()
{
	static std::map<size_t,fixed_size_allocator> s_globalAllocatorMap = {{8,fixed_size_allocator(8)},{16,fixed_size_allocator(16)},{32,fixed_size_allocator(32)}};

	return s_globalAllocatorMap;
}

}

bool __append_global_allocator_map_entries(const std::initializer_list<size_t>& i_entries)
{
	static std::map<size_t,fixed_size_allocator>& s_globalAllocatorMap = __get_global_allocator_map();

	for(const auto& entry : i_entries)
	{
		s_globalAllocatorMap.insert(std::make_pair(entry,fixed_size_allocator(entry)));
	}

	return true;
}

const fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize)
{
	const std::map<size_t,fixed_size_allocator>& globalAllocMap = __get_global_allocator_map();

	std::map<size_t,fixed_size_allocator>::const_iterator itAlloc = globalAllocMap.lower_bound(i_unitSize);
	if(itAlloc != globalAllocMap.end())
	{
		return &(itAlloc->second);
	}

	return nullptr;
}

const null_deleter g_nullDeleter = null_deleter();

void null_deleter::deallocate(const void* i_object) const
{
	//do notihng
}


}