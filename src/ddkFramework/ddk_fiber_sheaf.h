#pragma once

#include "ewas_fiber.h"

namespace ewas
{

class fiber_pool;

class fiber_sheaf
{
	friend class fiber_pool;
	typedef std::vector<fiber> fiber_container;

public:
	typedef fiber_container::iterator iterator;
	typedef fiber_container::const_iterator const_iterator;

	fiber_sheaf(const fiber_sheaf&) = delete;
	fiber_sheaf(fiber_sheaf&& other);
	void clear();
	fiber extract();
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	bool empty() const;
	size_t size() const;

private:
	fiber_sheaf() = default;

	fiber_container m_fiberCtr;
};

}