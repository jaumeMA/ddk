
#include "ddk_container_exceptions.h"

namespace ddk
{

template<typename T>
template<typename TT>
inherited_value<T>::value_wrapper<TT>::value_wrapper(const TT& i_value)
: m_value(i_value)
{}
template<typename T>
template<typename TT>
inherited_value<T>::value_wrapper_interface<T>* inherited_value<T>::value_wrapper<TT>::clone() const
{
    return new value_wrapper<TT>(m_value);
}
template<typename T>
template<typename TT>
void inherited_value<T>::value_wrapper<TT>::destroy()
{
    //if anybody access this data after destruction something  very bad will happen
    m_value.~TT();
}
template<typename T>
template<typename TT>
void inherited_value<T>::value_wrapper<TT>::setValue(const TT& i_value)
{
    m_value = i_value;
}
template<typename T>
template<typename TT>
const T& inherited_value<T>::value_wrapper<TT>::getValue() const
{
    return m_value;
}
template<typename T>
template<typename TT>
T& inherited_value<T>::value_wrapper<TT>::getValue()
{
    return m_value;
}

template<typename T>
inherited_value<T>::inherited_value()
: m_currentAgnosticType(k_invalidType)
{
}
template<typename T>
inherited_value<T>::inherited_value(const inherited_value& other)
{
    clone(other);
}
template<typename T>
template<typename TT>
inherited_value<T>::inherited_value(const TT& i_val, ...)
: m_currentAgnosticType(k_invalidType)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

    static const size_t s_TypeHashCode = ddk::getTypeId<T>();

    m_currentAgnosticType = s_TypeHashCode;

    m_arena = new value_wrapper<TT>(i_val);
}
template<typename T>
inherited_value<T>::~inherited_value()
{
    destroy();
}
template<typename T>
inherited_value<T>& inherited_value<T>::operator=(const inherited_value& other)
{
    destroy();

    clone(other);

    return *this;
}
template<typename T>
template<typename TT>
bool inherited_value<T>::isOfType() const
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");
    static const size_t s_TypeHashCode = ddk::getTypeId<T>();

    return m_currentAgnosticType == s_TypeHashCode;
}
template<typename T>
template<typename TT>
void inherited_value<T>::setValue(const TT& i_value)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

    static const size_t s_TypeHashCode = ddk::getTypeId<T>();

    if(m_currentAgnosticType == s_TypeHashCode)
    {
        value_wrapper<TT>* innerValue = dynamic_cast<value_wrapper<TT>*>(m_arena);

        innerValue->setValue(i_value);
    }
    else
    {
        destroy();

        m_currentAgnosticType = s_TypeHashCode;

        m_arena = new value_wrapper<TT>(i_value);
    }
}
template<typename T>
template<typename TT>
TT& inherited_value<T>::getValue()
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

    static const size_t s_TypeHashCode = ddk::getTypeId<T>();
    static const size_t s_InterfaceHashCode = ddk::getTypeId<T>();

    if(s_TypeHashCode == s_InterfaceHashCode || m_currentAgnosticType == s_TypeHashCode)
    {
        return dynamic_cast<TT&>(m_arena->getValue());
    }
    else
    {
        throw bad_access_exception{"Trying to access empty inherited value"};
    }
}
template<typename T>
template<typename TT>
const TT& inherited_value<T>::getValue() const
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

    static const size_t s_TypeHashCode = ddk::getTypeId<T>();
    static const size_t s_InterfaceHashCode = ddk::getTypeId<T>();

    if(s_TypeHashCode == s_InterfaceHashCode || m_currentAgnosticType == s_TypeHashCode)
    {
        return dynamic_cast<const TT&>(m_arena->getValue());
    }
    else
    {
        throw bad_access_exception{"Trying to access empty inherited value"};
    }
}
template<typename T>
inherited_value<T>::operator bool() const
{
    return m_currentAgnosticType != k_invalidType;
}
template<typename T>
size_t inherited_value<T>::getCurrTypeId() const
{
    return m_currentAgnosticType;
}
template<typename T>
void inherited_value<T>::destroy()
{
    if(m_currentAgnosticType != k_invalidType)
    {
        delete m_arena;

        m_arena = NULL;

        m_currentAgnosticType = k_invalidType;
    }
}
template<typename T>
void inherited_value<T>::clone(const inherited_value& other)
{
    if(other.m_currentAgnosticType != k_invalidType)
    {
        m_arena = other.m_arena->clone();
    }

    m_currentAgnosticType = other.m_currentAgnosticType;
}

}
