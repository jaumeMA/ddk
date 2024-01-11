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
};

template<typename>
struct sink_action_tag;

template<typename Sink>
struct sink_action_tag
{
	template<typename>
	friend struct sink_action_tag;

public:
	sink_action_tag(const Sink& i_sink);
	sink_action_tag(Sink&& i_sink);
	template<typename T>
	sink_action_tag(const sink_action_tag<function<void(T)>>& i_function);
	sink_action_tag(const sink_action_tag<Sink>& i_sink) = default;
	sink_action_tag(sink_action_tag<Sink>&& i_sink) = default;

	sink_action_tag& operator=(const sink_action_tag<Sink>& i_sink) = default;
	sink_action_tag& operator=(sink_action_tag<Sink>&& i_sink) = default;
	template<typename T>
	inline T&& operator()(T&& i_value) const;

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
	sink_action_tag(Sink&& i_sink);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,T))
	sink_action_tag(const sink_action_tag<Sink>& i_sink);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,T))
	sink_action_tag(sink_action_tag<Sink>&& i_sink);

	template<typename TT>
	inline TT&& operator()(TT&& i_value) const;

private:
	mutable sink_t m_sink;
};
const sink_action_tag k_agnosticIterableEmptySink = sink_action_tag{ [](auto&& i_value) {} };
typedef decltype(k_agnosticIterableEmptySink) k_agnostic_iterable_empty_sink;

template<typename T>
inline const sink_action_tag k_iterableEmptySink = sink_action_tag{ [](T) {} };
template<typename T>
using k_iterable_empty_sink = decltype(k_iterableEmptySink<T>);

template<typename T>
using agnostic_sink_action_tag = sink_action_tag<function<void(T)>>;

struct begin_action_tag
{
};
struct last_action_tag
{
};
struct forward_action_tag
{
};
struct backward_action_tag
{
};
struct displace_action_tag
{
public:
	typedef long long difference_type;

	displace_action_tag(difference_type i_displacement);

	difference_type displacement() const;

private:
	difference_type m_displacement;
};
template<typename T>
struct add_action_tag
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	add_action_tag(Args&& ... i_args);

	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	operator add_action_tag<TT>() const;
	const T& get() const;
	T extract() &&;

private:
	T m_payload;
};
template<typename T>
add_action_tag(const T&)->add_action_tag<T>;
template<typename T>
add_action_tag(T&&)->add_action_tag<T>;

struct remove_action_tag
{
};

template<typename ActionTag,typename ... Args>
inline ActionTag iterable_action_cs(const Args& ... i_args)
{
	return { i_args... };
}
template<>
inline void_action_tag iterable_action_cs(const void_action_tag&)
{
	return {};
}

}

#include "ddk_iterable_action_tag.inl"
#include "ddk_iterable_action_tags_template_helper.h"