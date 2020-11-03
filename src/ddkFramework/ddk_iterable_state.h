#pragma once

#include "ddk_iterable_action.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_fiber_local.h"

namespace ddk
{
namespace iter
{

//action component
class action_state
{
public:
    action_state();
	~action_state();

	void forward_result(action_result i_result);
	action_result get() const;
    action_result extract();
    void reset();
	bool operator==(result_success_t) const;
	bool operator!=(result_success_t) const;
	bool operator==(const action_error& i_error) const;
	bool operator!=(const action_error& i_error) const;

private:
    action_result m_actionResult;
};

struct iterable_state
{
public:
    static const size_t npos;

    iterable_state(size_t i_pos = 0);
    iterable_state(const iterable_state&) = default;

    template<typename Action>
    void apply(const Action& i_action);
    size_t position() const;
    void reset();
    bool operator==(const iterable_state& other) const;
    bool operator!=(const iterable_state& other) const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	static bool is_success();
	template<typename ErrorCode>
	static bool in_error(ErrorCode&&);
	operator bool() const;
	static bool flush_result();
	static void forward_result(action_result i_result);
	static action_result forward_result();
    template<typename Result>
    static Result forward_result_as();

private:
	static fiberlocal<action_result,iterable_state>& get_action_result();
		
	size_t m_currPos = 0;
};

}
}

#include "ddk_iterable_state.inl"
