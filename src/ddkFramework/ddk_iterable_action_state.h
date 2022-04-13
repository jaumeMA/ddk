#pragma once

#include "ddk_iterable_action.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{

typedef error<ActionError,erase_error,add_error,shift_error,stop_error> action_error;

class action_result : public result<random_access_action,action_error>
{
public:
	using result<random_access_action,action_error>::result;
	~action_result();

	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
};

class action_state
{
public:
	action_state();

	template<typename T>
	void forward_result(T&& i_result);
	template<typename T>
	T&& forward_result();
	action_result get() const;
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const;
	bool operator==(result_success_t) const;
	bool operator!=(result_success_t) const;

private:
	action_result m_actionResult;
};

typedef lent_reference_wrapper<action_state> action_state_lent_ref;
typedef lent_reference_wrapper<const action_state> action_state_const_lent_ref;
typedef lent_pointer_wrapper<action_state> action_state_lent_ptr;
typedef lent_pointer_wrapper<const action_state> action_state_const_lent_ptr;

}

#include "ddk_iterable_action_state.inl"
