#pragma once

#include "ewas_thread.h"
#include <vector>

namespace ewas
{

class thread_sheaf
{
	typedef std::vector<thread> thread_container;
	friend class thread_pool;

public:
	typedef thread_container::iterator iterator;
	typedef thread_container::const_iterator const_iterator;

	thread_sheaf(const thread_sheaf&) = delete;
	thread_sheaf(thread_sheaf&& other);
	void clear();
	thread extract();
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	size_t size() const;
	bool empty();

private:
	thread_sheaf() = default;

	thread_container m_threadCtr;
};


}