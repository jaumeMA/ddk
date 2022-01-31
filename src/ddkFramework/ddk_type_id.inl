
namespace ddk
{

template<typename UnderlyingType,typename T>
Id<UnderlyingType,T>::Id(const UnderlyingType& i_key)
: m_key(i_key)
{
}
template<typename UnderlyingType,typename T>
Id<UnderlyingType,T>::Id(UnderlyingType&& i_key)
: m_key(std::move(i_key))
{
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator==(const Id & other) const
{
	return m_key == other.m_key;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator!=(const Id & other) const
{
	return m_key != other.m_key;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator==(const UnderlyingType & other) const
{
	return m_key == other;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator!=(const UnderlyingType & other) const
{
	return m_key != other;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator<(const Id & other) const
{
	return m_key < other.m_key;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator<=(const Id & other) const
{
	return m_key <= other.m_key;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator>(const Id & other) const
{
	return m_key > other.m_key;
}
template<typename UnderlyingType,typename T>
bool Id<UnderlyingType,T>::operator>=(const Id & other) const
{
	return m_key >= other.m_key;
}
template<typename UnderlyingType,typename T>
UnderlyingType* Id<UnderlyingType,T>::operator->()
{
	return &m_key;
}
template<typename UnderlyingType,typename T>
const UnderlyingType* Id<UnderlyingType,T>::operator->() const
{
	return m_key;
}
template<typename UnderlyingType,typename T>
const UnderlyingType& Id<UnderlyingType,T>::getValue() const
{
	return m_key;
}

template<typename UnderlyingType,typename T>
Id<UnderlyingType,T> formatter<Id<UnderlyingType,T>>::format(const std::string& i_str)
{
	return Id<UnderlyingType,T>(formatter<UnderlyingType>::format(i_str));
}

}

namespace std
{

template<typename UnderlyingType,typename T>
std::size_t hash<ddk::Id<UnderlyingType,T>>::operator()(const ddk::Id<UnderlyingType,T> s) const noexcept
{
	return std::hash<UnderlyingType>{}(s.getValue());
}

}