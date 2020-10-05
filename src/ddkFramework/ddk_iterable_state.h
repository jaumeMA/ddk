#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{

struct iterable_state
{
public:
    static const size_t npos = -1;

    iterable_state() = default;
    template<typename Action>
    void apply(const Action& i_action);
    size_t position() const;
    bool operator==(const iterable_state& other) const;
    bool operator!=(const iterable_state& other) const;

private:
    size_t m_currPos = npos;
};

namespace detail
{

template<typename>
struct state_visitor;

template<>
struct state_visitor<input_action> : public static_visitor<void>
{
public:
    state_visitor(size_t& i_currPos)
    : m_currPos(i_currPos)
    {}
    ddk::detail::void_t visit(const stop_action&)
    {
        m_currPos = iterable_state::npos;

        return _void;
    }
    ddk::detail::void_t visit(const erase_action&)
    {
        return _void;
    }
    ddk::detail::void_t visit(const add_action&)
    {
        return _void;
    }
    ddk::detail::void_t visit(const go_forward_action&)
    {
        ++m_currPos;

        return _void;
    }

    size_t get_pos() const
    {
        return m_currPos;
    }

protected:
    size_t& m_currPos;
};
template<>
struct state_visitor<bidirectional_action> : state_visitor<input_action>
{
public:
    using state_visitor<input_action>::state_visitor;
    using state_visitor<input_action>::visit;

    ddk::detail::void_t visit(const go_backward_action&)
    {
        m_currPos--;

        return _void;
    }
};

template<>
struct state_visitor<random_access_action> : public state_visitor<bidirectional_action>
{
public:
    using state_visitor<bidirectional_action>::state_visitor;
    using state_visitor<bidirectional_action>::visit;

    ddk::detail::void_t visit(const shift_action& i_action)
    {
        m_currPos += i_action.shifted();

        return _void;
    }
};

}
}
}

#include "ddk_iterable_state.inl"
