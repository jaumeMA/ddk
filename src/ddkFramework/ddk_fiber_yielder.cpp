#include "ddk_fiber_yielder.h"

namespace ddk
{
namespace detail
{

yielder::yielder(yielder_interface& i_executor)
: m_executor(i_executor)
{
}

}
}