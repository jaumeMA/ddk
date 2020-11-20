#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

yielder_context::yielder_context()
{
	m_stop = std::make_pair(ddk::fiber_id(0),false);
}

}
}