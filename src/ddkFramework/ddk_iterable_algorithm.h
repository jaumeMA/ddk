//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
			return transform(_FUNC) <<= std::forward<Iterable>(i_iterable); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define UNARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform \
	{ \
	private: \
		template<typename T> \
		struct impl \
		{ \
			typedef T return_type; \
			template<typename Arg> \
			auto operator()(Arg&& i_value) const\
			{ \
				return (_OP std::forward<Args>(i_value)); \
			} \
		}; \
	public: \
		_NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			typedef decltype((_OP std::declval<typename ddk::detail::adaptor_traits<ddk::deduced_iterable<Iterable>>::const_reference>())) return_type; \
			\
			return transform(impl<return_type>{}) <<= std::forward<Iterable>(i_iterable); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define BINARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform \
	{ \
	private: \
		template<typename T> \
		struct impl \
		{ \
			typedef T return_type; \
			template<typename ... Args> \
			auto operator()(Args&& ... i_values) const\
			{ \
				return (std::forward<Args>(i_values) _OP ...); \
			} \
		}; \
	\
	public: \
		constexpr _NAME##_iterable_transform() = default; \
		template<typename ... Iterables> \
		inline auto operator()(Iterables&& ... i_iterables) const \
		{ \
			typedef decltype((std::declval<typename ddk::detail::adaptor_traits<ddk::deduced_iterable<Iterables>>::const_reference>() _OP ...)) return_type; \
			\
			return transform(impl<return_type>{}) <<= ddk::fusion(std::forward<Iterables>(i_iterables)...); \
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
	template<typename Return>
	struct as
	{
	public:
		template<typename Functor>
		struct transform
		{
		public:
			typedef Return return_type;

			transform(const Functor& i_functor);

			template<typename ... Args>
			constexpr inline auto operator()(Args&& ... i_args) const;

		private:
			const Functor m_functor;
		};
		template<typename Functor>
		transform(const Functor&) -> transform<Functor>;

		as() = default;

		template<typename Functor>
		auto operator()(Functor&& i_functor) const;
	};

	constexpr transform_iterable_transform() = default;
	template<typename Functor>
	inline auto operator()(Functor&& i_transform) const;
};
const transform_iterable_transform transform;
template<typename Return>
inline constexpr transform_iterable_transform::template as<Return> transform_as = transform_iterable_transform::template as<Return>();

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