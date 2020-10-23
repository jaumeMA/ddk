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

struct filter_action
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

struct displacement_action
{
public:
	displacement_action(size_t i_currDisplacement = 0);

	size_t get_displacement() const;

private:
	size_t m_currDisplacement;
};

struct go_forward_action : displacement_action
{
	using displacement_action::displacement_action;
};

struct go_backward_action : displacement_action
{
	using displacement_action::displacement_action;
};

struct shift_action : displacement_action
{
public:
	using displacement_action::displacement_action;

    int shifted() const;
    shift_action operator()(int i_shift) const;

private:
    int m_shift = 0;
};
SCOPED_ENUM_DECL(ShiftActionError,
                 ShiftBeyondBounds);
typedef result<void,ShiftActionError> shift_result;

SCOPED_ENUM_DECL(ActionError,
                 RemovalError,
                 AdditionError,
                 ShiftError);

typedef error<ActionError,EraseActionError,AddActionError,ShiftActionError> action_error;
typedef result<void,action_error> action_result;

typedef variant<filter_action,stop_action,erase_action,add_action,go_forward_action> input_action;
typedef variant<filter_action,stop_action,erase_action,add_action,go_forward_action> output_action;
typedef variant<filter_action,stop_action,erase_action,add_action,go_forward_action> forward_action;
typedef variant<filter_action,stop_action,erase_action,add_action,go_forward_action,go_backward_action> bidirectional_action;
typedef variant<filter_action,stop_action,erase_action,add_action,go_forward_action,go_backward_action,shift_action> random_access_action;

bool operator==(const forward_action& i_lhs, const forward_action& i_rhs);
bool operator==(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs);
bool operator==(const random_access_action& i_lhs, const random_access_action& i_rhs);
bool operator!=(const forward_action& i_lhs, const forward_action& i_rhs);
bool operator!=(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs);
bool operator!=(const random_access_action& i_lhs, const random_access_action& i_rhs);

const extern iter::filter_action filter_iteration;
const extern iter::stop_action stop_iteration;
const extern iter::erase_action erase_value;
const extern iter::add_action add_value;
const extern iter::go_forward_action go_next_place;
const extern iter::go_backward_action go_prev_place;
const extern iter::shift_action go_shift_place;

}
}
