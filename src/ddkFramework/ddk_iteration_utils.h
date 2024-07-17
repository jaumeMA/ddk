//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_task_executor.h"

template<typename Iterable,typename Sink>
ddk::future<ddk::iterable_result> operator<<=(ddk::task_executor& i_lhs, ddk::iteration<Iterable,Sink>& i_rhs);

#include "ddk_iteration_utils.inl"