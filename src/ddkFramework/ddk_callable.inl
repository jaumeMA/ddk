
namespace ddk
{

template<typename ... Callables>
template<typename ... CCallables>
callable<Callables...>::callable(CCallables&& ... i_callables)
: Callables(std::forward<CCallables>(i_callables))...
{
}

}