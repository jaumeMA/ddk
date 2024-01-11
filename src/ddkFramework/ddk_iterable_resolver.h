#pragma once

#include "ddk_iterable_adaptor_resolver.h"
#include "ddk_iterable_traits.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
class iterable_impl;

template<typename>
struct iterable_traits_correspondence_resolver;

template<typename Adaptor>
struct iterable_traits_correspondence_resolver
{
	typedef detail::iterable_traits<typename Adaptor::traits> type;
};

template<typename>
struct iterable_correspondence_impl;

template<typename Iterable>
struct iterable_correspondence_impl<iterable<Iterable>>
{
	typedef iterable<Iterable> type;
};
template<typename Iterable>
struct iterable_correspondence_impl<const iterable<Iterable>>
{
	typedef const iterable<Iterable> type;
};
template<typename Iterable>
struct iterable_correspondence_impl
{
private:
	typedef typename iterable_traits_correspondence_resolver<iterable_adaptor<Iterable>>::type traits_t;

public:
	typedef iterable<iterable_impl<traits_t,Iterable>> type;
};

template<typename Iterable>
using iterable_correspondence = typename iterable_correspondence_impl<Iterable>::type;

}
}

#include "ddk_iterable_traits_resolver.h"