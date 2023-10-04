
namespace ddk
{

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
ddk::continuation_token promised_context::enqueue(Callable&& i_callable,unsigned char i_depth)
{
	return m_recipients.accept(std::forward<Callable>(i_callable),i_depth);
}

}