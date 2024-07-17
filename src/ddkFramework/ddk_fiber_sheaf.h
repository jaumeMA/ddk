//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_fiber.h"
#include "ddk_optional.h"

namespace ddk
{

class fiber_pool;

class fiber_sheaf
{
	friend class fiber_pool;
	typedef std::vector<fiber> fiber_container;

public:
	typedef fiber_container::iterator iterator;
	typedef fiber_container::const_iterator const_iterator;
	typedef ddk::result<size_t,fiber::start_error> start_result;
	typedef ddk::result<size_t,fiber::stop_error> stop_result;

	fiber_sheaf() = default;
	fiber_sheaf(const fiber_sheaf&) = delete;
	fiber_sheaf(fiber_sheaf&& other);
	~fiber_sheaf();
	fiber_sheaf& operator=(const fiber_sheaf&) = delete;
	fiber_sheaf& operator=(fiber_sheaf&& other);
	start_result start(const ddk::function<void()>& i_function);
	stop_result stop();
	bool joinable() const;
	void clear();
	bool empty() const;
	size_t size() const;

private:
	fiber_container m_fiberCtr;
};

}