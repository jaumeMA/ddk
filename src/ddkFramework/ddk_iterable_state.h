#pragma once

#include "ddk_iterable_action.h"
#include "ddk_shared_reference_wrapper.h"

namespace ddk
{
namespace iter
{
namespace detail
{

//action component
class action_state
{
public:
    action_state() = default;
	~action_state();

    void set(action_result i_result);
    action_result get() const;
    action_result extract();
    void reset();

private:
    action_result m_actionResult = success;
};

typedef shared_reference_wrapper<action_state> action_state_shared_ref;

}

struct iterable_state
{
public:
    static const size_t npos;

    iterable_state(size_t i_initPos = npos);
    iterable_state(const iterable_state&) = default;

    template<typename Action>
    void apply(const Action& i_action);
    size_t position() const;
    void reset();
    bool operator==(const iterable_state& other) const;
    bool operator!=(const iterable_state& other) const;
    void forward_result(action_result i_result);
    action_result forward_result();
    template<typename Result>
    Result forward_result_as();

private:
    size_t m_currPos;
    detail::action_state_shared_ref m_actionState;
};

namespace detail
{

template<typename>
struct state_visitor;

template<>
struct state_visitor<input_action> : public static_visitor<size_t>
{
public:
    state_visitor(size_t i_currPos)
    : m_currPos(i_currPos)
    {}
	size_t visit(const filter_action&)
	{
		return m_currPos;
	}
	size_t visit(const stop_action&)
    {
        return m_currPos;
    }
    size_t visit(const erase_action&)
    {
        return m_currPos;
    }
    size_t visit(const add_action&)
    {
        return m_currPos;
    }
    size_t visit(const go_forward_action&)
    {
        return m_currPos + 1;
    }

protected:
    size_t m_currPos;
};
template<>
struct state_visitor<bidirectional_action> : state_visitor<input_action>
{
public:
    using state_visitor<input_action>::state_visitor;
    using state_visitor<input_action>::visit;

    size_t visit(const go_backward_action&)
    {
        return m_currPos - 1;
    }
};

template<>
struct state_visitor<random_access_action> : public state_visitor<bidirectional_action>
{
public:
    using state_visitor<bidirectional_action>::state_visitor;
    using state_visitor<bidirectional_action>::visit;

    size_t visit(const shift_action& i_action)
    {
        return m_currPos + i_action.shifted();
    }
};

}
}
}

#include "ddk_iterable_state.inl"
