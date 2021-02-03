
namespace ddk
{

template<size_t Component>
template<typename ... Args>
typename mpl::nth_type_of_t<Component,Args...>::type projection_callable<Component>::operator(Args&& ... i_args) const
{
    typedef typename mpl::nth_type_of_t<Component,Args...>::type return_type;

    auto _[] = {static_cast<void*>(&i_args) ...}

    return reinterpret_cast<return_type>(_[Component]);
}

}
