#pragma once

#include "ewas_none.h"
#include "variant.h"
#include "ewas_formatter.h"

namespace ewas
{

template<typename T>
class optional : protected variant2<detail::none_t,T>
{
	typedef value_wrapper<T> optional_type;
	typedef variant2<detail::none_t,T> base_t;
	using variant2<detail::none_t, T>::getValue;

public:
	typedef typename value_wrapper<T>::raw_type raw_type;
	typedef typename value_wrapper<T>::pointer_type pointer_type;
	typedef typename value_wrapper<T>::const_pointer_type const_pointer_type;
	typedef typename value_wrapper<T>::lreference lreference;
	typedef typename value_wrapper<T>::const_lreference const_lreference;
	typedef typename value_wrapper<T>::rreference rreference;
	typedef typename value_wrapper<T>::const_rreference const_rreference;
	using any_value::clear;

	optional()
	: base_t(none)
	{
	}
	optional(const detail::none_t&)
	: base_t(none)
	{
	}
	template<typename TT>
	optional(TT&& i_arg, typename std::enable_if<std::is_constructible<T,TT>::value>::type* = NULL)
	: base_t(std::forward<TT>(i_arg))
	{
	}
	optional(const optional<T>& other)
	: base_t(other)
	{
	}
	optional(optional<T>&& other)
	: base_t(std::move(other))
	{
	}
	operator bool() const
	{
		return this->template isOfType<detail::none_t>() == false;
	}
	const_lreference operator*() const
	{
		return this->template getValue<T>();
	}
	lreference operator*()
	{
		return this->template getValue<T>();
	}
	pointer_type operator->()
	{
		return &(this->template getValue<T>());
	}
	const_pointer_type operator->() const
	{
		return &(this->template getValue<T>());
	}
	const_lreference getValue() const
	{
		return this->template getValue<T>();
	}
	lreference getValue()
	{
		return this->template getValue<T>();
	}
	bool empty() const
	{
		return this->template isOfType<detail::none_t>();
	}
	optional<T>& operator=(const optional<T>& other)
	{
		variant2<detail::none_t,T>::operator=(other);

		return *this;
	}
	optional<T>& operator=(optional<T>&& other)
	{
		variant2<detail::none_t,T>::operator=(std::move(other));

		return *this;
	}
	template<typename TT>
	optional<T>& operator=(TT&& i_arg)
	{
		variant2<detail::none_t,T>::operator=(std::forward<TT>(i_arg));

		return *this;
	}
	bool operator==(const optional<T>& other) const
	{
		const bool lhsType = this->template isOfType<detail::none_t>();
		const bool rhsType = other.template isOfType<detail::none_t>();

		if(lhsType == false && rhsType == false)
		{
			return this->template getValue<T>() == other.template getValue<T>();
		}
		else
		{
			return lhsType == rhsType;
		}
	}
	bool operator!=(const optional<T>& other) const
	{
		const bool lhsType = this->template isOfType<detail::none_t>();
		const bool rhsType = other.template isOfType<detail::none_t>();

		if(lhsType == false && rhsType == false)
		{
			return this->template getValue<T>() != other.template getValue<T>();
		}
		else
		{
			return lhsType != rhsType;
		}
	}
};

}

namespace ewas
{

template<typename T>
struct formatter<optional<T>>
{
static optional<T> format(const std::string& i_str)
{
	return optional<T>(formatter<T>::format(i_str));
}
};

}