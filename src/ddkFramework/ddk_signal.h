//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_signal_functor.h"
#include "ddk_connection.h"
#include "ddk_macros.h"
#include "ddk_linked_list.h"
#include "ddk_signal_connector.h"
#include <tuple>

namespace ddk
{

template<typename ... Types>
class signal<void(Types...)> : protected detail::signal_connector
{
  typedef signal_functor<void(Types...)> signal_functor_t;

public:
	signal() = default;
    signal(const signal<void(Types...)>&) = delete;
    signal<void(Types...)>& operator=(const signal<void(Types...)>&) = delete;

	NO_DISCARD_RETURN connection connect(const ddk::function<void(Types...)>& i_function) const;
	void disconnect();
	template<typename ... Args>
	void execute(Args&& ... i_args) const;

private:
	bool disconnect(const detail::connection_base& i_base) const override final;

	mutable linked_list<signal_functor_t> m_callers;
};

}

#include "ddk_signal.inl"