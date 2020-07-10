#include "ewas_system_allocator.h"
#include <cstdlib>

namespace ewas
{

void* system_allocator::allocate(size_t numUnits, size_t unitSize) const
{
    void *res = NULL;

    if(numUnits >= 1)
    {
        res = malloc(numUnits * unitSize);
    }

    return res;
}
void system_allocator::deallocate(void *ptr) const
{
    if(ptr)
    {
        free(ptr);
    }

    return;
}
void* system_allocator::reallocate(void *ptr, size_t numUnits, size_t unitSize) const
{
    void *res = NULL;

    if(numUnits >= 1)
    {
        //num allocations remains the same
        res = realloc(ptr,numUnits * unitSize);
    }

    return res;
}

}