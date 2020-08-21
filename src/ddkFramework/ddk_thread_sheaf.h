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

public:
	typedef thread_container::iterator iterator;
	typedef thread_container::const_iterator const_iterator;

	thread_sheaf(const thread_sheaf&) = delete;
	thread_sheaf(thread_sheaf&& other);
	void start(const std::function<void()>& i_function);
	void stop();
	void insert(thread i_thread);
	optional<thread> extract();
	size_t size() const;
	bool empty();

private:
	thread_sheaf() = default;

	thread_container m_threadCtr;
};


}