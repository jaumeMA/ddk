#include "ddk_connection.h"

namespace ddk
{

connection::connection()
: m_referredConnection(nullptr)
{}
connection::connection(detail::connection_base& i_conn)
: m_referredConnection(&i_conn)
{
}
connection::connection(connection&& other)
: m_referredConnection(nullptr)
{
	std::swap(m_referredConnection,other.m_referredConnection);
}
connection::~connection()
{
	if(m_referredConnection)
	{
		m_referredConnection->disconnect();
		m_referredConnection = nullptr;
	}
}
connection& connection::operator=(connection&& other)
{
	if(m_referredConnection)
	{
		m_referredConnection->disconnect();
		m_referredConnection = nullptr;
	}

	std::swap(m_referredConnection,other.m_referredConnection);

    return *this;

}
connection& connection::operator=(detail::connection_base& i_conn)
{
	if(m_referredConnection)
	{
		m_referredConnection->disconnect();
		m_referredConnection = nullptr;
	}

	m_referredConnection = &i_conn;

	return *this;
}
bool connection::is_enabled() const
{
	return (m_referredConnection) ? m_referredConnection->is_enabled() : false;
}
void connection::enable()
{
	if(m_referredConnection)
	{
		m_referredConnection->enable();
	}
}
void connection::disable()
{
	if(m_referredConnection)
	{
		m_referredConnection->disable();
	}
}
void connection::disconnect()
{
	if(m_referredConnection)
	{
		m_referredConnection->disconnect();
		m_referredConnection = nullptr;
	}
}
bool connection::is_connected() const
{
	return m_referredConnection != nullptr;
}

}