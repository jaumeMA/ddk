#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

yielder_context::yielder_context(fiber_id i_id, State i_state)
: m_state(std::make_pair(i_id,i_state))
{
}

}
}