
namespace ddk
{

TEMPLATE(typename T,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args)...)
values_vector<T> make_values_vector(Args&& ... i_args)
{
	return {std::forward<Args>(i_args)...};
}

}