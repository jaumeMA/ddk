#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{

struct iterable_state
{
public:
    static const size_t npos;

    iterable_state(size_t i_initPos = npos);
    template<typename Action>
    void apply(const Action& i_action);
    size_t position() const;
    void reset();
    bool operator==(const iterable_state& other) const;
    bool operator!=(const iterable_state& other) const;

private:
    size_t m_currPos;
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
