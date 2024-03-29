
#include "ddk_async.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename Sink>
linearized_index_access_dumping<Sink>::linearized_index_access_dumping(Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
template<typename Value>
void linearized_index_access_dumping<Sink>::apply(Value&& i_value) const
{
	m_sink.at(m_currIndex++) = std::forward<Value>(i_value);
}

template<typename Sink>
dynamic_access_dumping<Sink>::dynamic_access_dumping(Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
template<typename Value>
void dynamic_access_dumping<Sink>::apply(Value&& i_value) const
{
	m_sink.push_back(std::forward<Value>(i_value));
}

template<typename Sink>
type_access_dumping<Sink>::type_access_dumping(Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
template<typename ... T>
void type_access_dumping<Sink>::apply(const variant<T...>& i_value) const
{
	set(typename mpl::make_sequence<0,mpl::get_num_types<T...>()>::type{},i_value);
}
template<typename Sink>
template<size_t ... Indexs,typename ... T>
void type_access_dumping<Sink>::set(const mpl::sequence<Indexs...>&,const variant<T...>& i_value)
{
	typedef void(*funcType)(Sink&,const variant<T...>&);
	typedef type_access_dumping<Sink> type_access_dumping_t;

	static const funcType funcTable[] = { &type_access_dumping_t::_set<Indexs> ... };

	(*funcTable[m_currIndex])(m_sink,i_value);
}
template<typename Sink>
template<size_t Index,typename ... T>
void type_access_dumping<Sink>::_set(Sink& i_sink,const variant<T...>& i_value)
{
	i_sink.template set<Index>(i_value.template get<Index>());
}

template<typename Sink,typename Iterable>
inline future<iterable_result> iterable_transformation_dump(Sink& i_sink,Iterable&& i_transformedIterable)
{
	return ddk::async([&]()
	{
		typedef typename Iterable::const_reference const_reference;

		iterable_dumper_type<Sink,typename Iterable::value_type> _dumper(i_sink);

		return static_cast<iterable_result>([dumper = std::move(_dumper)](const_reference i_value)
		{
			dumper.apply(i_value);
		} <<= std::forward<Iterable>(i_transformedIterable));
	});
}

}
}
}
