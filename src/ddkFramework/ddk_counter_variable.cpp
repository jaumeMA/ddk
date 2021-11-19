#include "ddk_counter_variable.h"

namespace ddk
{

counter_variable::counter_variable(int i_initCounter)
: m_reference(i_initCounter)
{
}
void counter_variable::push() const
{
	m_mutex.lock();

	m_reference--;

	m_condVar.notify_all();

	m_mutex.unlock();
}
void counter_variable::pull() const
{
	m_mutex.lock();

	m_reference++;

	while(m_reference > 0)
	{
		m_condVar.wait(m_mutex);
	}

	m_mutex.unlock();
}
int counter_variable::get_value() const
{
	return m_reference;
}

}