#include "ddk_sync_executor.h"

namespace ddk
{

template<typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(const std::function<Return(Types...)>& i_function, Args&& ... i_args)
{
	async_shared_ref<Return> asyncExecutor = make_async_executor(i_function,std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(const T* i_object, Return(baseT::*i_funcPtr)(Types...), Args&& ... i_args)
{
	async_shared_ref<Return> asyncExecutor = make_async_executor(std::function<Return(Types...)>(std::bind(i_funcPtr,i_object)),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(T* i_object, Return(baseT::*i_funcPtr)(Types...), Args&& ... i_args)
{
	async_shared_ref<Return> asyncExecutor = make_async_executor(std::function<Return(Types...)>(std::bind(i_funcPtr,i_object)),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(Return(*i_funcPtr)(Types...), Args&& ... i_args)
{
	async_shared_ref<Return> asyncExecutor = make_async_executor(std::function<Return(Types...)>(i_funcPtr),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename Functor, typename ... Args>
async_shared_ref<typename mpl::aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type> async(const Functor& i_functor, Args&& ... i_args)
{
	typedef typename mpl::aqcuire_callable_return_type<Functor>::return_type return_type;
	typedef typename mpl::aqcuire_callable_return_type<Functor>::func_type func_type;

	async_shared_ref<return_type> asyncExecutor = async(func_type(i_functor),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename ... Types, typename ... Args>
async_shared_ref<void> async(const std::function<void(Types...)>& i_function, Args&& ... i_args)
{
	async_shared_ref<void> asyncExecutor = make_async_executor(std::function<detail::void_t(Types...)>([i_function](Types&& ... args) -> detail::void_t { i_function(std::forward<Types>(args) ...); return _void; }),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename T, typename baseT, typename ... Types, typename ... Args>
async_shared_ref<void> async(const T* i_object, void(baseT::*i_funcPtr)(Types...)const, Args&& ... i_args)
{
	async_shared_ref<void> asyncExecutor = make_async_executor(std::function<detail::void_t(Types...)>([i_object,i_funcPtr](Types&& ... args) -> detail::void_t { (i_object->*i_funcPtr)(std::forward<Types>(args) ...); return _void; }),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename T, typename baseT, typename ... Types, typename ... Args>
async_shared_ref<void> async(T* i_object, void(baseT::*i_funcPtr)(Types...), Args&& ... i_args)
{
	async_shared_ref<void> asyncExecutor = make_async_executor(std::function<detail::void_t(Types...)>([i_object,i_funcPtr](Types&& ... args) -> detail::void_t { (i_object->*i_funcPtr)(std::forward<Types>(args) ...); return _void; }),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}
template<typename ... Types, typename ... Args>
async_shared_ref<void> async(void(*i_funcPtr)(Types...), Args&& ... i_args)
{
	async_shared_ref<void> asyncExecutor = make_async_executor(std::function<detail::void_t(Types...)>([i_funcPtr](Types&& ... args) -> detail::void_t { (*i_funcPtr)(std::forward<Types>(args) ...); return _void; }),std::forward<Args>(i_args) ...);

	return asyncExecutor;
}

}