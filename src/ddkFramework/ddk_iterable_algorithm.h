#pragma once

#include "ddk_iterable_algorithm_impl.h"
#include "ddk_concepts.h"
#include "ddk_container_concepts.h"
#include "ddk_function_ops.h"

#define FUNC_ITERABLE_TRANSFORM(_NAME,_FUNC) \
	struct _NAME##_iterable_transform \
	{ \
	public: \
		_NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			using ::operator<<=; \
			\
			return transform(_FUNC) <<= deduce_iterable(i_iterable); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define UNARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform \
	{ \
	public: \
		_NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			using ::operator<<=; \
			\
			return transform([](auto&& i_value){ return _OP i_value; }) <<= deduce_iterable(i_iterable); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define BINARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform \
	{ \
	private: \
		struct impl \
		{ \
			template<typename ... Args> \
			auto operator()(Args&& ... i_values) const\
			{ \
				return (i_values _OP ...); \
			} \
		}; \
	\
	public: \
		constexpr _NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			using ::operator<<=; \
			\
			return transform(impl{}) <<= deduce_iterable(i_iterable); \
		} \
		template<typename Iterable, typename ... Iterables> \
		inline auto operator()(Iterable&& i_iterable, Iterables&& ... i_iterables) const \
		{ \
			using ::operator<<=; \
			\
			return transform(impl{}) <<= ddk::fusion(std::forward<Iterable>(i_iterable),std::forward<Iterables>(i_iterables)...); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

namespace ddk
{
namespace iter
{

struct transform_iterable_transform
{
public:
	constexpr transform_iterable_transform() = default;
	template<typename Functor>
	inline auto operator()(Functor&& i_transform) const;
};
const transform_iterable_transform transform;

}
}

TEMPLATE(typename Sink,typename Iterable)
REQUIRES(IS_CONTAINER(Sink))
inline auto operator<<=(Sink& i_lhs, Iterable&& i_rhs);

#include "ddk_iterable_algorithm.inl"

namespace ddk
{
namespace iter
{

UNARY_ITERABLE_TRANSFORM(inv,-)

BINARY_ITERABLE_TRANSFORM(sum,+)
BINARY_ITERABLE_TRANSFORM(subs,-)
BINARY_ITERABLE_TRANSFORM(prod,*)
BINARY_ITERABLE_TRANSFORM(div,/ )

FUNC_ITERABLE_TRANSFORM(sqrt,ddk::sqrt)
FUNC_ITERABLE_TRANSFORM(pow,ddk::pow)
FUNC_ITERABLE_TRANSFORM(sin,ddk::sin)
FUNC_ITERABLE_TRANSFORM(cos,ddk::cos)

}
}