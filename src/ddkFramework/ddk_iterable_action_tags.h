#pragma once

#include "ddk_function_template_helper.h"
#include "ddk_template_helper.h"
#include "ddk_function_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

struct void_action_tag
{
	constexpr void_action_tag() = default;
};

template<typename>
struct sink_action_tag;

template<typename Sink>
struct sink_action_tag
{
	template<typename>
	friend struct sink_action_tag;

public:
	constexpr sink_action_tag(const Sink& i_sink);
	constexpr sink_action_tag(Sink&& i_sink);
	template<typename T>
	constexpr sink_action_tag(const sink_action_tag<function<void(T)>>& i_function);
	constexpr sink_action_tag(const sink_action_tag<Sink>& i_sink) = default;
	constexpr sink_action_tag(sink_action_tag<Sink>&& i_sink) = default;

	constexpr sink_action_tag& operator=(const sink_action_tag<Sink>& i_sink) = default;
	constexpr sink_action_tag& operator=(sink_action_tag<Sink>&& i_sink) = default;
	template<typename T>
	constexpr inline T&& operator()(T&& i_value) const;

private:
	mutable Sink m_sink;
};
template<typename Sink>
sink_action_tag(const Sink&) -> sink_action_tag<Sink>;
template<typename Sink>
sink_action_tag(Sink&&) -> sink_action_tag<Sink>;

template<typename T>
struct sink_action_tag<function<void(T)>>
{
	template<typename>
	friend struct sink_action_tag;
	typedef function<void(T)> sink_t;

public:
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,T))
	constexpr sink_action_tag(Sink&& i_sink);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,T))
	constexpr sink_action_tag(const sink_action_tag<Sink>& i_sink);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,T))
	constexpr sink_action_tag(sink_action_tag<Sink>&& i_sink);

	template<typename TT>
	constexpr inline TT&& operator()(TT&& i_value) const;

private:
	sink_t m_sink;
};
const sink_action_tag k_iterableEmptySink = sink_action_tag{ [](auto&& i_value) noexcept {} };
typedef decltype(k_iterableEmptySink) k_iterable_empty_sink;

template<typename T>
using agnostic_sink_action_tag = sink_action_tag<function<void(T)>>;

struct begin_action_tag
{
	constexpr begin_action_tag() = default;
};

struct end_action_tag
{
	constexpr end_action_tag() = default;
};

struct forward_action_tag
{
	constexpr forward_action_tag() = default;
};

struct backward_action_tag
{
	constexpr backward_action_tag() = default;
};

struct displace_action_tag
{
public:
	typedef long long difference_type;

	constexpr displace_action_tag(difference_type i_displacement);

	constexpr difference_type displacement() const;

private:
	difference_type m_displacement;
};

struct size_action_tag
{
	constexpr size_action_tag() = default;
};

template<typename T>
struct add_action_tag
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	constexpr add_action_tag(Args&& ... i_args);

	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	constexpr operator add_action_tag<TT>() const;
	constexpr const T& get() const;
	constexpr T extract() &&;

private:
	T m_payload;
};
template<typename T>
add_action_tag(const T&)->add_action_tag<T>;
template<typename T>
add_action_tag(T&&)->add_action_tag<T>;

struct remove_action_tag
{
	constexpr remove_action_tag() = default;
};

template<typename ActionTag,typename ... Args>
constexpr inline ActionTag iterable_action_cs(const Args& ... i_args)
{
	return { i_args... };
}
template<>
constexpr inline void_action_tag iterable_action_cs(const void_action_tag&)
{
	return {};
}

}

#include "ddk_iterable_action_tag.inl"