#pragma once

#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename Sink, typename Adaptor>
struct sink_adaptor
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;

public:
	typedef typename adaptor_t::traits traits;
	typedef typename adaptor_t::const_traits const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	TEMPLATE(typename AAdaptor)
	REQUIRES(IS_CONSTRUCTIBLE(Adaptor,AAdaptor))
	sink_adaptor(const Sink& i_sink, AAdaptor&& i_adaptor);
	template<typename AAdaptor, typename ActionTag>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, ActionTag&& i_action);

private:
	Sink m_sink;
	Adaptor m_adaptor;
};
template<typename Sink, typename Adaptor>
sink_adaptor(const Sink&, Adaptor&&) -> sink_adaptor<Sink,Adaptor&&>;

}
}

#include "ddk_iterable_sink_adaptor.inl"