
namespace ddk
{

template<typename Callable>
template<typename CCallable>
dynamic_callable<Callable>::dynamic_callable(CCallable&& i_callable)
: m_callable(std::forward<CCallable>(i_callable))
{
}
template<typename Callable>
template<typename T>
void dynamic_callable<Callable>::operator()(T&& i_value)
{
	eval(m_callable,std::forward<T>(i_value));
}

}