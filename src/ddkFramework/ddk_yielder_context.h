//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_fiber_defs.h"

namespace ddk
{
namespace detail
{
struct yielder_context
{
	enum State
	{
		Running,
		Paused,
		Stopped
	};

	yielder_context(fiber_id i_id, State i_state = Running);
	virtual ~yielder_context() = default;

	void stop(fiber_id i_id);
	bool is_stopped(fiber_id i_id) const;
	void pause(fiber_id i_id);
	bool is_paused(fiber_id i_id) const;
	bool is_running(fiber_id i_id) const;
	fiber_id get_id() const;

private:
	std::pair<fiber_id,State> m_state;
};

}
}
