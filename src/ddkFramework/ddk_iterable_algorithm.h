#pragma once

#include "ddk_iterable.h"
#include "ddk_iterable_value.h"
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_iterable_utils.h"
#include "ddk_tuple.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename Transform, typename ... Iterables>
class iterable_pack
{
public:
	explicit iterable_pack(const Transform& i_transform, const Iterables& ... i_iterables);
	explicit iterable_pack(Transform&& i_transform,const Iterables& ... i_iterables);
	explicit iterable_pack(const Iterables& ... i_iterables);

	template<size_t Index>
	inline const typename mpl::nth_type_of<Index,Iterables...>::type& get() const;
	Transform get_transform() const;

private:
	Transform m_transform;
	tuple<const Iterables...> m_iterables;
};

#define UNARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	template<typename Iterable> \
	class _NAME##_iterable_transform \
	{ \
	public: \
		_NAME##_iterable_transform() = default; \
		auto operator()(typename Iterable::const_reference i_value) const \
		{ \
			return (_OP i_value); \
		} \
	};

#define BINARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	template<typename ... Iterables> \
	class _NAME##_iterable_transform \
	{ \
	public: \
		_NAME##_iterable_transform() = default; \
		auto operator()(typename Iterables::const_reference ... i_values) const \
		{ \
			return (i_values _OP ...); \
		} \
	};

UNARY_ITERABLE_TRANSFORM(neg,-)

BINARY_ITERABLE_TRANSFORM(sum,+)
BINARY_ITERABLE_TRANSFORM(subs,+)
BINARY_ITERABLE_TRANSFORM(prod,+)

}

TEMPLATE(typename Transform, typename ... Iterables)
REQUIRES(IS_CALLABLE(Transform))
inline detail::iterable_pack<Transform,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_transform(Transform&& i_transform, const Iterables& ... i_iterables);

template<typename Iterable>
inline detail::iterable_pack<detail::neg_iterable_transform<Iterable>,decltype(deduce_iterable(std::declval<const Iterable&>()))> iterable_neg(const Iterable& i_iterable);

template<typename ... Iterables>
inline detail::iterable_pack<detail::sum_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_sum(const Iterables& ... i_iterables);

template<typename ... Iterables>
inline detail::iterable_pack<detail::subs_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_subs(const Iterables& ... i_iterables);

template<typename ... Iterables>
inline detail::iterable_pack<detail::prod_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_prod(const Iterables& ... i_iterables);

}
}

template<typename Sink,typename Transform,typename Iterable>
inline ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_lhs,const ddk::trans::detail::iterable_pack<Transform,Iterable>& i_rhs);

template<typename Sink, typename Transform,typename ... Iterables>
inline ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_lhs,const ddk::trans::detail::iterable_pack<Transform,Iterables...>& i_rhs);


#include "ddk_iterable_algorithm.inl"
