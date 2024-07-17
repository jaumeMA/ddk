//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
struct iterable_visitor
{
	typedef iterable_adaptor<Iterable> adaptor_t;

	friend inline adaptor_t& deduce_adaptor(iterable_visitor& i_iterableVisitor)
	{
		return i_iterableVisitor.m_adaptor;
	}

protected:
	template<typename ... Args>
	iterable_visitor(Args&& ... i_args);

	template<typename Action>
	inline void loop(const Action& i_action);
	template<typename Action>
	inline void loop(const Action& i_action) const;

private:
	template<typename TypePack, typename Action>
	constexpr inline auto _loop(const Action& i_action);
	template<typename TypePack,typename Action>
	constexpr inline auto _loop(const Action& i_action) const;


	adaptor_t m_adaptor;
};

}
}

#include "ddk_iterable_visitor.inl"