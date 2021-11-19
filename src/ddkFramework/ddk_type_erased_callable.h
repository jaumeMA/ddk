#pragma once

#include "ddk_function_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Return>
struct callable_interface
{
public:
	typedef Return return_type;

	virtual ~callable_interface() = default;

	virtual return_type operator()() const = 0;
	virtual return_type operator()() = 0;
};
	
template<typename Callable>
struct callable_impl : public callable_interface<typename mpl::aqcuire_callable_return_type<Callable>::type>
{
public:
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type return_type;

	callable_impl(const Callable& i_callable);

	return_type operator()() const override;
	return_type operator()() override;

private:
	Callable m_callable;
};

}

template<typename Return>
struct type_erased_callable
{
public:
	type_erased_callable() = default;
	template<typename Callable>
	type_erased_callable(Callable& i_callable);

	Return operator()() const;
	Return operator()();

private:
	detail::callable_interface<Return>* m_callable;
};

template<typename Callable>
inline auto make_callable(Callable& i_callable);

}

#include "ddk_type_erased_callable.inl"