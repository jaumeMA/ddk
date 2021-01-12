#pragma once

namespace ddk
{

#define DDK_ITERABLE_TYPE(_ITERABLE,_ADAPTOR,_TAG) \
friend class _ADAPTOR; \
template<typename Action> \
friend inline typename std::add_const<_ADAPTOR>::type& adaptor_resolver(_ITERABLE&, Action&&); \
friend _TAG iterable_tag_resolver(const _ITERABLE&);

}
