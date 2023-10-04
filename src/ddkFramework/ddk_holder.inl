
namespace ddk
{

template<typename T>
holder<T>::holder(T& i_obj)
: m_obj(i_obj)
{
}
template<typename T>
holder<T>::operator bool() const
{
	return static_cast<bool>(m_obj);
}
template<typename T>
T& holder<T>::operator->()
{
	return m_obj;
}
template<typename T>
const T& holder<T>::operator->() const
{
	return m_obj;
}
template<typename T>
T& holder<T>::operator*()
{
	return m_obj;
}
template<typename T>
const T& holder<T>::operator*()const
{
	return m_obj;
}

}