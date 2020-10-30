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

struct no_op_action
{
};

struct erase_action
{
};
SCOPED_ENUM_DECL(EraseActionError,
                 NonExistingValue,
                 ErasingFromConstantIterable);
typedef ddk::result<void,EraseActionError> erase_result;

struct add_action
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

struct stop_action
{
};

struct shift_action
{
public:
	shift_action(int i_shift);

    int shifted() const;
	int shifting() const;
	int incremental_shift() const;
	shift_action operator()(int i_shift) const;
	bool apply(const shift_action& i_appliedAction);

private:
    int m_targetShift;
	int m_currShift;
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

typedef variant<stop_action,erase_action,add_action,go_forward_action> input_action;
typedef variant<stop_action,erase_action,add_action,go_forward_action> output_action;
typedef variant<stop_action,erase_action,add_action,go_forward_action> forward_action;
typedef variant<stop_action,erase_action,add_action,go_forward_action,go_backward_action> bidirectional_action;
typedef variant<stop_action,erase_action,add_action,go_forward_action,go_backward_action,shift_action> random_access_action;
typedef variant<no_op_action,stop_action,erase_action,add_action,go_forward_action,go_backward_action,shift_action> any_action;

typedef error<ActionError,EraseActionError,AddActionError,ShiftActionError> action_error;
typedef result<any_action,action_error> action_result;

bool operator==(const forward_action& i_lhs, const forward_action& i_rhs);
bool operator==(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs);
bool operator==(const random_access_action& i_lhs, const random_access_action& i_rhs);
bool operator!=(const forward_action& i_lhs, const forward_action& i_rhs);
bool operator!=(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs);
bool operator!=(const random_access_action& i_lhs, const random_access_action& i_rhs);

const extern iter::no_op_action no_op;
const extern iter::stop_action stop_iteration;
const extern iter::erase_action erase_value;
const extern iter::add_action add_value;
const extern iter::go_forward_action go_next_place;
const extern iter::go_backward_action go_prev_place;
const extern iter::shift_action go_to_place;
const extern iter::shift_action go_no_place;

}
}
