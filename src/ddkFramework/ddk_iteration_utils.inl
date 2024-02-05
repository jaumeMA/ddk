
template<typename Iterable,typename Sink>
ddk::future<ddk::iterable_result> operator<<=(ddk::task_executor& i_lhs, ddk::iteration<Iterable,Sink>& i_rhs)
{
	typedef void ret_type;
	typedef typename ddk::mpl::which_type<ddk::mpl::is_same_type<ret_type,void>::value,ddk::detail::void_t,ret_type>::type sink_ret_type;
	ddk::optional<ddk::future<ddk::detail::void_t>> res;

	auto newIteration = std::move(i_rhs).transform([&i_lhs,&i_rhs,&res](const ddk::function<void()>& i_func)
	{
		ddk::future<ddk::detail::void_t> visitationRes = i_lhs.enqueue(ddk::make_function([payload = i_func]()
		{
			ddk::eval(payload);

			return ddk::_void;
		}));

		res = (res) ? ddk::fusion(std::move(res).extract(),std::move(visitationRes))
		.then(ddk::make_function([](std::array<ddk::detail::void_t,2> i_res) -> ddk::detail::void_t
		{
			return ddk::_void;
		})) : std::move(visitationRes);
	});

	const ddk::iterable_result iterRes = execute_iteration(newIteration);

	if (res)
	{
		return std::move(res).extract()
		.then(ddk::make_function([iterationResult = iterRes](ddk::detail::void_t)
		{
			return iterationResult;
		}));
	}

	//no worry, this is deferred
	return ddk::async(ddk::make_function([iterRes]() { return iterRes; }));
}