//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_smart_pointer_concepts.h"

namespace ddk
{
namespace detail
{

struct private_async_state_base
{
public:
	typedef typename async_interface_base::cancel_result cancel_result;

	private_async_state_base();
	virtual ~private_async_state_base() = default;

	cancel_result cancel();
	void attach(async_base_dist_ptr i_executor);
	bool detach();
	template<typename Predicate>
	bool detach_if(Predicate&& i_predicate);
	async_base_dist_ptr get_async_execution() const;

private:
	mutable mutex m_asyncMutex;
	mutable async_base_dist_ptr m_asyncExecutor;
};

using private_async_state_base_shared_ref = shared_reference_wrapper<private_async_state_base>;
using private_async_state_base_const_shared_ref = shared_reference_wrapper<const private_async_state_base>;
using private_async_state_base_shared_ptr = shared_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_shared_ptr = shared_pointer_wrapper<const private_async_state_base>;

using private_async_state_base_weak_ptr = weak_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_weak_ptr = weak_pointer_wrapper<const private_async_state_base>;

using private_async_state_base_lent_ref = lent_reference_wrapper<private_async_state_base>;
using private_async_state_base_const_lent_ref = lent_reference_wrapper<const private_async_state_base>;
using private_async_state_base_lent_ptr = lent_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_lent_ptr = lent_pointer_wrapper<const private_async_state_base>;

}
}

#include "ddk_async_shared_state_base.inl"