#include "ddk_connection_base.h"
#include <cassert>
#include "ddk_macros.h"
#include "ddk_signal.h"

namespace ddk
{
namespace detail
{

connection_base::connection_base(const signal_connector& i_connector)
: m_connector(i_connector)
, m_isEnabled(true)
{
}
void connection_base::enable()
{
	m_isEnabled = true;
}
void connection_base::disable()
{
	m_isEnabled = false;
}
void connection_base::disconnect()
{
	m_connector.disconnect(*this);
}
bool connection_base::is_enabled() const
{
	return m_isEnabled;
}
size_t connection_base::get_id() const
{
	return reinterpret_cast<size_t>(this);
}

}
}