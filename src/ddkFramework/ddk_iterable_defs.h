#pragma once

namespace ddk
{

#define DDK_ITERABLE_TYPE(_ITERABLE,_ADAPTOR,_TAG) \
friend class _ADAPTOR; \
friend inline typename std::add_const<_ADAPTOR>::type& adaptor_resolver(const _ITERABLE&,...); \
friend _TAG iterable_tag_resolver(typename std::add_const<_ITERABLE>::type&);

}