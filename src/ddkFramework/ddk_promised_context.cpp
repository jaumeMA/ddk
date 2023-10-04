#include "ddk_promised_context.h"

namespace ddk
{

void promised_context::start(const ddk::function<void()>& i_callable)
{
	ddk::eval(i_callable);
}
void promised_context::notify_recipients()
{
	m_recipients.notify(false);
}
void promised_context::transfer_recipients(promised_context i_context)
{
	m_recipients.move(std::move(i_context.m_recipients));
}
void promised_context::clear_recipients()
{
	m_recipients.clear();
}
void promised_context::transfer(detail::execution_context_base&& other)
{
}
bool promised_context::dismiss(unsigned char i_depth,ddk::continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void promised_context::clear()
{
	m_recipients.clear();
}

}