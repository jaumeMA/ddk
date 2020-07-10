#include "ewas_fiber_yielder.h"

namespace ewas
{
namespace detail
{

fiber_yielder::fiber_yielder(fiber_yielder_interface& i_executor)
: m_executor(i_executor)
{
}
void fiber_yielder::yield(yielder_context* i_context)
{
	m_executor.yield(i_context);
}
void fiber_yielder::suspend(yielder_context* i_context)
{
	m_executor.suspend(i_context);
}

}
}