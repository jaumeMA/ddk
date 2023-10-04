
namespace ddk
{
namespace detail
{

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
continuation_token async_executor_recipients::accept(Callable&& i_callable,unsigned char i_depth)
{
	if (is_admissible())
	{
		mutex_guard mg(m_mutex);

		const size_t token = std::rand();

		m_pendingCallables[i_depth].emplace_back(token,std::forward<Callable>(i_callable));

		return { token };
	}
	else
	{
		return continuation_token::ntoken;
	}
}

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
continuation_token execution_context_base::enqueue(Callable&& i_callable,unsigned char i_depth)
{
	return m_recipientsRef->accept(std::forward<Callable>(i_callable),i_depth);
}

}

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
void immediate_execution_context::start(Callable&& i_callable, bool i_useAndKeep)
{
	eval(std::forward<Callable>(i_callable));

	notify_recipients(i_useAndKeep);
}

}