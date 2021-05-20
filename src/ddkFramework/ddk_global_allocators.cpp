#include "ddk_global_allocators.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace
{

std::map<size_t,fixed_size_allocator>& __get_global_allocator_map()
{
	static std::map<size_t,fixed_size_allocator> s_globalAllocatorMap;

	s_globalAllocatorMap.insert(std::make_pair(8,8));
	s_globalAllocatorMap.insert(std::make_pair(16,16));
	s_globalAllocatorMap.insert(std::make_pair(32,32));
	s_globalAllocatorMap.insert(std::make_pair(64,64));

	return s_globalAllocatorMap;
}

std::map<size_t,fixed_size_allocator>& _get_global_allocator_map()
{
	static std::map<size_t,fixed_size_allocator>& s_globalAllocMap = __get_global_allocator_map();

	return s_globalAllocMap;
}

}

bool __append_global_allocator_map_entries(const std::initializer_list<size_t>& i_entries)
{
	std::map<size_t,fixed_size_allocator>& s_globalAllocatorMap = _get_global_allocator_map();

	for(const auto& entry : i_entries)
	{
		s_globalAllocatorMap.insert(std::make_pair(entry,fixed_size_allocator(entry)));
	}

	return true;
}

const fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize)
{
	const std::map<size_t,fixed_size_allocator>& globalAllocMap = _get_global_allocator_map();

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