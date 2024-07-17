
namespace ddk
{

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable),IS_RETURN_TYPE_CONVERTIBLE_TO(Callable,bool))
void cond_var::wait_until(mutex& i_mutex,Callable&& i_predicate)
{
	while (eval(std::forward<Callable>(i_predicate)))
	{
		pthread_cond_wait(&m_condVar,i_mutex.get_impl());
	}
}
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable),IS_RETURN_TYPE_CONVERTIBLE_TO(Callable,bool))
void cond_var::wait_until(mutex& i_mutex,Callable&& i_predicate,const std::chrono::nanoseconds& i_time)
{
	while (eval(std::forward<Callable>(i_predicate)))
	{
		const std::chrono::seconds secs(std::chrono::duration_cast<std::chrono::seconds>(i_time));
		const struct timespec time_to_wait = { time(nullptr) + static_cast<time_t>(secs.count()), static_cast<long>((i_time - std::chrono::duration_cast<std::chrono::nanoseconds>(secs)).count()) };

		pthread_cond_timedwait(&m_condVar,i_mutex.get_impl(),&time_to_wait);
	}
}

}