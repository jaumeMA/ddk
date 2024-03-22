
namespace ddk
{
namespace detail
{

template<typename Sink, typename Adaptor>
TEMPLATE(typename AAdaptor)
REQUIRED(IS_CONSTRUCTIBLE(Adaptor,AAdaptor))
sink_adaptor<Sink,Adaptor>::sink_adaptor(const Sink& i_sink, AAdaptor&& i_adaptor)
: m_sink(i_sink)
, m_adaptor(std::forward<AAdaptor>(i_adaptor))
{
}
template<typename Sink, typename Adaptor>
template<typename AAdaptor,typename ActionTag>
constexpr auto sink_adaptor<Sink,Adaptor>::perform_action(AAdaptor&& i_adaptor, ActionTag&& i_action)
{
	if constexpr (IS_ADAPTOR_REPRESENTABLE_BY_ACTION_COND(Adaptor,ActionTag))
	{
		if (auto actionRes = adaptor_t::perform_action(std::forward<AAdaptor>(i_adaptor).m_adaptor,std::forward<ActionTag>(i_action)))
		{
			ddk::terse_eval(i_adaptor.m_sink,actionRes.get());

			return actionRes;
		}
		else
		{
			return actionRes;
		}
	}
	else
	{
		return adaptor_t::perform_action(std::forward<AAdaptor>(i_adaptor).m_adaptor,std::forward<ActionTag>(i_action));
	}
}

}
}