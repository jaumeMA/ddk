#pragma once

#include <stddef.h>
#include "ddk_variant.h"
#include "ddk_any_value.h"
#include "ddk_scoped_enum.h"
#include "ddk_result.h"

namespace ddk
{
namespace iter
{

struct base_action
{
};

struct stop_action : base_action
{
	void operator()() const;
};

struct erase_action : base_action
{
};

SCOPED_ENUM_DECL(EraseActionError,
                 NonExistingValue,
                 ErasingFromConstantIterable);
typedef ddk::result<void,EraseActionError> erase_result;

struct add_action : base_action
{
public:
    add_action() = default;
    template<typename T, typename ... Args>
    add_action operator()(Args&& ... i_args)
    {
        add_action res(std::forward<Args>(i_args) ...);

        return res;
    }
    template<typename T>
    const T& get() const
    {
        return m_value.getValue<T>();
    }

private:
    template<typename T, typename ... Args>
    add_action(Args&& ... i_args)
    : m_value(T{std::forward<Args>(i_args) ...})
    {
    }

    any_value m_value;
};
SCOPED_ENUM_DECL(AddActionError,
                NonConvertibleType,
				AddingToConstantIterable,
				AddedItemFiltered);
typedef ddk::result<void,AddActionError> add_result;

struct shift_action : base_action
{
public:
	shift_action() = default;
	shift_action(const shift_action&) = default;
	shift_action(shift_action&& other);
	shift_action(int i_targetShift, int i_currShift = 0, bool i_stepByStep = false);

    int shifted() const;
	int shifting() const;
	shift_action operator()(int i_targetShift,int i_currShift = 0) const;
	bool apply(const shift_action& i_appliedAction);
	void set_step_by_step(bool i_cond);
	bool step_by_step() const;
	shift_action& operator=(const shift_action& other);
	shift_action& operator=(shift_action&& other);
	operator bool() const;

private:
    int m_targetShift = 0;
	int m_currShift = 0;
	bool m_stepByStep = false;
};
SCOPED_ENUM_DECL(ShiftActionError,
				 ShiftOutOfBounds,
				 ItemFiltered);
typedef result<void,ShiftActionError> shift_result;

struct go_forward_action : public shift_action
{
	go_forward_action();
	go_forward_action(const shift_action& other);
};

struct go_backward_action : public shift_action
{
	go_backward_action();
	go_backward_action(const shift_action& other);
};

SCOPED_ENUM_DECL(ActionError,
                 RemovalError,
                 AdditionError,
                 ShiftError);

typedef variant<erase_action,add_action,go_forward_action> input_action;
typedef variant<erase_action,add_action,go_forward_action> output_action;
typedef variant<erase_action,add_action,go_forward_action> forward_action;
typedef variant<erase_action,add_action,go_forward_action,go_backward_action> bidirectional_action;
typedef variant<erase_action,add_action,go_forward_action,go_backward_action,shift_action> random_access_action;

typedef variant<go_forward_action> const_input_action;
typedef variant<go_forward_action> const_output_action;
typedef variant<go_forward_action> const_forward_action;
typedef variant<go_forward_action,go_backward_action> const_bidirectional_action;
typedef variant<shift_action> const_random_access_action;

bool operator==(const base_action& i_lhs, const base_action& i_rhs);
bool operator!=(const base_action& i_lhs, const base_action& i_rhs);
bool operator==(const shift_action& i_lhs,const shift_action& i_rhs);
bool operator!=(const shift_action& i_lhs,const shift_action& i_rhs);

const extern iter::stop_action stop_iteration;
const extern iter::erase_action erase_value;
const extern iter::add_action add_value;
const extern iter::go_forward_action go_next_place;
const extern iter::go_backward_action go_prev_place;
const extern iter::shift_action go_to_place;
const extern iter::shift_action go_no_place;

}
}
