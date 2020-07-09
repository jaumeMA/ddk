#include "ewas_connection_base.h"
#include <cassert>
#include "ewas_macros.h"
#include "ewas_signal.h"

namespace ewas
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