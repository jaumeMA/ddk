
#include "ddk_iterable_action.h"

namespace ddk
{

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::iterable_adaptor(multi_map<Key,Value,Map,Allocator>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
auto iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::get_value()
{
    throw;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
auto iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::get_value() const
{
    throw;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
auto iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_value(Sink&& i_sink)
{
    return no_action{};
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
auto iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_value(Sink&& i_sink) const
{
    return no_action{};
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::perform_action(const begin_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::perform_action(const end_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::perform_action(const forward_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::perform_action(const backward_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::perform_action(const displace_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::valid() const
{
    return false;
}

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::iterable_adaptor(const multi_map<Key,Value,Map,Allocator>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
auto iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::get_value() const
{
    throw;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
auto iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::forward_value(Sink&& i_sink) const
{
    return no_action{};
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::perform_action(const begin_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::perform_action(const end_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::perform_action(const forward_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::perform_action(const backward_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::perform_action(const displace_action_tag&) const
{
    return false;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::valid() const
{
    return false;
}

}
