//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_function.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"

namespace ddk
{

struct continuation_token
{
public:
	static const size_t ntoken = -1;

	continuation_token(size_t i_token = ntoken);
	continuation_token(const continuation_token&) = delete;
	continuation_token(continuation_token&& other);

	continuation_token& operator=(const continuation_token&) = delete;
	continuation_token& operator=(continuation_token&& other);
	bool operator==(const continuation_token& other) const;
	bool operator==(const size_t& other) const;
	operator bool() const;

private:
	size_t m_id;
};

}