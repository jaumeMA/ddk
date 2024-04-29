//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_yielder_context.h"
#include "ddk_variant.h"
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct typed_yielder_context : yielder_context
{
public:
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rreference;
	typedef typename embedded_type<T>::raw_type value_type;
	typedef variant<T,async_exception> result_type;

	typed_yielder_context();
    template<typename TT>
	inline typed_yielder_context(TT&& i_value);
	inline typed_yielder_context(const typed_yielder_context& other);
	inline typed_yielder_context(typed_yielder_context&& other);

	inline typed_yielder_context& operator=(const typed_yielder_context&) = delete;
	inline typed_yielder_context& operator=(typed_yielder_context&&) = delete;
	TEMPLATE(typename Arg)
	REQUIRES(IS_CONSTRUCTIBLE(result_type,Arg))
	inline void insert_value(Arg&& i_value);
	template<typename Sink>
	inline void get_value(Sink&& i_sink) const;
	template<typename Sink>
	inline void get_value(Sink&& i_sink);
	template<typename Sink>
	inline void extract_value(Sink&& i_sink) &&;

private:
	mutable result_type m_value;
};

template<>
struct typed_yielder_context<void> : yielder_context
{
public:

	typed_yielder_context() = default;
	inline ~typed_yielder_context() = default;
};

}
}

#include "ddk_typed_yielder_context.inl"
