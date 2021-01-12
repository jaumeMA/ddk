
namespace ddk
{

template<typename ... Args>
inline values_tuple<Args...> make_values_tuple(Args&& ... i_args)
{
    return values_tuple<Args...>(std::forward<Args>(i_args) ...);

}

}
