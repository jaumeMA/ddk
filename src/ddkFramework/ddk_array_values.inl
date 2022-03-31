
namespace ddk
{

template<typename T,size_t Dim>
values_array<T,Dim>::values_array(const base_t& other)
: base_t(other)
{
}
template<typename T,size_t Dim>
values_array<T,Dim>::values_array(base_t&& other)
: base_t(std::move(other))
{
}
template<typename T,size_t Dim>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args)...)
auto values_array<T,Dim>::append(Args&& ... i_args) const
{
	typedef typename mpl::make_sequence<0,Dim>::type seq_type;

	return construct(true,seq_type{},std::forward<Args>(i_args)...);
}
template<typename T,size_t Dim>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args)...)
auto values_array<T,Dim>::prepend(Args&& ... i_args) const
{
	typedef typename mpl::make_sequence<0,Dim>::type seq_type;

	return construct(false,seq_type{},std::forward<Args>(i_args)...);
}
template<typename T,size_t Dim>
template<size_t ... Indexs,typename ... Args>
values_array<T,Dim + mpl::num_types<Args...>> values_array<T,Dim>::construct(bool i_order, const mpl::sequence<Indexs...>&,Args&& ... i_args) const
{
	typedef values_array<T,Dim + mpl::num_types<Args...>> ret_type;

	return (i_order) ? ret_type({this->operator[](Indexs)...,std::forward<Args>(i_args)...}) : ret_type({std::forward<Args>(i_args)...,this->operator[](Indexs)...});
}

TEMPLATE(typename T, typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args)...)
values_array<T,mpl::num_types<Args...>> make_values_array(Args&& ... i_args)
{
	return {std::forward<Args>(i_args)...};
}

}