#pragma once

#include "ddk_iterable_action.h"
#include "ddk_static_visitor.h"

namespace ddk
{
namespace iter
{

struct action_shift_visitor : public static_visitor<size_t>
{
public:
	action_shift_visitor(size_t i_currPos)
	: m_currPos(i_currPos)
	{}
	size_t visit(...) const
	{
		return m_currPos;
	}
	size_t visit(const shift_action& i_action) const;

protected:
	size_t m_currPos;
};

struct consolidate_visitor: public static_visitor<bool>
{
public:
	consolidate_visitor(const shift_action& i_appliedAction)
	: m_appliedAction(i_appliedAction)
	{
	}
	inline bool visit(...) const
	{
		return true;
	}
	bool visit(shift_action& i_action) const;

protected:
	shift_action m_appliedAction;
};

}
}