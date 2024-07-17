#include "ddk_async_shared_state_base.h"

namespace ddk
{
namespace detail
{

private_async_state_base::private_async_state_base()
: m_asyncMutex(MutexType::Recursive)
{
}
typename private_async_state_base::cancel_result private_async_state_base::cancel()
{
	mutex_guard lg(m_asyncMutex);

	if (m_asyncExecutor)
	{
		return m_asyncExecutor->cancel();
	}
	else
	{
		return make_error<cancel_result>(async_interface_base::CancelErrorCode::CancelNoAsync);
	}
}
void private_async_state_base::attach(async_base_dist_ptr i_executor)
{
	mutex_guard lg(m_asyncMutex);

	m_asyncExecutor = i_executor;
}
bool private_async_state_base::detach()
{
	mutex_guard lg(m_asyncMutex);

	if (m_asyncExecutor)
	{
		m_asyncExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Predicate>
bool private_async_state_base::detach_if(Predicate&& i_predicate)
{
	mutex_guard lg(m_asyncMutex);

	if (static_cast<bool>(m_asyncExecutor) && ddk::eval(std::forward<Predicate>(i_predicate)))
	{
		m_asyncExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
async_base_dist_ptr private_async_state_base::get_async_execution() const
{
	mutex_guard lg(m_asyncMutex);

	return m_asyncExecutor;
}

}
}