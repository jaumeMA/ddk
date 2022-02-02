#include "ddk_thread_executor_capabilities.h"

namespace ddk
{
namespace detail
{

executor_capabilities<ddk::thread>::executor_capabilities(ddk::thread i_thread)
: m_context(std::move(i_thread))
{
}
bool executor_capabilities<ddk::thread>::set_affinity(const cpu_set_t& i_set)
{
	return m_context.set_affinity(i_set);
}

executor_capabilities<ddk::thread_sheaf>::executor_capabilities(ddk::thread_sheaf i_threadSheaf)
: m_context(std::move(i_threadSheaf))
{
}
bool executor_capabilities<ddk::thread_sheaf>::set_affinity(const cpu_set_t& i_set)
{
	return m_context.set_affinity(i_set);
}

}
}