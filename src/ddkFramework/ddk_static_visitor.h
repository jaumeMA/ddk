//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_void.h"
#include "ddk_crash.h"

namespace ddk
{

template<typename>
class static_visitor;

template<typename Return>
class static_visitor
{
public:
	struct callable_tag;
	typedef Return return_type;

	template<typename T>
	inline Return operator()(const T&) const;
};

}

#include "ddk_static_visitor.inl"