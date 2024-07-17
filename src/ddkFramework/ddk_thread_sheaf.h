//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_thread.h"
#include <vector>
#include "ddk_optional.h"

namespace ddk
{

class thread_sheaf
{
	typedef std::vector<thread> thread_container;
	friend class thread_pool;

	friend thread_sheaf& operator<<(thread_sheaf& i_lhs,thread i_rhs);

public:
	typedef thread_container::iterator iterator;
	typedef thread_container::const_iterator const_iterator;
	typedef ddk::result<size_t,thread::start_error> start_result;
	typedef ddk::result<size_t,thread::stop_error> stop_result;

	thread_sheaf() = default;
	thread_sheaf(const thread_sheaf&) = delete;
	thread_sheaf(thread_sheaf&& other);
	thread_sheaf& operator=(const thread_sheaf&) = delete;
	thread_sheaf& operator=(thread_sheaf&& other);
	start_result start(const ddk::function<void()>& i_function);
	stop_result stop();
	start_result start(const ddk::function<void()>& i_function, size_t i_numThreads);
	bool joinable() const;
	size_t set_affinity(const cpu_set_t& i_set);
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	void clear();
	size_t size() const;
	bool empty();

private:
	thread_container m_threadCtr;
};


}