#include "ddk_fixed_size_allocator.h"
#include "ddk_macros.h"
#include "ddk_lock_guard.h"

namespace ddk
{

fixed_size_allocator::fixed_size_allocator(size_t i_unitSize,size_t i_numUnits)
: fixed_size_allocator(std::vector<char>(i_numUnits* i_unitSize,0),i_unitSize,i_numUnits)
{
}
fixed_size_allocator::fixed_size_allocator(std::vector<char> i_vector,size_t i_unitSize, size_t i_numUnits)
: slab_allocator(i_vector.data(),i_unitSize,i_numUnits)
, m_pool(std::move(i_vector))
{
}

}
