#pragma once

#include "ddk_future.h"
#include "ddk_iterable.h"
#include "ddk_concepts.h"
#include "ddk_container_concepts.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename Sink>
struct linearized_index_access_dumping
{
public:
	linearized_index_access_dumping(Sink& i_sink);
	template<typename Value>
	inline void apply(Value&& i_value) const;

private:
	Sink& m_sink;
	mutable size_t m_currIndex = 0;
};

template<typename Sink>
struct dynamic_access_dumping
{
public:
	dynamic_access_dumping(Sink& i_sink);
	template<typename Value>
	inline void apply(Value&& i_value) const;

private:
	Sink& m_sink;
};

template<typename Sink>
struct type_access_dumping
{
public:
	type_access_dumping(Sink& i_sink);
	template<typename ... T>
	inline void apply(const variant<T...>& i_value) const;

private:
	template<size_t ... Indexs,typename ... T>
	void set(const mpl::sequence<Indexs...>&, const variant<T...>& i_value);
	template<size_t Index, typename ... T>
	static void _set(Sink& i_sink, const variant<T...>& i_value);

	Sink& m_sink;
	mutable size_t m_currIndex = 0;
};

TEMPLATE(typename Sink,typename Traits)
REQUIRES(ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_INDEX_ACCESS(Sink,typename Traits::value_type))
inline future<ddk::iter::action_result> iterable_transformation_dump(Sink&& i_sink, const ddk::detail::iterable<Traits>& i_transformedIterable)
{
	typedef typename Traits::iterable_value iterable_value;

	linearized_index_access_dumping<Sink> _dumper(i_sink);

	return (ddk::make_function([dumper = std::move(_dumper)](iterable_value i_value) { dumper.apply(*i_value); }) <<= i_transformedIterable).attach(ddk::this_thread);
}

TEMPLATE(typename Sink,typename Traits)
REQUIRES(ASSIGNABLE_DYNAMIC_CONTAINER_BY_INDEX_ACCESS(Sink,typename Traits::value_type))
inline future<ddk::iter::action_result> iterable_transformation_dump(Sink&& i_sink,const ddk::detail::iterable<Traits>& i_transformedIterable, void* = nullptr)
{
	typedef typename Traits::iterable_value iterable_value;

	dynamic_access_dumping<Sink> _dumper(i_sink);

	return (ddk::make_function([dumper = std::move(_dumper)](iterable_value i_value) { dumper.apply(*i_value); }) <<= i_transformedIterable).attach(ddk::this_thread);
}

TEMPLATE(typename Sink,typename Traits)
REQUIRES(ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_TYPE_ACCESS(Sink,typename Traits::value_type))
inline future<ddk::iter::action_result> iterable_transformation_dump(Sink&& i_sink,const ddk::detail::iterable<Traits>& i_transformedIterable,int* = nullptr)
{
	typedef typename Traits::iterable_value iterable_value;

	type_access_dumping<Sink> _dumper(i_sink);

	return (ddk::make_function([dumper = std::move(_dumper)](iterable_value i_value) { dumper.apply(*i_value); }) <<= i_transformedIterable).attach(ddk::this_thread);
}

}
}
}

#include "ddk_iterable_algorithm_impl.inl"