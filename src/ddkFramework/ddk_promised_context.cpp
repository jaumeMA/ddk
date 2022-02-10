#include "ddk_promised_context.h"

namespace ddk
{

void promised_context::start(const ddk::function<void()>& i_callable)
{
	ddk::eval(i_callable);
}
void promised_context::notify_recipients()
{
	m_recipients.notify();
}
void promised_context::clear_recipients()
{
	m_recipients.clear();
}
ddk::continuation_token promised_context::enqueue(const ddk::function<void()>& i_callable,unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
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