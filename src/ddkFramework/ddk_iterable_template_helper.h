#pragma once

#include <iterator>

namespace ddk
{

template<typename Iterable>
typename std::iterator_traits<typename Iterable::iterator>::iterator_category iterable_tag_resolver(const Iterable&);

}