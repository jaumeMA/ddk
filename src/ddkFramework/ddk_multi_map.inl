
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(const Value& i_value)
: map_t()
, m_value(i_value)
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(Value&& i_value)
: map_t()
, m_value(std::move(i_value))
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(const multi_map_value& other)
: map_t(other)
, m_value(other.m_value)
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(multi_map_value&& other)
: map_t(std::move(other))
, m_value(std::move(other.m_value))
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
lent_pointer_wrapper<typename multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value> multi_map_value<Key,Value,Map,Allocator,Balancer>::get_holder()
{
    return lend(m_pHolder);
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
lent_pointer_wrapper<const typename multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value> multi_map_value<Key,Value,Map,Allocator,Balancer>::get_holder() const
{
    return lend(m_pHolder);
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
lent_pointer_wrapper<typename multi_map_value<Key,Value,Map,Allocator,Balancer>::map_node_t> multi_map_value<Key,Value,Map,Allocator,Balancer>::get_node_location() const
{
    return m_pNodeLocation;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
void multi_map_value<Key,Value,Map,Allocator,Balancer>::set_holder(lent_reference_wrapper<multi_map_value> i_parent)
{
    if(m_holder != i_parent)
    {
        m_holder = i_parent;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
void multi_map_value<Key,Value,Map,Allocator,Balancer>::set_holder(const std::nullptr_t&)
{
    m_holder = nullptr;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(const multi_map_value& other)
{
    m_value = other.m_value;

    map_t::operator=(other);

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(const Value& i_val)
{
    m_value = i_val;

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(multi_map_value&& other)
{
    m_value = std::move(other.m_value);

    map_t::operator=(std::move(other));

    set_references();

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(Value&& i_val)
{
    m_value = std::move(i_val);

    set_references();

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::get_value()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::getValue() const
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::operator reference()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::operator const_reference() const
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
size_t multi_map_value<Key,Value,Map,Allocator,Balancer>::size(const function<bool(const_reference)>& filter) const
{
    size_t res = 0;

    const_iterator itPoliThis = this->begin();

    for(; itPoliThis != this->end(); itPoliThis++)
    {
        if(filter == nullptr || filter(*itPoliThis))
        {
            res++;
        }

        res += itPoliThis->second.size();
    }

    return res;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
void multi_map_value<Key,Value,Map,Allocator,Balancer>::set_node_location(lent_reference_wrapper<map_node_t> i_loc)
{
    m_nodeLocation = i_loc;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
void multi_map_value<Key,Value,Map,Allocator,Balancer>::set_references()
{
    if(lent_pointer_wrapper<map_node_t> currNode = static_lent_cast<map_node_t>(this->get_first_elem()))
    {
        do
        {
            currNode->get_value().second.set_holder(static_lent_cast<multi_map_value>(this->ref_from_this()));

            currNode->get_value().second.set_node_location(promote_to_ref(currNode));

            currNode->get_value().second.set_references();
        }
        while(currNode = static_lent_cast<map_node_t>(this->get_next_elem(promote_to_ref(currNode))));
    }
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at() const
{
    return m_value;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key,Keys&& ... i_keys)
{
    multi_map_value<Key,Value,Map,Allocator,Balancer>& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key,Keys&& ... i_keys) const
{
    const multi_map_value<Key,Value,Map,Allocator,Balancer>& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
multi_map_impl<Key,Value,Map,Allocator,Balancer>::multi_map_impl(const multi_map_impl& other)
: map_t(other)
{
    set_references();
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
multi_map_impl<Key,Value,Map,Allocator,Balancer>& multi_map_impl<Key,Value,Map,Allocator,Balancer>::operator=(const multi_map_impl& other)
{
    map_t::operator=(other);

    set_references();

    return *this;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
size_t multi_map_impl<Key,Value,Map,Allocator,Balancer>::size(const function<bool(const_reference)>& filter) const
{
    size_t res = 0;

    const_iterator itPoliThis = this->begin();

    for(; itPoliThis != this->end(); itPoliThis++)
    {
        if(filter == nullptr || eval(filter,*itPoliThis))
        {
            res++;
        }

        res += itPoliThis->second.getTotalSize(filter);
    }

    return res;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_impl<Key,Value,Map,Allocator,Balancer>::underlying_type& multi_map_impl<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key, Keys&& ... i_keys)
{
    Value& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
const typename multi_map_impl<Key,Value,Map,Allocator,Balancer>::underlying_type& multi_map_impl<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key, Keys&& ... i_keys) const
{
    const Value& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
void multi_map_impl<Key,Value,Map,Allocator,Balancer>::set_references()
{
    if(lent_pointer_wrapper<map_node_t> currNode = static_lent_cast<map_node_t>(this->get_first_elem()))
    {
        const auto headNode = this->getHead();

        do
        {
            currNode->m_value.second.set_node_location(currNode);

            currNode->m_value.second.set_references();
        }
        while(currNode = static_lent_cast<map_node_t>(this->get_next_elem(currNode)));
    }
}

}
}