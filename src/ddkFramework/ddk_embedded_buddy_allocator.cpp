#include "ddk_embedded_buddy_allocator.h"
#include <cstdlib>

namespace ddk
{

namespace detail
{

const int k_embedBuddyMark = std::rand();

char* set_embed_buddy_order(char* i_ptr,unsigned char i_order)
{
	if (int* ptr = reinterpret_cast<int*>(i_ptr))
	{
		*ptr = (k_embedBuddyMark << 8) | i_order;

		return reinterpret_cast<char*>(ptr + 1);
	}
	else
	{
		return nullptr;
	}
}
unsigned char get_embed_buddy_order(void*& i_ptr)
{
	if (int* ptr = reinterpret_cast<int*>(i_ptr))
	{
		ptr--;

		if (((*(ptr) & ~(0x7)) >> 8) == k_embedBuddyMark)
		{
			i_ptr = ptr;

			unsigned char res = *(reinterpret_cast<char*>(i_ptr));

			return res;
		}
	}

	return -1;
}

}
}