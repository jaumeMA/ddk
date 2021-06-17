
#include "ddk_allocator_exceptions.h"
#include "ddk_reference_wrapper.h"
#include "ddk_container_exceptions.h"

#define LESS_THAN(_A,_B) eval(m_compare,_A,_B)

namespace ddk
{
namespace detail
{

template<typename Key,typename Value>
template<typename ... Args>
map_node<Key,Value>::map_node(const Key& i_key, Args&& ... i_args)
: m_value(i_key,std::forward<Args>(i_args)...)
{
}
template<typename Key,typename Value>
map_node<Key,Value>::~map_node()
{
    if(m_leftChild)
    {
        m_leftChild->set_parent(nullptr);
    }

    if(m_rightChild)
    {
        m_rightChild->set_parent(nullptr);
    }
}
template<typename Key,typename Value>
void map_node<Key,Value>::replace_node(unique_reference_wrapper<map_node<Key,Value>> other)
{
    if(other)
    {
        //set descendants
        if(m_leftChild && m_leftChild != other)
        {
            other->m_leftChild = std::move(m_leftChild);
            other->m_leftChild->set_parent(ddk::lend(other));
        }
        if(m_rightChild && m_rightChild != other)
        {
            other->m_rightChild = std::move(m_rightChild);
            other->m_rightChild->set_parent(ddk::lend(other));
        }

        //set the parent
        if(m_parent)
        {
            if(this == m_parent->m_leftChild)
            {
                m_parent->m_leftChild = std::move(other);
            }
            else
            {
                m_parent->m_rightChild = std::move(other);
            }
        }

        other->set_parent(m_parent);

        m_parent = nullptr;
        m_rightChild = nullptr;
        m_leftChild = nullptr;
    }
}
template<typename Key,typename Value>
template<typename Map>
unique_reference_wrapper<map_node<Key,Value>> map_node<Key,Value>::extract_node(lent_pointer_wrapper<Map> i_map)
{
    if(m_parent)
    {
        lent_pointer_wrapper<map_node> parent = std::move(m_parent);

        return (parent->m_leftChild.get() == this) ? promote_to_ref(std::move(parent->m_leftChild)) : promote_to_ref(std::move(parent->m_rightChild));
    }
    else if(i_map)
    {
        return promote_to_ref(std::move(i_map->m_root));
    }
    else
    {
        throw bad_access_exception{"Accessing orphane node"};
    }
}
template<typename Key,typename Value>
lent_pointer_wrapper<map_node<Key,Value>> map_node<Key,Value>::get_parent()
{
    return m_parent;
}
template<typename Key,typename Value>
void map_node<Key,Value>::set_parent(lent_pointer_wrapper<map_node<Key,Value>> parent)
{
    m_parent = parent;
}
template<typename Key,typename Value>
void map_node<Key,Value>::set_left_child(unique_pointer_wrapper<map_node<Key,Value>> i_node)
{
    m_leftChild = std::move(i_node);
}
template<typename Key,typename Value>
map_node_lent_ptr<Key,Value> map_node<Key,Value>::get_left_child()
{
    return lend(m_leftChild);
}
template<typename Key,typename Value>
unique_pointer_wrapper<map_node<Key,Value>> map_node<Key,Value>::extract_left_child()
{
    return std::move(m_leftChild);
}
template<typename Key,typename Value>
void map_node<Key,Value>::set_right_child(unique_pointer_wrapper<map_node<Key,Value>> i_node)
{
    m_rightChild = std::move(i_node);
}
template<typename Key,typename Value>
map_node_lent_ptr<Key,Value> map_node<Key,Value>::get_right_child()
{
    return lend(m_rightChild);
}
template<typename Key,typename Value>
unique_pointer_wrapper<map_node<Key,Value>> map_node<Key,Value>::extract_right_child()
{
    return std::move(m_rightChild);
}
template<typename Key,typename Value>
std::pair<const Key,Value>& map_node<Key,Value>::get_value()
{
    return m_value;
}
template<typename Key,typename Value>
const std::pair<const Key,Value>& map_node<Key,Value>::get_value() const
{
    return m_value;
}

template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::map_iterator(lent_pointer_wrapper<map_impl> i_mapImpl,map_node_lent_ptr i_currNode)
: m_mapImpl(i_mapImpl)
, m_currNode(i_currNode)
{
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::reference map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator*()
{
    return reference(m_currNode->get_value().first,m_currNode->get_value().second);
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::const_reference map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator*() const
{
    return const_reference(m_currNode->get_value().first,m_currNode->get_value().second);
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator++() const
{
    m_currNode = (m_currNode) ? m_mapImpl->get_next_elem(promote_to_ref(m_currNode)) : nullptr;

    return *this;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator++(int) const
{
    map_iterator res = *this;

    if(m_mapImpl)
    {
        m_currNode = (m_currNode) ? m_mapImpl->get_next_elem(promote_to_ref(m_currNode)) : nullptr;
    }

    return res;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator--() const
{
    if(m_mapImpl)
    {
        m_currNode = (m_currNode) ? m_mapImpl->get_prev_elem(promote_to_ref(m_currNode)) : nullptr;
    }

    return *this;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator--(int) const
{
    map_iterator res = *this;

    m_currNode = (m_currNode) ? m_mapImpl->get_prev_elem(promote_to_ref(m_currNode)) : nullptr;

    return res;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
bool map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator==(const map_iterator& other) const
{
    if(m_mapImpl && other.m_mapImpl)
    {
        return m_currNode == other.m_currNode;
    }
    else
    {
        return m_mapImpl == other.m_mapImpl;
    }
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
bool map_impl<Key,Value,Node,Allocator,Balancer>::map_iterator::operator!=(const map_iterator& other) const
{
    if(m_mapImpl && other.m_mapImpl)
    {
        return m_currNode != other.m_currNode;
    }
    else
    {
        return m_mapImpl != other.m_mapImpl;
    }
}

template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::const_map_iterator(lent_pointer_wrapper<const map_impl> i_mapImpl, map_node_const_lent_ptr i_currNode)
: m_mapImpl(i_mapImpl)
, m_currNode(i_currNode)
{
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::const_map_iterator(const map_iterator& other)
: m_mapImpl(i_mapImpl)
, m_currNode(i_currNode)
{
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::const_reference map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator*() const
{
    return const_reference(m_currNode->m_value.first,m_currNode->m_value.second);
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator++() const
{
    m_currNode = (m_currNode) ? m_mapImpl->get_next_elem(promote_to_ref(m_currNode)) : nullptr;

    return *this;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator++(int) const
{
    const_map_iterator res = *this;

    if(m_mapImpl)
    {
        m_currNode = (m_currNode) ? m_mapImpl->get_next_elem(promote_to_ref(m_currNode)) : nullptr;
    }

    return res;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator--() const
{
    if(m_mapImpl)
    {
        m_currNode = (m_currNode) ? m_mapImpl->get_prev_elem(promote_to_ref(m_currNode)) : nullptr;
    }

    return *this;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator--(int) const
{
    const_map_iterator res = *this;

    m_currNode = (m_currNode) ? m_mapImpl->get_prev_elem(promote_to_ref(m_currNode)) : nullptr;

    return res;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
bool map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator==(const const_map_iterator& other) const
{
    if(m_mapImpl && other.m_mapImpl)
    {
        return m_currNode == other.m_currNode;
    }
    else
    {
        return m_mapImpl == other.m_mapImpl;
    }
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
bool map_impl<Key,Value,Node,Allocator,Balancer>::const_map_iterator::operator!=(const const_map_iterator& other) const
{
    if(m_mapImpl && other.m_mapImpl)
    {
        return m_currNode != other.m_currNode;
    }
    else
    {
        return m_mapImpl != other.m_mapImpl;
    }
}

template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
template<typename Arg>
void map_impl<Key,Value,Node,Allocator,Balancer>::assign(const_key_reference key, Arg&& i_arg)
{
    this->operator[](key) = std::forward<Arg>(val);
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::create_tree(const map_impl& i_tree)
{
    if(map_node_const_lent_ptr rootNode = ddk::lend(i_tree.m_root))
    {
        m_rootNode = allocate(promote_to_ref(rootNode));
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::clear_tree()
{
    if(m_root)
    {
        if(lent_pointer_wrapper<Node> leftChild = m_root->get_left_child())
        {
            leftChild->set_parent(nullptr);
        }

        if(lent_pointer_wrapper<Node> rightChild = m_root->get_right_child())
        {
            rightChild->set_parent(nullptr);
        }

        m_root = nullptr;
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::erase_node(map_node_lent_ref i_node)
{
    if(map_node_lent_ptr otherParent = i_node->get_parent())
    {
        if(i_node->m_leftChild && i_node->m_rightChild)
        {
            map_node_lent_ref rightMostNode = findMax(promote_to_ref(i_node->m_leftChild));

            rightMostNode->m_parent->m_rightChild = rightMostNode->m_leftChild;

            if(i_node == m_root)
            {
                m_root = rightMostNode;
            }

            i_node->replace_node(rightMostNode);
        }
        else if(pairNode->m_leftChild)
        {
            if(i_node == m_root)
            {
                m_root = i_node->m_leftChild;
            }

            i_node->replace_node(i_node->m_leftChild);
        }
        else if(i_node->m_rightChild)
        {
            if(i_node == m_root)
            {
                m_root = pairNode->m_rightChild;
            }

            i_node->replace_node(i_node->m_rightChild);
        }
        else
        {
            if(i_node->m_parent)
            {
                if(i_node == i_node->m_parent->m_leftChild)
                {
                    i_node->m_parent->m_leftChild = nullptr;
                }
                else
                {
                    i_node->m_parent->m_rightChild = nullptr;
                }
            }
            else
            {
                m_root = nullptr;
            }
        }

        _onNodeErased(static_cast<MapNode*>(pairNode));
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_unique_ref map_impl<Key,Value,Node,Allocator,Balancer>::allocate(map_node_const_lent_ref i_node) const
{
    if(void* mem = (void*)m_alloc.allocate(1))
    {
        map_node_unique_ref newNode = as_unique_reference(new(mem) map_node_t(i_node->m_value.first,i_node->m_value.second),{ ddk::lend(m_alloc),ddk::AllocationMode::AllocationOnly });

        if(i_node->m_leftChild)
        {
            newNode->m_leftChild = allocate(promote_to_ref(lend(i_node->m_leftChild)));
            newNode->m_leftChild->set_parent(lend(newNode));
        }

        if(i_node->m_rightChild)
        {
            newNode->m_rightChild = allocate(promote_to_ref(lend(i_node->m_pRightChild)));
            newNode->m_rightChild->set_parent(lend(newNode));
        }

        return std::move(newNode);
    }
    else
    {
        throw bad_allocation_exception{"Allocation could not be done."};
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
template<typename ... Args>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_unique_ref map_impl<Key,Value,Node,Allocator,Balancer>::allocate(const_key_reference key, Args&& ... i_args) const
{
    if(void* mem = (void*)m_alloc.allocate(1))
    {
        return as_unique_reference(new(mem) map_node_t(key,std::forward<Args>(i_args)...),{ ddk::lend(m_alloc),ddk::AllocationMode::AllocationOnly });
    }
    else
    {
        throw bad_allocation_exception{ "Allocation could not be done." };
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ref map_impl<Key,Value,Node,Allocator,Balancer>::findMin(map_node_lent_ref node) const
{
    map_node_lent_ref currNode = node;

    while(map_node_lent_ptr leftChild = currNode->get_left_child()) currNode = promote_to_ref(leftChild);

    return currNode;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ref map_impl<Key,Value,Node,Allocator,Balancer>::findMax(map_node_lent_ref node) const
{
    map_node_lent_ref currNode = node;

    while(map_node_lent_ptr rightChild = currNode->m_rightChild) currNode = promote_ro_ref(rightChild);

    return currNode;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ref map_impl<Key,Value,Node,Allocator,Balancer>::add_node(const std::pair<const Key,Value>& i_value)
{
    if(map_node_lent_ptr root = lend(m_root))
    {
        map_node_lent_ref foundNode = get_closest_bigger_node(i_value.first,root);

        if(foundNode->m_value.first == i_value.first)
        {
            foundNode->m_value.second = std::forward<typename mpl::PairType<R,T>::second_type>(i_value.second);

            return foundNode;
        }
        else
        {
            map_node_unique_ref newNode = allocate(std::forward<Key>(i_value.first),std::forward<Value>(i_value.second));

            map_node_lent_ref refNewNode = lend(newNode);

            insert(std::move(newNode),foundNode);

            this->_onNodeInserted(refNewNode);

            return refNewNode;
        }
    }
    else
    {
        m_root = allocate(std::forward<Key>(i_value.first),std::forward<Value>(i_value.second));

        this->_onNodeInserted(lend(m_root));

        return lend(m_root);
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl()
: m_alloc(allocator_t{})
, m_compare(ddk::make_function([](const_key_reference i_lhs,const_key_reference i_rhs) { return i_lhs < i_rhs; }))
{
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl(const compare_func& i_compare)
: m_alloc(allocator_t{})
, m_compare(i_compare)
{
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl(const_reference value,const_key_reference key)
: m_alloc(allocator_t{})
, m_compare(ddk::make_function([](const_key_reference i_lhs,const_key_reference i_rhs) { return i_lhs < i_rhs; }))
{
    add_node(std::make_pair(key,value));
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl(const_reference value,const_key_reference key,const compare_func& i_compare)
: m_alloc(allocator_t{})
, m_compare(i_compare)
{
    add_node(std::make_pair(key,value));
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl(const map_impl& other)
: m_alloc(allocator_t{})
, m_compare(other.m_compare)
{
    create_tree(other);
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::map_impl(map_impl&& other)
: m_alloc(allocator_t{})
, m_compare(std::move(other.m_compare))
, m_root(std::move(other.m_root))
{
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>::~map_impl()
{
    clear_tree();
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>& map_impl<Key,Value,Node,Allocator,Balancer>::operator=(const map_impl& other)
{
    clear_tree();

    create_tree(other);

    m_compare = other.m_compare;

    return *this;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
map_impl<Key,Value,Node,Allocator,Balancer>& map_impl<Key,Value,Node,Allocator,Balancer>::operator=(map_impl&& other)
{
    clear_tree();

    this->setRootNode(other.getRootNode());

    other.setRootNode(iterable_type::m_pHead);

    m_compare = mpl::move(other.m_compare);

    return *this;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_iterator map_impl<Key,Value,Node,Allocator,Balancer>::find(const_key_reference key) const
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        return {*this,search(key,detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode))};
    }

    return {};
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::iterator map_impl<Key,Value,Node,Allocator,Balancer>::find(const_key_reference key)
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        return {*this,search(key,detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode))};
    }

    return {};
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_reference map_impl<Key,Value,Node,Allocator,Balancer>::operator[](const_key_reference key) const
{
    if(m_root)
    {
        if(map_node_lent_ptr foundNode = search(key,promote_to_ref(lend(m_root))))
        {
            return foundNode->m_value.second;
        }
        else
        {
            throw bad_access_exception{ "Trying to access non existent entry." };
        }
    }
    else
    {
        throw bad_access_exception{"Trying to access empty map."};
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::reference map_impl<Key,Value,Node,Allocator,Balancer>::operator[](const_key_reference key)
{
    map_node_lent_ptr foundNode = (m_root) ? map_node_lent_ptr{get_closest_bigger_node(key,promote_to_ref(ddk::lend(m_root)))} : nullptr;

    if(foundNode && foundNode->get_value().first == key)
    {
        return foundNode->get_value().second;
    }
    else
    {
        map_node_unique_ref newNode = allocate(key,Value{});

        map_node_lent_ref newNodeRef = ddk::lend(newNode);

        if(m_root)
        {
            insert(std::move(newNode),(foundNode) ? promote_to_ref(foundNode) : promote_to_ref(ddk::lend(m_root)));
        }
        else
        {
            m_root = std::move(newNode);
        }

        this->_onNodeInserted(newNodeRef);

        return newNodeRef->get_value().second;
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
template<typename ... Args>
std::pair<bool,typename map_impl<Key,Value,Node,Allocator,Balancer>::iterator> map_impl<Key,Value,Node,Allocator,Balancer>::emplace(const_key_reference i_key, Args&& ... i_args)
{
    map_node_lent_ptr foundNode = (m_root) ? map_node_lent_ptr{get_closest_bigger_node(key,promote_to_ref(ddk::lend(m_root)))} : nullptr;

    if(foundNode && foundNode->m_value.first == i_key)
    {
        return make_pair<bool,iterator_type>(false,this->constructIteratorAtNode(foundNode));
    }
    else
    {
        map_node_unique_ref newNode = allocate(i_key,std::forward<TT>(i_args)...);

        map_node_lent_ref newNodeRef = ddk::lend(newNode);

        if(m_root)
        {
            insert(std::move(newNode),(foundNode) ? promote_to_ref(foundNode) : ddk::lend(m_root));
        }
        else
        {
            m_root = std::move(newNode);
        }

        _onNodeInserted(newNodeRef);

        return make_pair<bool,iterator>(true,{*this,newNodeRef});
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::clear()
{
    eraseSubTree(*this);

    return;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::iterator map_impl<Key,Value,Node,Allocator,Balancer>::begin()
{
    if(map_node_lent_ptr firstElem = get_first_elem())
    {
        return { this->ref_from_this(),promote_to_ref(firstElem) };
    }
    else
    {
        return {};
    }
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_iterator map_impl<Key,Value,Node,Allocator,Balancer>::begin() const
{
    if(map_node_lent_ptr firstElem = get_first_elem())
    {
        return { this->ref_from_this(),promote_to_ref(firstElem) };
    }
    else
    {
        return {};
    }
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::iterator map_impl<Key,Value,Node,Allocator,Balancer>::end()
{
    return { this->ref_from_this(),nullptr };
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::const_iterator map_impl<Key,Value,Node,Allocator,Balancer>::end() const
{
    return { this->ref_from_this(),nullptr };
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
size_t map_impl<Key,Value,Node,Allocator,Balancer>::size() const
{
    if(map_node_lent_ptr currElem = get_first_elem())
    {
        size_t res = 1;

        while(currElem = get_next_elem(promote_to_ref(currElem))) ++res;

        return res;
    }

    return 0;
}
template<typename Key,typename Value,typename Node,template<typename> class Allocator,template<typename,typename> class Balancer>
bool map_impl<Key,Value,Node,Allocator,Balancer>::empty() const
{
    return m_root == nullptr;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ptr map_impl<Key,Value,Node,Allocator,Balancer>::get_first_elem() const
{
    if(map_node_lent_ptr rootNode = ddk::lend(m_root))
    {
        return findMin(promote_to_ref(rootNode));
    }
    else
    {
        return nullptr;
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ptr map_impl<Key,Value,Node,Allocator,Balancer>::get_last_elem() const
{
    if(map_node_lent_ptr rootNode = ddk::lend(m_root))
    {
        return findMax(promote_to_ref(rootNode));
    }
    else
    {
        return nullptr;
    }
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ptr map_impl<Key,Value,Node,Allocator,Balancer>::get_next_elem(map_node_lent_ref i_currNode) const
{
    map_node_lent_ptr nextElem = nullptr;

    map_node_lent_ptr rightChild = i_currNode->get_right_child();

    if(map_node_lent_ptr currParent = i_currNode->get_parent())
    {
        if(currParent->get_left_child() == i_currNode)
        {
            nextElem = (rightChild) ? findMin(promote_to_ref(rightChild)) : currParent;
        }
        else if(rightChild)
        {
            nextElem = findMin(promote_to_ref(rightChild));
        }
        else
        {
            //look for the first parent which is the left one
            map_node_lent_ptr currGrandParent = i_currNode->get_parent();
            map_node_lent_ptr rightGrandSon = currGrandParent->get_right_child();

            while(currGrandParent && currParent == rightGrandSon)
            {
                currParent = currGrandParent;
                currGrandParent = currParent->get_parent();
            }

            nextElem = currGrandParent;
        }
    }
    else if(rightChild)
    {
        nextElem = findMin(promote_to_ref(rightChild));
    }

    return nextElem;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ptr map_impl<Key,Value,Node,Allocator,Balancer>::get_prev_elem(map_node_lent_ref i_currNode) const
{
    map_node_lent_ptr prevElem = nullptr;

    map_node_lent_ptr leftChild = i_currNode->get_left_child();

    if(map_node_lent_ptr currParent = i_currNode->get_parent())
    {
        if(currParent->get_right_child() == i_currNode)
        {
            prevElem = (leftChild) ? findMax(promote_to_ref(leftChild)) : currNodeParent;
        }
        else if(leftChild)
        {
            prevElem = findMax(promote_to_ref(leftChild));
        }
        else
        {
            //look for the first parent which is the left one
            map_node_lent_ptr currGrandParent = i_currNode->get_parent();
            map_node_lent_ptr leftGrandSon = currGrandParent->get_left_child();

            while(currGrandParent && currParent == leftGrandSon)
            {
                currParent = currGrandParent;
                currGrandParent = currParent->get_parent();
            }

            prevElem = currNodeGrandParent;
        }
    }
    else if(leftChild)
    {
        return findMax(promote_to_ref(leftChild));
    }

    return prevElem;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::_onNodeInserted(map_node_lent_ref node)
{
    m_root = balancer_t::balance(this->ref_from_this(),node);
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::_onNodeErased(map_node_lent_ref node)
{
    m_root = balancer_t::balance(this->ref_from_this(),node);
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ref map_impl<Key,Value,Node,Allocator,Balancer>::get_closest_bigger_node(const_key_reference i_key, map_node_lent_ref i_rootNode) const
{
    map_node_lent_ptr currNode = i_rootNode;
    map_node_lent_ref candidate = i_rootNode;

    while(currNode)
    {
        if(i_key == currNode->get_value().first)
        {
            candidate = promote_to_ref(currNode);

            break;
        }
        else if(LESS_THAN(i_key,currNode->get_value().first))
        {
            candidate = promote_to_ref(currNode);

            currNode = currNode->get_left_child();
        }
        else
        {
            currNode = currNode->get_right_child();
        }
    }

    return candidate;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ref map_impl<Key,Value,Node,Allocator,Balancer>::get_closest_smaller_node(const_key_reference i_key, map_node_lent_ref i_rootNode) const
{
    map_node_lent_ptr currNode = i_rootNode;
    map_node_lent_ref candidate = i_rootNode;

    while(currNode)
    {
        if(i_key == currNode->get_value().first)
        {
            candidate = promote_to_ref(currNode);

            break;
        }
        else if(LESS_THAN(i_key,currNode->get_value().first))
        {
            currNode = currNode->get_left_child();
        }
        else
        {
            candidate = currNode;

            currNode = currNode->get_right_child();
        }
    }

    return candidate;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
typename map_impl<Key,Value,Node,Allocator,Balancer>::map_node_lent_ptr map_impl<Key,Value,Node,Allocator,Balancer>::search(const Key& key, map_node_lent_ref i_rootNode) const
{
    map_node_lent_ptr currNode = i_rootNode;
    map_node_lent_ptr candidate = nullptr;

    while(currNode)
    {
        if(key == currNode->m_value.first)
        {
            candidate = currNode;

            break;
        }
        else if(LESS_THAN(key,currNode->get_value().first))
        {
            currNode = currNode->get_left_child();
        }
        else
        {
            currNode = currNode->get_right_child();
        }
    }

    return candidate;
}
template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
void map_impl<Key,Value,Node,Allocator,Balancer>::insert(map_node_unique_ref other, map_node_lent_ref i_rootNode) const
{
    map_node_lent_ref currNode = i_rootNode;

    while(true)
    {
        if(LESS_THAN(other->get_value().first,currNode->get_value().first))
        {
            if(map_node_lent_ptr leftChild = currNode->get_left_child())
            {
                currNode = promote_to_ref(leftChild);
            }
            else
            {
                other->set_parent(currNode);
                currNode->set_left_child(std::move(other));

                break;
            }
        }
        else
        {
            if(map_node_lent_ptr rightChild = currNode->get_right_child())
            {
                currNode = promote_to_ref(rightChild);
            }
            else
            {
                other->set_parent(currNode);
                currNode->set_right_child(std::move(other));

                break;
            }
        }
    }
}

}
}