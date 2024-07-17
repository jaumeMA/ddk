//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_awaitable.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename Return, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function);

}

#include "ddk_await.inl"
